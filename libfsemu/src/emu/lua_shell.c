#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef WITH_LUA

#include "lua_shell.h"

#ifdef __cplusplus
extern "C" {
#endif
#include <lualib.h>
#ifdef __cplusplus
}
#endif

#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include <fs/net.h>
#include <fs/log.h>
#include <fs/thread.h>
#include <fs/conf.h>
#include <fs/emu_lua.h>

static const char *hello_msg = "FS-UAE " PACKAGE_VERSION " " LUA_VERSION "\r\n";
static const char *bye_msg = "bye.\r\n";
static const char *fail_msg = "FAILED!\r\n";
static const char *default_addr = "127.0.0.1";
static const char *default_port = "6800";
static fs_thread *g_listen_thread = NULL;
static int g_listen_fd = -1;
static int g_keep_listening;
static int g_client_fd = -1;

#define MAX_CMD_LEN     256

#ifdef WINDOWS
static int close(int socket) {
    return closesocket(socket);
}
#define SHUTDOWN_ARG    SD_BOTH
#else
#define SHUTDOWN_ARG    SHUT_RDWR
#endif

#ifdef MSG_NOSIGNAL
#define SEND_FLAG       MSG_NOSIGNAL
#else
#define SEND_FLAG       0
#endif

static int myprintf(int fd, const char *fmt, ...)
{
    va_list ap;
    char buf[128];

    va_start(ap, fmt);
    int n = vsnprintf(buf, 128, fmt, ap);
    va_end(ap);
    if(n>0) {
        return send(fd, buf, n, SEND_FLAG);
    } else {
        return 0;
    }
}

static void print_lua_error(int fd, lua_State *L)
{
    // pop error and print
    const char *err_msg = lua_tostring(L, -1);
    myprintf(fd, "ERROR: %s\r\n", err_msg);
    lua_pop(L,1);
}

// print replacement that writes to socket
static int l_my_print(lua_State* L)
{
    // retrieve fd via closure
    int fd = lua_tointeger(L, lua_upvalueindex(1));

    int n = lua_gettop(L);
    lua_getglobal(L, "tostring");
    for(int i=1; i<=n; i++) {
      lua_pushvalue(L, -1);  /* function to be called */
      lua_pushvalue(L, i);   /* value to print */
      lua_call(L, 1, 1);
      size_t l;
      const char *s = lua_tolstring(L, -1, &l);  /* get result */
      if (s == NULL) {
        return luaL_error(L,
           LUA_QL("tostring") " must return a string to " LUA_QL("print"));
      }
      if (i>1) {
          send(fd, "\t", 1, SEND_FLAG);
      }
      send(fd, s, l, SEND_FLAG);
      lua_pop(L, 1);  /* pop result */
    }
    send(fd, "\r\n", 2, SEND_FLAG);
    return 0;
}

static int l_my_quit(lua_State *L)
{
    int *quit_flag = (int *)lua_touserdata(L, lua_upvalueindex(1));
    *quit_flag = 1;
    return 0;
}

static void setup_shell_state(int fd, lua_State *L, int *quit_flag)
{
    // replace print function
    lua_pushinteger(L, fd);
    lua_pushcclosure(L, &l_my_print, 1);
    lua_setglobal(L, "print");

    // add a quit function
    lua_pushlightuserdata(L, quit_flag);
    lua_pushcclosure(L, &l_my_quit, 1);
    lua_setglobal(L, "quit");
}

static int handle_command(int fd, lua_State *L, const char *cmd_line)
{
    // parse and execute command
    if(luaL_loadbuffer(L, cmd_line, strlen(cmd_line), "=shell")
        || lua_pcall(L, 0, LUA_MULTRET, 0)) {
        print_lua_error(fd, L);
        return 0;
    }

    // is there a result? -> call print!
    if (lua_gettop(L) > 0) {
      luaL_checkstack(L, LUA_MINSTACK, "too many results to print");
      lua_getglobal(L, "print");
      lua_insert(L, 1);
      if (lua_pcall(L, lua_gettop(L)-1, 0, 0) != LUA_OK) {
          print_lua_error(fd, L);
      }
    }
    return 0;
}

static char* fixup_line(char *buf, int size)
{
    // overwrite last newline
    if(size>0) {
        size--;
    }
    buf[size] = '\0';

    // strip return if before newline
    if(size>0) {
        if(buf[size-1] == '\r') {
            buf[size-1] = '\0';
        }
    }

    // prepend 'return' ?
    if(buf[0]=='=') {
        buf -= 6;
        strcpy(buf,"return");
        buf[6] = ' ';
    }

    printf("lua: got line: '%s'\n", buf);

    return buf;
}

static char* read_line(int fd, const char *prompt, char *cmd_line, int len,
                       int *read_pos, int *bytes_left, int *error_flag)
{
    int result;

    // send prompt
    result = send(fd, prompt, strlen(prompt), SEND_FLAG);
    if(result < 0) {
        return NULL;
    }

    // leave room for prepending "return" in buffer
    char *buf = cmd_line + 6;
    len -= 7;

    // do we have still bytes left?
    int left = *bytes_left;
    int pos = *read_pos;
    int offset = 0;
    if(left > 0) {
        for(int i=0;i<left;i++) {
            // found a newline in buffer
            if(buf[pos+i] == '\n') {
                *bytes_left = left - 1 - i;
                *read_pos = pos + i + 1;
                return fixup_line(buf + pos, i+1);
            }
        }
        // no newline found in buffer
        // move left bytes to front of buffer
        if(pos > 0) {
            memcpy(buf, buf+pos, left);
        }
        len -= left;
        offset = left;
    }

    // read new data from socket
    char *ptr = buf + offset;
    while(len > 0) {
        result = recv(fd, ptr, len, 0);
        // error reading from socket
        if(result < 0) {
            if(errno != EAGAIN) {
                *error_flag = 1;
                return NULL;
            } else {
                continue;
            }
        }
        // socket was closed
        else if(result == 0) {
            *error_flag = 0;
            return NULL;
        }

        // contains read data a newline?
        for(int i=0;i<result;i++) {
            if(*(ptr+i) == '\n') {
                // extra data after newline?
                *read_pos = offset + i + 1;
                *bytes_left = result - 1 - i;
                return fixup_line(buf, offset+i+1);
            }
        }
        offset += result;
        len -= result;
        ptr += result;
    }
    return NULL;
}

static void main_loop(int fd, lua_State *L, int *quit_flag)
{
    char *cmd_line = (char *)malloc(MAX_CMD_LEN);
    if(cmd_line == NULL) {
        myprintf(fd, "ERROR: out of memory\r\n");
        return;
    }

    int result = 0;
    int read_pos = 0;
    int error_flag = 0;
    int bytes_left = 0;
    while(!*quit_flag) {
        // read a command
        char *line = read_line(fd, "> ", cmd_line, MAX_CMD_LEN,
                               &read_pos, &bytes_left, &error_flag);
        if(line == NULL) {
            if(error_flag) {
                myprintf(fd, "ERROR: error reading line\r\n");
            }
            break;
        }
        // exit shell? -> CTRL+D
        if(line[0] == '\x04') {
            myprintf(fd, "aborted.\r\n");
            break;
        }

        // execute command
        fs_emu_lua_lock_state(L);
        result = handle_command(fd, L, line);
        fs_emu_lua_unlock_state(L);
        if(result < 0) {
            myprintf(fd, "ERROR: command handling failed\r\n");
            break;
        }
    }

    // free command line
    free(cmd_line);

    // show final result
    if(result < 0) {
        myprintf(fd, fail_msg);
    } else {
        myprintf(fd, bye_msg);
    }

    fs_log("lua_shell: main loop done\n");
}

static void handle_client(int fd)
{
    fs_log("lua-shell: client connect\n");

    // welcome
    send(fd, hello_msg, strlen(hello_msg), SEND_FLAG);

    // create lua context
    lua_State *L = fs_emu_lua_create_state();
    if(L == NULL) {
        // error
    }
    else {
        // flag to tell shell end
        int quit_flag = 0;

        // setup state for shell
        setup_shell_state(fd, L, &quit_flag);

        // enter main loop
        main_loop(fd, L, &quit_flag);

        // free context
        fs_emu_lua_destroy_state(L);
    }

    // close connection
    close(fd);
    fs_log("lua-shell: client disconnect\n");
}

static void *lua_shell_listener(void *data)
{
    struct sockaddr_in client_addr;

    fs_log("lua-shell: +listener: %d\n", g_listen_fd);
    while(g_keep_listening) {
        // get next client
        socklen_t cli_size = sizeof(client_addr);
        int client_fd = accept(g_listen_fd, (struct sockaddr *)&client_addr, &cli_size);
        if(client_fd < 0) {
            fs_log("lua-shell: failed accept: %s\n", strerror(errno));
            break;
        }
        g_client_fd = client_fd;
        handle_client(client_fd);
        g_client_fd = -1;
    }
    fs_log("lua-shell: -listener\n");
    return NULL;
}

void fs_emu_lua_shell_init(void)
{
    // is shell enabled?
    if(!fs_config_get_boolean("lua_shell")) {
        fs_log("lua-shell: disabled\n");
        return;
    }

    // get config values
    const char *addr = fs_config_get_string("lua_shell_addr");
    if(addr == NULL) {
        addr = default_addr;
    }
    const char *port = fs_config_get_string("lua_shell_port");
    if(port == NULL) {
        port = default_port;
    }
    fs_log("lua-shell: addr=%s, port=%s\n", addr, port);

#ifdef WINDOWS
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD(2, 2);
    int wsa_err = WSAStartup(wVersionRequested, &wsaData);
    if (wsa_err != 0) {
        fs_emu_warning("ERROR: WSAStartup failed with error: %d\n", wsa_err);
        return;
    }
#endif

    // find address
    struct addrinfo hints;
    struct addrinfo *result;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    // Allow IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;
    int s = getaddrinfo(addr, port, &hints, &result);
    if (s != 0) {
        fs_log("lua-shell: getaddrinfo failed: %s\n", gai_strerror(s));
        return;
    }
    if(s > 1) {
        fs_log("lua-shell: found multiple address matches... taking first\n");
    }

    // try to open socket
    g_listen_fd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if(g_listen_fd < 0) {
        freeaddrinfo(result);
        fs_log("lua-shell: can't create socket: %s\n", strerror(errno));
        return;
    }

    // allow to rebind soon
    int yes = 1;
    if (setsockopt(g_listen_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(int)) == -1)
    {
        freeaddrinfo(result);
        close(g_listen_fd);
        fs_log("lua-shell: can't set socket option: %s\n", strerror(errno));
        return;
    }

    // bind socket
    if(bind(g_listen_fd, result->ai_addr, result->ai_addrlen) < 0) {
        freeaddrinfo(result);
        close(g_listen_fd);
        fs_log("lua-shell: can't bind socket: %s\n", strerror(errno));
        return;
    }

    // cleanup addrinfo
    freeaddrinfo(result);

    // start listening
    if(listen(g_listen_fd, 5) < 0) {
        close(g_listen_fd);
        fs_log("lua-shell: can't listen on socket: %s\n", strerror(errno));
        return;
    }

    // launch listener thread
    g_keep_listening = 1;
    g_listen_thread = fs_thread_create("lua_shell_listener",
                                       lua_shell_listener, NULL);
}

void fs_emu_lua_shell_free(void)
{
    fs_log("lua-shell: stopping... %d %d\n", g_client_fd, g_listen_fd) ;

    // close client socket
    if(g_client_fd >= 0) {
#if (defined(__APPLE__) || defined(WINDOWS))
        close(g_client_fd);
#else
        shutdown(g_client_fd, SHUTDOWN_ARG);
#endif
    }

    // close listener socket
    if(g_listen_fd >= 0) {
#if (defined(__APPLE__) || defined(WINDOWS))
        close(g_listen_fd);
#else
        shutdown(g_listen_fd, SHUTDOWN_ARG);
#endif
    }

    // end listener thread
    if(g_listen_thread != NULL) {
        fs_thread_wait(g_listen_thread);
        fs_thread_free(g_listen_thread);
        g_listen_thread = NULL;
    }

    fs_log("lua-shell: stopping done...\n");
}

#endif // WITH_LUA
