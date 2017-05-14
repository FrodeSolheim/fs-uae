#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef WINDOWS
#define WINVER 0x0502
#endif
#include <fs/emu.h>
#define _GNU_SOURCE 1
#include "netplay.h"

char *g_fs_emu_netplay_server = 0;
static fs_emu_checksum_function g_rand_checksum_function = 0;
static fs_emu_checksum_function g_state_checksum_function = 0;

int fs_emu_netplay_enabled() {
    return g_fs_emu_netplay_server != 0;
}

void fs_emu_set_rand_check_function(fs_emu_checksum_function function) {
    g_rand_checksum_function = function;
}

void fs_emu_set_state_check_function(fs_emu_checksum_function function) {
    g_state_checksum_function = function;
}

#ifdef WITH_NETPLAY

#ifdef WINDOWS
#include <Winsock2.h>
#include <Ws2tcpip.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>
#endif
#include <stdlib.h>
#include <string.h>

#include <fs/emu.h>
#include <fs/thread.h>

#include "hud.h"
#include "libfsemu.h"

#ifdef USE_GLIB
#include <glib.h>
#endif

#define FS_EMU_NETPLAY_PROTOCOL_VERSION 1

fs_emu_player g_fs_emu_players[MAX_PLAYERS] = {};
static int g_socket = 0;
//static gchar *g_port = "9999";
static volatile int g_frame = 0;
//static const char *g_hostname = "127.0.0.1";
static fs_thread *g_receive_thread = NULL;
static fs_thread *g_netplay_thread = NULL;
static fs_mutex *g_send_mutex = NULL;
static fs_mutex *g_connection_mutex = NULL;

static fs_condition *g_wait_for_frame_cond = NULL;
static fs_mutex *g_wait_for_frame_mutex = NULL;

char *g_fs_emu_netplay_port = "25100";
char g_fs_emu_netplay_tag[4];
unsigned char g_fs_emu_netplay_password[20 + 1] = {};
//uint32_t g_fs_emu_netplay_pass_checksum = 0;
uint32_t g_fs_emu_netplay_session_key = 0;
uint32_t g_fs_emu_netplay_resume_at_packet = 0;
int g_fs_emu_netplay_player = -1;
int g_fs_emu_netplay_num_players = 0;
int g_fs_emu_netplay_connected = 0;

// FIXME: move to emulator
static int g_fs_emu_netplay_emulation_version_major = PACKAGE_MAJOR;
static int g_fs_emu_netplay_emulation_version_minor = PACKAGE_MINOR;
static int g_fs_emu_netplay_emulation_version_revision = PACKAGE_REVISION;

#define TCP

#ifdef TCP
#else
static struct sockaddr g_server_address;
static int g_addrlen = 0;
#endif

#ifdef WINDOWS
int close(int socket) {
    return closesocket(socket);
}
#endif

#define BIT(x) (1 << x)
#define MESSAGE_HELLO     (1 << 24)

#define MESSAGE_EXT_MASK    (1 << 31)
#define MESSAGE_FRAME_MASK  (1 << 30)
#define MESSAGE_INPUT_MASK  (1 << 29)

#define MESSAGE_READY              0

#define MESSAGE_MEM_CHECK          5
#define MESSAGE_RND_CHECK          6
#define MESSAGE_PING               7
#define MESSAGE_PLAYERS            8
#define MESSAGE_PLAYER_TAG_0       9
#define MESSAGE_PLAYER_TAG_1      10
#define MESSAGE_PLAYER_TAG_2      11
#define MESSAGE_PLAYER_TAG_3      12
#define MESSAGE_PLAYER_TAG_4      13
#define MESSAGE_PLAYER_TAG_5      14
#define MESSAGE_PLAYER_PING       15
#define MESSAGE_PLAYER_LAG        16
#define MESSAGE_SET_PLAYER_TAG    17
#define MESSAGE_PROTOCOL_VERSION  18
#define MESSAGE_EMULATION_VERSION 19

#define MESSAGE_ERROR             20
#define MESSAGE_TEXT              21
#define MESSAGE_SESSION_KEY       22
#define MESSAGE_HALT              23

#define MESSAGE_MEMCHECK  (0x80000000 | (5 << 24))
#define MESSAGE_RNDCHECK  (0x80000000 | (6 << 24))

#define CREATE_EXT_MESSAGE(x, y) (0x80000000 | (x << 24) | (y & 0x00ffffff))

static fs_mutex* g_input_event_mutex;
static GQueue* g_input_event_queue;

static fs_emu_dialog* g_waiting_dialog = NULL;

static void show_waiting_dialog() {
    fs_emu_acquire_gui_lock();
    if (g_waiting_dialog) {
        return;
    }
    g_waiting_dialog = fs_emu_dialog_create(
            "Connected To Net Play Server", NULL, "Abort");
    //fs_emu_dialog_add_option(dialog, "Abort", 1);
    fs_emu_dialog_set_line(g_waiting_dialog, 0,
            "Waiting for game to start...");
    fs_emu_dialog_show(g_waiting_dialog);
    fs_emu_release_gui_lock();
}

static void dismiss_waiting_dialog() {
    if (g_waiting_dialog) {
        fs_emu_acquire_gui_lock();
        fs_emu_dialog_dismiss(g_waiting_dialog);
        fs_emu_dialog_destroy(g_waiting_dialog);
        g_waiting_dialog = NULL;
        fs_emu_release_gui_lock();
    }
}

const char *fs_emu_get_netplay_tag(int player) {
    if (player == -1) {
        player = g_fs_emu_netplay_player;
    }
    if (player >= 0 && player < MAX_PLAYERS) {
        printf("%d ---- %s\n", player, g_fs_emu_players[player].tag);
        return g_fs_emu_players[player].tag;
    }
    return "PLY";
}

int fs_emu_netplay_connected() {
    return g_fs_emu_netplay_connected;
}

void fse_init_netplay() {
    const char *value;

    g_send_mutex = fs_mutex_create();
    g_connection_mutex = fs_mutex_create();
    g_input_event_mutex = fs_mutex_create();
    g_input_event_queue = g_queue_new();
    g_wait_for_frame_cond = fs_condition_create();
    g_wait_for_frame_mutex = fs_mutex_create();

    value = fs_config_get_const_string("netplay_server");
    if (value) {
        g_fs_emu_netplay_server = g_strdup(value);
    }
    if (!fs_emu_netplay_enabled()) {
        return;
    }

    value = fs_config_get_const_string("netplay_tag");
    if (value) {
        strncpy(g_fs_emu_netplay_tag, value, 4);
    }
    else {
        g_fs_emu_netplay_tag[0] = 'U';
        g_fs_emu_netplay_tag[1] = 'N';
        g_fs_emu_netplay_tag[2] = 'K';
    }
    g_fs_emu_netplay_tag[3] = '\0';

    value = fs_config_get_const_string("netplay_port");
    if (value) {
        g_fs_emu_netplay_port = g_strdup(value);
    }

    char *password_value = fs_config_get_string("netplay_password");
    if (password_value) {
        GChecksum *cs = g_checksum_new(G_CHECKSUM_SHA1);
        g_checksum_update(cs, (unsigned char *) "FSNP", 4);
        int len = strlen(password_value);
        for (int i = 0; i < len; i++) {
            unsigned char c = password_value[i];
            // only include ASCII characters
            if (c < 128) {
                g_checksum_update(cs, &c, 1);
            }
        }
        gsize digest_len = 20;
        g_checksum_get_digest(cs, g_fs_emu_netplay_password, &digest_len);
        free(password_value);
        g_checksum_free(cs);
    }

}

static int fs_emu_get_netplay_input_event() {
    fs_mutex_lock(g_input_event_mutex);
    int input_event = FS_POINTER_TO_INT(g_queue_pop_tail(
            g_input_event_queue));
    fs_mutex_unlock(g_input_event_mutex);
    return input_event;
}

static void fs_emu_queue_netplay_input_event(int input_event) {
    if (input_event == 0) {
        fs_log("WARNING: tried to queue input event 0\n");
        return;
    }
    fs_mutex_lock(g_input_event_mutex);
    g_queue_push_head(g_input_event_queue, FS_INT_TO_POINTER(input_event));
    fs_mutex_unlock(g_input_event_mutex);
}

static void uint_to_bytes(uint32_t message, unsigned char* buffer) {
    buffer[0] = (message >> 24) & 0x0000ff;
    buffer[1] = (message >> 16) & 0x0000ff;
    buffer[2] = (message >> 8) & 0x0000ff;
    buffer[3] = (message) & 0x0000ff;
}

static uint32_t bytes_to_uint(unsigned char* buffer) {
    return (buffer[0] << 24) | (buffer[1] << 16) | (buffer[2] << 8) | \
            buffer[3];
}

static void fs_emu_netplay_on_disconnect() {
    dismiss_waiting_dialog();
    //fs_emu_log("fs_emu_netplay_on_disconnect\n");
    fs_mutex_lock(g_connection_mutex);
    if (!g_fs_emu_netplay_connected) {
        fs_mutex_unlock(g_connection_mutex);
        //fs_emu_log("fs_emu_netplay_on_disconnect - not connected\n");
        return;
    }
    fs_emu_log("fs_emu_netplay_on_disconnect - disconnecting\n");
    // FIXME: free socket
    // FIXME: reconnect or let the player continue without net play
    // FIXME: show onscreen connecting dialog with abort button...
    if (fs_emu_netplay_enabled()) {
        fs_emu_warning("disconnected or connection error");
        // for now, continue without netplay
        free(g_fs_emu_netplay_server);
        g_fs_emu_netplay_server = NULL;
        g_fs_emu_netplay_connected = 0;
        fs_emu_warning("emulator is now running in offline mode");
    }
    else {
        // netplay is no longer enabled, user has chosen to abort connection
        // and play offline / disconnect
        fs_emu_warning("disconnected");
        g_fs_emu_netplay_connected = 0;
    }
    fs_mutex_unlock(g_connection_mutex);
}

void fs_emu_netplay_disconnect() {
    close(g_socket);
    fs_emu_netplay_on_disconnect();
}

static void fs_emu_netplay_on_socket_error() {
    fs_emu_log("fs_emu_netplay_on_socket_error\n");
    fs_emu_netplay_on_disconnect();
}

static int send_bytes(void *buffer, int len) {
    /*
    static int temp_a = 0;
    if (((unsigned char*)buffer)[0] == 0x87) {
        temp_a++;
        printf("ping no %d\n", temp_a);
    }
    */
    fs_mutex_lock(g_send_mutex);
    int bytes_written = send(g_socket, (char *) buffer, len, 0);
    fs_mutex_unlock(g_send_mutex);
    if (bytes_written != len) {
        fs_emu_warning("ERROR: send returned %d (should be %d)\n",
                bytes_written, len);
        //printf("errno: %d\n", WSAGetLastError());
        // FIXME: DO NOT EXIT -- RECONNECT INSTEAD
        fs_emu_netplay_on_socket_error();
        return 0;
    }
    return 1;
}

static void send_message(uint32_t message) {
    unsigned char buffer[4];
    uint_to_bytes(message, buffer);
    send_bytes(buffer, 4);
}

int fs_emu_send_netplay_message(const char *text) {
    fs_log("send netplay message: %s\n", text);
    if (!fs_emu_netplay_connected()) {
        fs_log("-> not connected\n");
        return 0;
    }
    int len = strlen(text);
    static unsigned char buffer[5] = {};
    uint_to_bytes(CREATE_EXT_MESSAGE(MESSAGE_TEXT, len), buffer);
    fs_mutex_lock(g_send_mutex);
    int bytes_written = send(g_socket, (char *) buffer, 4, 0);
    bytes_written += send(g_socket, (char *) text, len, 0);
    fs_mutex_unlock(g_send_mutex);
    if (bytes_written != len + 4) {
        fs_emu_warning("ERROR: send returned %d (should be %d)\n",
                bytes_written, len + 4);
        fs_emu_netplay_on_socket_error();
        return 0;
    }
    return 1;
}

int fs_emu_netplay_send_input_event(int input_event) {
    if (input_event & 0xff000000) {
        fs_log("WARNING: input event with high 8 bits set: %d\n",
                input_event);
        return 1;
    }
    if (!fs_emu_netplay_enabled()) {
        return 0;
    }
    if (!fs_emu_netplay_connected()) {
        return 0;
    }
    //printf("sending event %d\n", input_event);
    uint32_t message = MESSAGE_INPUT_MASK | input_event;
    send_message(message);
    return 1;
}

int fs_emu_netplay_wait_for_frame(int frame) {

    //printf("fs_emu_netplay_wait_for_frame %d\n", frame);
    /*
    while (1) {
        fs_ml_usleep(100 * 1000);
        if (fs_emu_is_quitting()) {
            fs_log("fs_emu_netplay_wait_for_frame: quitting\n");
            return 0;
        }
    }
    */

    if (!g_fs_emu_throttling) {
        static int warned = 0;
        if (!warned) {
            fs_emu_warning("Netplay is not compatible with throttling "
                    "disabled");
            warned = 1;
        }
    }

    fs_mutex_lock(g_wait_for_frame_mutex);
    while (g_frame < frame) {
        //fs_time_val abs_time;
        // wait max 100 ms for a new frame, to allow the loop to end if the
        // emu is quitting

        /*
        g_get_current_time(&abs_time);
        g_time_val_add(&abs_time, 100 * 1000);
        g_cond_timed_wait(g_wait_for_frame_cond, g_wait_for_frame_mutex,
                &abs_time);
        */

        //fs_get_current_time(&abs_time);
        //fs_time_val_add(&abs_time, 100 * 1000);
        // int64_t abs_time = fs_get_real_time() + 100 * 1000;
        int64_t end_time = fs_condition_get_wait_end_time(100 * 1000);

        // FIXME: check for spurious wakeup
        fs_condition_wait_until(
            g_wait_for_frame_cond, g_wait_for_frame_mutex, end_time);

        if (fs_emu_is_quitting()) {
            fs_log("fs_emu_netplay_wait_for_frame: quitting\n");
            fs_mutex_unlock(g_wait_for_frame_mutex);
            return 0;
        }
        if (!fs_emu_netplay_enabled()) {
            // no longer in net play mode
            fs_mutex_unlock(g_wait_for_frame_mutex);
            return 0;
        }
        if (g_waiting_dialog) {
            fs_emu_acquire_gui_lock();
            if (g_waiting_dialog) {
                if (fs_emu_dialog_result(g_waiting_dialog) ==
                        DIALOG_RESULT_NEGATIVE) {
                    fs_emu_netplay_disconnect();
                }
            }
            fs_emu_release_gui_lock();
        }
    }
    fs_mutex_unlock(g_wait_for_frame_mutex);

    if (frame == 1) {
        dismiss_waiting_dialog();
    }

    send_message(MESSAGE_RNDCHECK | (g_rand_checksum_function() &
            0x00ffffff));
    send_message(MESSAGE_MEMCHECK | (g_state_checksum_function() &
            0x00ffffff));
    send_message(MESSAGE_FRAME_MASK | frame);

    // add all pending events for this frame to libfsemu's input queue
    int input_event;
    while ((input_event = fs_emu_get_netplay_input_event()) != 0) {
        //printf("\n\n---> %08x\n", input_event);
        if (input_event & 0x80000000) {
            int sentinel_frame = input_event & 0x7fffffff;
            if (frame != sentinel_frame) {
                // should not happen..
                fs_log("ERROR: synchronization error ("
                        "frame %d != sentinel %d)\n", frame, sentinel_frame);
                exit(1);
            }
            break;
        }
        else {
            //printf("queue input event %d\n", input_event);
            fs_emu_queue_input_event_internal(input_event);
        }
    }

    return 1;
}

//#define EXTRACT_BITS(m, a, b) ((m >> a) & ((1 << (b - a + 1)) - 1))
//#define FILTER_BITS(m, a, b) (((m >> a) << a) & ((1 << (b + 1)) - 1))

static void handle_player_tag_message(int ply, int data) {
    fs_emu_player *p = g_fs_emu_players + ply;
    p->tag[0] = (data & 0x00ff0000) >> 16;
    p->tag[1] = (data & 0x0000ff00) >> 8;
    p->tag[2] = (data & 0x000000ff);
    p->tag[3] = '\0';
    fs_emu_log("received player tag for player %d: \"%s\"\n", ply, p->tag);
}

static char g_text_buffer[FSE_MAX_CHAT_STRING_LENGTH + 1];

static void process_text_message(const char *text, int from_player) {
    //printf("process message from player %d -- self = %d\n",
    //        from_player, g_fs_emu_netplay_player);
    if (from_player == g_fs_emu_netplay_player) {
        // message from self --already printed to console
        return;
    }
    /*
    char *line = fs_strdup_printf("<%s> %s",
            fs_emu_get_netplay_tag(from_player), text);
    fs_emu_add_console_line(line, 0);
    g_free(line);
    */

    fs_emu_hud_add_chat_message(text, fs_emu_get_netplay_tag(from_player));
}

static void handle_ext_message(int message, int data) {
    if (message == MESSAGE_PING) {
        //printf("ping at frame %d\n", g_frame);
        send_message(CREATE_EXT_MESSAGE(MESSAGE_PING, 0));
    }
    else if (message == MESSAGE_PLAYERS) {
        g_fs_emu_netplay_player = (data >> 8) & 0xff;
        g_fs_emu_netplay_num_players = data & 0xff;
    }
    else if (message == MESSAGE_PLAYER_TAG_0) {
        handle_player_tag_message(0, data);
    }
    else if (message == MESSAGE_PLAYER_TAG_1) {
        handle_player_tag_message(1, data);
    }
    else if (message == MESSAGE_PLAYER_TAG_2) {
        handle_player_tag_message(2, data);
    }
    else if (message == MESSAGE_PLAYER_TAG_3) {
        handle_player_tag_message(3, data);
    }
    else if (message == MESSAGE_PLAYER_TAG_4) {
        handle_player_tag_message(4, data);
    }
    else if (message == MESSAGE_PLAYER_TAG_5) {
        handle_player_tag_message(5, data);
    }
    else if (message == MESSAGE_PLAYER_PING) {
        int ply = (data & 0x00ff0000) >> 16;
        if (ply < MAX_PLAYERS) {
            g_fs_emu_players[ply].ping = data & 0x0000ffff;
            //printf("received player ping %d %d\n", ply, g_fs_emu_players[ply].ping);
        }
    }
    else if (message == MESSAGE_PLAYER_LAG) {
        int ply = (data & 0x00ff0000) >> 16;
        if (ply < MAX_PLAYERS) {
            g_fs_emu_players[ply].lag = data & 0x0000ffff;
            //printf("received player lag %d %d\n", ply, g_fs_emu_players[ply].ping);
        }
    }
    else if (message == MESSAGE_ERROR) {
        fs_emu_warning("ERROR %d from netplay server", data);
        fs_emu_netplay_disconnect();
    }
    else if (message == MESSAGE_TEXT) {
        int from_player = (data & 0xff0000) >> 16;
        int text_len = data & 0xffff;
        fs_log("received text message (len %d) from player %d\n", text_len,
                from_player);
        int remaining = text_len;
        if (remaining > FSE_MAX_CHAT_STRING_LENGTH) {
            remaining = FSE_MAX_CHAT_STRING_LENGTH;
        }
        char *p = g_text_buffer;
        while (1) {
            //printf("--> %d <--\n", remaining);
            int bytes_read = recv(g_socket, p, remaining, 0);
            if (bytes_read <= 0) {
                fs_log("ERROR: recv returned %d\n", bytes_read);
                fs_emu_netplay_on_socket_error();
                return;
            }
            remaining -= bytes_read;
            text_len -= bytes_read;
            p += bytes_read;
            if (remaining == 0) {
                break;
            }
        }
        *p = '\0';
        process_text_message(g_text_buffer, from_player);
        // in case more than max chars of text was sent, read and ignore
        // the rest of the data
        while (text_len) {
            char buffer;
            int bytes_read = recv(g_socket, &buffer, 1, 0);
            if (bytes_read <= 0) {
                fs_log("ERROR: recv returned %d\n", bytes_read);
                fs_emu_netplay_on_socket_error();
                return;
            }
            text_len -= bytes_read;
        }
    }
    else if (message == MESSAGE_SESSION_KEY) {
        g_fs_emu_netplay_session_key = data;
        fs_log("received session key: %d\n", g_fs_emu_netplay_session_key);
    }
    else {
        fs_emu_warning("net play: received unknown (ext) message %d\n",
                message);
        fs_emu_netplay_disconnect();
    }
}

static void handle_message(uint32_t message) {
    //printf("----> received message %08x\n", message);
    int message_type = message & 0xff000000;

    if (message_type & MESSAGE_EXT_MASK) {
        handle_ext_message((message_type & 0x7f000000) >> 24,
                message & 0x00ffffff);
    }
    else if (message_type & MESSAGE_FRAME_MASK) {
        int frame = message & 0x3fffffff;
        //printf("received frame %d\n", frame);

        // add frame to netplay input queue as a sentinel between groups
        // of input events to achieve synchronization between frames
        // events. High bit set indicates this sentinel.

        fs_emu_queue_netplay_input_event(0x80000000 | frame);
        //printf("queueing sentinel %x\n", 0x80000000 | frame);

        fs_mutex_lock(g_wait_for_frame_mutex);
        g_frame = frame;
        fs_condition_signal(g_wait_for_frame_cond);
        fs_mutex_unlock(g_wait_for_frame_mutex);
    }
    else if (message_type & MESSAGE_INPUT_MASK) {
        // received input event
        int input_event = message & 0x00ffffff;
        //printf("received event %d\n", input_event);
        //fs_emu_queue_input_event_internal(input_event);
        fs_emu_queue_netplay_input_event(input_event);
    }
}

static void *receive_thread(void * data) {
    static unsigned char buffer[5] = {};
    int count = 0;
    while (1) {
        // FIXME: add shutdown condition

        int bytes_read = recv(g_socket, (char *) buffer + count, 4 - count, 0);
        if (bytes_read <= 0) {
            fs_log("ERROR: recv returned %d\n", bytes_read);
            // FIXME: handle problem better
            //return NULL;
            //exit(1);
            fs_emu_netplay_on_socket_error();
            return NULL;
        }
        count += bytes_read;
        if (bytes_read == 4) {
            count = 0;
            uint32_t message = bytes_to_uint(buffer);
            handle_message(message);
        }

    }
    return NULL;
}

int fs_emu_netplay_connect() {
    g_socket = 0;

    fs_log("look up address for %s...\n", g_fs_emu_netplay_server);

    struct addrinfo hints;
    struct addrinfo *result, *rp;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    // Allow IPv4 or IPv6
#ifdef TCP
    hints.ai_socktype = SOCK_STREAM;
#else
    hints.ai_socktype = SOCK_DGRAM;
#endif
    hints.ai_flags = 0;
    hints.ai_protocol = 0;
    int s = getaddrinfo(g_fs_emu_netplay_server, g_fs_emu_netplay_port,
            &hints, &result);
    if (s != 0) {
         fs_emu_log("getaddrinfo: %s\n", gai_strerror(s));
         // FIXME:
         //exit(1);
         return 0;
     }

    /* getaddrinfo() returns a list of address structures.
        Try each address until we successfully connect(2).
        If socket(2) (or connect(2)) fails, we (close the socket
        and) try the next address. */

    for (rp = result; rp != NULL; rp = rp->ai_next) {
        // FIXME
        //printf("---->\n");

        g_socket = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (g_socket == -1) {
            continue;
        }
#ifdef TCP
        if (connect(g_socket, rp->ai_addr, rp->ai_addrlen) != -1) {
            // connected
            break;
        }
        close(g_socket);
        g_socket = 0;
#else
        g_server_address = *rp->ai_addr;
        g_addrlen = sizeof(struct sockaddr);
#endif
    }

    freeaddrinfo(result);

    if (g_socket <= 0) {
        fs_log("ERROR: could not connect to server\n");
        return 0;
    }

#ifdef TCP
    int flag = 1;
    int r = setsockopt(g_socket, IPPROTO_TCP, TCP_NODELAY, (char *) &flag,
            sizeof(int));
    if (r < 0) {
        //fs_log("ERROR: could not set TCP_NODELAY\n");
        fs_emu_warning("ERROR: Could not set TCP_NODELAY option");
        close(g_socket);
        g_socket = 0;
        return 0;
    }
#endif

    /*
     0 -  4  FSNP
     4 -  5  PROTOCOL
     5 -  9  PASSWORD
     9 - 17  EMULATOR VERSION
    17 - 20  SESSION KEY
    20 - 21  PLAYER NUMBER
    21 - 24  TAG
    24 - 28  RESUME AT PACKET
   */

    unsigned char data[28] = "FSNP     FSUAE              ";
    data[0] = 'F';
    data[1] = 'S';
    data[2] = 'N';
    data[3] = 'P';

    // protocol version
    data[4] = FS_EMU_NETPLAY_PROTOCOL_VERSION;
    //uint_to_bytes(g_fs_emu_netplay_pass_checksum, data + 5);
    data[5] = g_fs_emu_netplay_password[0];
    data[6] = g_fs_emu_netplay_password[1];
    data[7] = g_fs_emu_netplay_password[2];
    data[8] = g_fs_emu_netplay_password[3];

    uint_to_bytes(g_fs_emu_netplay_session_key, data + 16);

    // overwrite with protocol version (overwriting 1 byte of session key
    // on purpose, because session key is 24 bit)
    data[14] = g_fs_emu_netplay_emulation_version_major;
    data[15] = g_fs_emu_netplay_emulation_version_minor;
    data[16] = g_fs_emu_netplay_emulation_version_revision;

    data[20] = g_fs_emu_netplay_player;
    data[21] = g_fs_emu_netplay_tag[0];
    data[22] = g_fs_emu_netplay_tag[1];
    data[23] = g_fs_emu_netplay_tag[2];
    uint_to_bytes(g_fs_emu_netplay_resume_at_packet, data + 24);

    send_bytes(data, 28);
    return 1;
}

static void *netplay_thread(void * data) {
    fs_log("netplay_thread started\n");
    int connected = 0;
    int retry_secs = 2;

    fs_emu_acquire_gui_lock();
    fs_emu_dialog* dialog = fs_emu_dialog_create(
            "Connecting To Net Play Server", NULL, "Abort");
    //fs_emu_dialog_add_option(dialog, "Abort", 1);
    char *line = g_strdup_printf("%s:%s", g_fs_emu_netplay_server,
            g_fs_emu_netplay_port);
    fs_emu_dialog_set_line(dialog, 0, line);
    free(line);
    fs_emu_dialog_show(dialog);
    fs_emu_release_gui_lock();

    int connection_attempt = 1;
    while (1) {
        if (connection_attempt > 1) {
            fs_emu_acquire_gui_lock();
            char *line = g_strdup_printf("Connection attempt %d",
                    connection_attempt);
            fs_emu_dialog_set_line(dialog, 2, line);
            free(line);
            fs_emu_release_gui_lock();
        }
        connected = fs_emu_netplay_connect();
        if (connected) {
            break;
        }
        fs_emu_warning("Error connecting to server, "
                "retrying in %d seconds", retry_secs);
        for (int i = 0; i < retry_secs * 10; i++) {
            if (fs_emu_is_quitting()) {
                fs_log("netplay_thread: quitting\n");
                return NULL;
            }
            if (fs_emu_dialog_result(dialog) == DIALOG_RESULT_NEGATIVE) {
                break;
            }
            fs_ml_usleep(100 * 1000);
        }
        if (fs_emu_dialog_result(dialog) == DIALOG_RESULT_NEGATIVE) {
            break;
        }
        if (retry_secs < 10) {
            retry_secs++;
        }
        connection_attempt++;
    }

    fs_emu_acquire_gui_lock();
    fs_emu_dialog_dismiss(dialog);
    fs_emu_dialog_destroy(dialog);
    fs_emu_release_gui_lock();

    if (connected) {
        g_fs_emu_netplay_connected = 1;
        show_waiting_dialog();
    }
    else {
        // the user aborted netplay connection
        fs_emu_warning("Continuing game without net play\n");
        free(g_fs_emu_netplay_server);
        g_fs_emu_netplay_server = NULL;
    }

    // FIXME: use non-joinable thread?
    g_receive_thread = fs_thread_create(
                "netplay-receive", receive_thread, NULL);
    if (g_receive_thread == NULL) {
        fs_log("ERROR: could not create receive thread\n");
    }

    return NULL;
}

void fs_emu_netplay_start() {
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

    // FIXME: use non-joinable thread?
    g_netplay_thread = fs_thread_create("netplay", netplay_thread, NULL);
    if (g_netplay_thread == NULL) {
        fs_emu_warning("ERROR: could not create netplay thread\n");
    }
}

#endif

