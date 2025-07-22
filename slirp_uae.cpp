
#include "sysconfig.h"
#ifdef _WIN32
#include "Winsock2.h"
#endif
#include "sysdeps.h"

#ifdef WITH_SLIRP

#include "uae/likely.h"
#include "threaddep/thread.h"

// 1 ERROR
// 2 WARN
// 3 INFO
// 4 DEBUG
// 5 TRACE

#define UAE_LOG_INFO 3
#define UAE_LOG_DEBUG 4

static int uae_slirp_log_level = UAE_LOG_INFO;

// #define uae_slirp_log(format, ...)             \
//     uae_log("[SLIRP] " format, ##__VA_ARGS__);

#define uae_slirp_log(format, ...)           \
    if (uae_likely(uae_slirp_log_level >= UAE_LOG_INFO)) { \
        uae_log(_T("[SLIRP] ") format, ##__VA_ARGS__);           \
    }

#define uae_slirp_log_debug(format, ...)           \
    if (uae_unlikely(uae_slirp_log_level >= UAE_LOG_DEBUG)) { \
        uae_log(_T("[SLIRP] ") format, ##__VA_ARGS__);              \
    }

#include "options.h"
#include "uae/slirp.h"
#include "uae.h"

#define WITH_NEW_SLIRP

#ifdef WITH_NEW_SLIRP

// FIXME: Add redir functionality!

#include "uae/time.h"

#include "slirp2/src/libslirp.h"

void uae_slirp_output(const uint8_t *pkt, int pkt_len);

static Slirp *slirp;

// Timer and slirp initialization code adapted from libvdeslirp
// Copyright (C) 2019 Renzo Davoli VirtualSquare, LGPL 2.1

struct uae_slirp_timer {
	struct uae_slirp_timer *next;
	uint64_t expire_time;
	SlirpTimerCb handler;
	void *opaque;
};

static struct uae_slirp_timer *timer_head;

static ssize_t uae_slirp_send_packet(const void *pkt, size_t pkt_len,
                                     void *opaque)
{
    // SlirpState *s = opaque;
	uae_slirp_output((const uint8_t *) pkt, pkt_len);
	// FIXME: Is the return value important here?
	return pkt_len;
    // return qemu_send_packet(&s->nc, pkt, pkt_len);
}

static void uae_slirp_guest_error(const char *msg, void *opaque)
{
	(void) opaque;
	fprintf(stderr, "Slirp guest error: %s\n", msg);
	// FIXME:
	// uae_error(msg);
}

static int64_t uae_slirp_clock_get_ns(void *opaque)
{
	// Don't think we need nanosecond precision here.
	return uae_time_us() * 1000;
}

static void *uae_slirp_timer_new(SlirpTimerCb cb,
                                 void *cb_opaque, void *opaque)
{
	struct uae_slirp_timer *qt = (struct uae_slirp_timer *) malloc(sizeof(*qt));
	if (qt) {
		qt->next = timer_head;
		qt->expire_time = -1;
		qt->handler = cb;
		qt->opaque = cb_opaque;
		timer_head = qt;
	}
	return qt;
}

static void uae_slirp_timer_free(void *timer, void *opaque)
{
	struct uae_slirp_timer *qt = (struct uae_slirp_timer *) timer;
	struct uae_slirp_timer **scan;
	for (scan = &timer_head; *scan != NULL && *scan != qt; scan = &((*scan) ->next))
		;
	if (*scan) {
		*scan = qt->next;
		free(qt);
	}
}

static void uae_slirp_timer_mod(void *timer, int64_t expire_time,
                                void *opaque)
{
	(void) opaque;
	struct uae_slirp_timer *qt = (struct uae_slirp_timer *) timer;
	qt->expire_time = expire_time;
}

static void uae_slirp_register_poll_fd(int fd, void *opaque)
{
	(void) fd;
	(void) opaque;
}

static void uae_slirp_unregister_poll_fd(int fd, void *opaque)
{
	(void) fd;
	(void) opaque;
}

static void uae_slirp_notify(void *opaque)
{
	(void) opaque;
}

static const SlirpCb uae_slirp_callbacks = {
    .send_packet = uae_slirp_send_packet,
    .guest_error = uae_slirp_guest_error,
    .clock_get_ns = uae_slirp_clock_get_ns,
    .timer_new = uae_slirp_timer_new,
    .timer_free = uae_slirp_timer_free,
    .timer_mod = uae_slirp_timer_mod,
    .register_poll_fd = uae_slirp_register_poll_fd,
    .unregister_poll_fd = uae_slirp_unregister_poll_fd,
    .notify = uae_slirp_notify,
};

// #include <stdlib.h>
// #include <unistd.h>

#ifdef _WIN32
// Winsock2.h is included at the top
#define poll WSAPoll
// For inet_pton. Requires Windows Vista or later.
#include <ws2tcpip.h>
#else
#include <poll.h>
#endif
// #include <time.h>
// #include <errno.h>
// #include <sys/socket.h>
// #include <sys/un.h>

static int pfd_len;
static int pfd_size;
#ifdef _WIN32
static WSAPOLLFD *pfd;
#else
static struct pollfd *pfd;
#endif

#define UAE_SLIRP_POLLFD_SIZE_INCREASE 16

static int uae_slirp_slirp_to_poll(int events)
{
	int ret = 0;
	if (events & SLIRP_POLL_IN) ret |= POLLIN;
	if (events & SLIRP_POLL_OUT) ret |= POLLOUT;
#ifdef _WIN32
	// POLLPRI is not supported by Winsock.
	// POLLHUP is not supported for WSAPoll? Actually, it is defined
	// as output only (revents) and is ignored for events.
	// Also ignore POLLERR (Not supported?)
#else
	if (events & SLIRP_POLL_PRI) ret |= POLLPRI;
	if (events & SLIRP_POLL_ERR) ret |= POLLERR;
	if (events & SLIRP_POLL_HUP) ret |= POLLHUP;
#endif
	return ret;
}

static int uae_slirp_poll_to_slirp(int events)
{
	int ret = 0;
	if (events & POLLIN) ret |= SLIRP_POLL_IN;
	if (events & POLLOUT) ret |= SLIRP_POLL_OUT;
	if (events & POLLPRI) ret |= SLIRP_POLL_PRI;
	if (events & POLLERR) ret |= SLIRP_POLL_ERR;
	if (events & POLLHUP) ret |= SLIRP_POLL_HUP;
	return ret;
}

static int uae_slirp_add_poll(int fd, int events, void *opaque)
{
	// printf("uae_slirp_add_poll fd=%d events=%d\n", fd, events);
	if (pfd_len >= pfd_size) {
		int newsize = pfd_size + UAE_SLIRP_POLLFD_SIZE_INCREASE;
		struct pollfd *new_ = (struct pollfd *) realloc(pfd, newsize * sizeof(struct pollfd));
		if (new_) {
			pfd = new_;
			pfd_size = newsize;
		}
		// printf("newsize %d\n", newsize);
	}
	if (pfd_len < pfd_size) {
		int idx = pfd_len++;
		pfd[idx].fd = fd;
		pfd[idx].events = uae_slirp_slirp_to_poll(events);
		// printf("-> idx %d\n", idx);
		return idx;
	} else {
		// printf("return -1\n");
		return -1;
	}
}

static int uae_slirp_get_revents(int idx, void *opaque)
{
	// printf("uae_slirp_get_revents idx=%d\n", idx);
	return uae_slirp_poll_to_slirp(pfd[idx].revents);
}

static void uae_slirp_update_ra_timeout(uint32_t *timeout, void *opaque)
{
	struct uae_slirp_timer *qt;
	int64_t now_ms = uae_slirp_clock_get_ns(opaque) / 1000000;
	for (qt = timer_head; qt != NULL; qt = qt->next) {
		if (qt->expire_time != -1UL) {
			int64_t diff = qt->expire_time - now_ms;
			if (diff < 0) diff = 0;
			if (diff < *timeout) *timeout = diff;
		}
	}
}

static void uae_slirp_check_ra_timeout(void *opaque)
{
	struct uae_slirp_timer *qt;
	int64_t now_ms = uae_slirp_clock_get_ns(opaque) / 1000000;
	for (qt = timer_head; qt != NULL; qt =  qt->next) {
		if (qt->expire_time != -1UL) {
			int64_t diff = qt->expire_time - now_ms;
			if (diff <= 0) {
				qt->expire_time = -1UL;
				qt->handler(qt->opaque);
			}
		}
	}
}

static volatile int slirp_thread_active;
static uae_thread_id slirp_tid;
extern uae_sem_t slirp_sem2;

static void slirp_receive_func(void *arg)
{
	uae_slirp_log("Slirp polling thread started\n");
	slirp_thread_active = 1;
	while (slirp_thread_active) {
		uint32_t timeout = -1;
		int pollout = 0;
		// Reset file descriptor array. It will be filled again by callbacks
		// when calling slirp_pollfds_fill.
		pfd_len = 0;

		uae_sem_wait (&slirp_sem2);
		slirp_pollfds_fill(slirp, &timeout, uae_slirp_add_poll, NULL);
		uae_slirp_update_ra_timeout(&timeout, slirp);
		uae_sem_post (&slirp_sem2);

		if (pfd_len > 0) {
			pollout = poll(pfd, pfd_len, timeout);
#ifdef _WIN32
			if (pollout == -1) {
				printf("WSA poll error %d\n", WSAGetLastError());
			}
#endif
		} else {
			sleep_millis(timeout);
		}

		uae_sem_wait (&slirp_sem2);
		slirp_pollfds_poll(slirp, (pollout <= 0), uae_slirp_get_revents, NULL);
		uae_slirp_check_ra_timeout(slirp);
		uae_sem_post (&slirp_sem2);
	}
	slirp_thread_active = -1;
	uae_slirp_log("Slirp polling thread ended\n");
	return;
}

#else

#ifdef WITH_BUILTIN_SLIRP
#include "slirp/slirp.h"
#include "slirp/libslirp.h"
#include "threaddep/thread.h"
#endif

#ifdef WITH_QEMU_SLIRP
#include "uae/dlopen.h"
#include "uae/ppc.h"
#include "uae/qemu.h"
#endif

#endif

/* Implementation enumeration must correspond to slirp_implementations in
 * cfgfile.cpp. */
enum Implementation {
	AUTO_IMPLEMENTATION = 0,
	NO_IMPLEMENTATION,
	BUILTIN_IMPLEMENTATION,
	QEMU_IMPLEMENTATION,
};

static Implementation impl;

static Implementation check_conf(Implementation check)
{
	int conf = AUTO_IMPLEMENTATION;
	if (conf == AUTO_IMPLEMENTATION || conf == check) {
		return check;
	}
	return AUTO_IMPLEMENTATION;
}

int uae_slirp_init(void)
{
#ifdef WITH_NEW_SLIRP
	if (slirp) {
		return 0;
	}
//return -1;
#if 0
	// if (impl == BUILTIN_IMPLEMENTATION) {

		int restricted = 0;
		const char *vhostname = NULL;
		const char *tftp_server_name = NULL;
		const char *tftp_export = NULL;
		const char *bootfile = "";
		const char **dnssearch = NULL;
		const char *vdomainname = NULL;

		bool ipv4 = true;
		bool ipv6 = false;

		struct in_addr net  = { .s_addr = htonl(0x0a000200) }; /* 10.0.2.0 */
		struct in_addr mask = { .s_addr = htonl(0xffffff00) }; /* 255.255.255.0 */
		struct in_addr host = { .s_addr = htonl(0x0a000202) }; /* 10.0.2.2 */
		struct in_addr dhcp = { .s_addr = htonl(0x0a00020f) }; /* 10.0.2.15 */
		struct in_addr dns  = { .s_addr = htonl(0x0a000203) }; /* 10.0.2.3 */

		struct in6_addr ip6_prefix;
		struct in6_addr ip6_host;
		struct in6_addr ip6_dns;

		int vprefix6_len = 64;
	// #ifndef _WIN32
	// 	struct in_addr smbsrv = { .s_addr = 0 };
	// #endif
		// NetClientState *nc;
		// SlirpState *s;
		// char buf[20];
		// uint32_t addr;
		// int shift;
		// char *end;
		// struct slirp_config_str *config;
#endif
#if 0
    //Slirp *
    slirp = slirp_init(
		restricted,
		ipv4,
		net,
		mask,
		host,
		ipv6,
		ip6_prefix,
		vprefix6_len,
		ip6_host,
		vhostname,
		tftp_server_name,
		tftp_export,
		bootfile,
		dhcp,
		dns,
		ip6_dns,
		dnssearch,
		vdomainname,
		&slirp_cb,
		NULL
	);
#endif

	SlirpConfig cfg_storage;

	SlirpConfig *cfg = &cfg_storage;
	memset(cfg, 0, sizeof(*cfg));
	cfg->version = 1;

	cfg->restricted = 0;
	cfg->in_enabled = 1;
	inet_pton(AF_INET,"10.0.2.0", &(cfg->vnetwork));
	inet_pton(AF_INET,"255.255.255.0", &(cfg->vnetmask));
	inet_pton(AF_INET,"10.0.2.2", &(cfg->vhost));
	cfg->in6_enabled = 1;
	inet_pton(AF_INET6, "fd00::", &cfg->vprefix_addr6);
	cfg->vprefix_len = 64;
	inet_pton(AF_INET6, "fd00::2", &cfg->vhost6);
	cfg->vhostname = "slirp";
	cfg->tftp_server_name = NULL;
	cfg->tftp_path = NULL;
	cfg->bootfile = NULL;
	inet_pton(AF_INET,"10.0.2.15", &(cfg->vdhcp_start));
	inet_pton(AF_INET,"10.0.2.3", &(cfg->vnameserver));
	inet_pton(AF_INET6, "fd00::3", &cfg->vnameserver6);
	cfg->vdnssearch = NULL;
	cfg->vdomainname = NULL;
	cfg->if_mtu = 0; // IF_MTU_DEFAULT
	cfg->if_mru = 0; // IF_MTU_DEFAULT

	cfg->disable_host_loopback = 0;

	slirp = slirp_new(cfg, &uae_slirp_callbacks, NULL);
	uae_slirp_log("Slirp context created: %p\n", slirp);
	// FIXME?
	return 0;
// }
#else
#if defined(WITH_QEMU_SLIRP)
	if (impl == AUTO_IMPLEMENTATION) {
		impl = check_conf(QEMU_IMPLEMENTATION);
	}
#endif
#if defined(WITH_BUILTIN_SLIRP)
	if (impl == AUTO_IMPLEMENTATION) {
		impl = check_conf(BUILTIN_IMPLEMENTATION);
	}
#endif
	if (impl == AUTO_IMPLEMENTATION) {
		impl = NO_IMPLEMENTATION;
	}

#ifdef WITH_QEMU_SLIRP
	if (impl == QEMU_IMPLEMENTATION) {
		return uae_qemu_uae_init() == NULL;
	}
#endif
#ifdef WITH_BUILTIN_SLIRP
	if (impl == BUILTIN_IMPLEMENTATION) {
		return slirp_init();
	}
#endif
	return -1;
#endif
}

void uae_slirp_cleanup(void)
{
#ifdef WITH_NEW_SLIRP
	slirp_cleanup(slirp);
	slirp = NULL;
#else
#ifdef WITH_QEMU_SLIRP
	if (impl == QEMU_IMPLEMENTATION) {
		UAE_LOG_STUB("");
		return;
	}
#endif
#ifdef WITH_BUILTIN_SLIRP
	if (impl == BUILTIN_IMPLEMENTATION) {
		slirp_cleanup();
		return;
	}
#endif
#endif
}

void uae_slirp_input(const uint8_t *pkt, int pkt_len)
{
#ifdef WITH_NEW_SLIRP
	uae_slirp_log_debug(_T("uae_slirp_input pkt_len %d\n"), pkt_len);
	slirp_input(slirp, pkt, pkt_len);
#else
#ifdef WITH_QEMU_SLIRP
	if (impl == QEMU_IMPLEMENTATION) {
		if (qemu_uae_slirp_input) {
			qemu_uae_slirp_input(pkt, pkt_len);
		}
		return;
	}
#endif
#ifdef WITH_BUILTIN_SLIRP
	if (impl == BUILTIN_IMPLEMENTATION) {
		slirp_input(pkt, pkt_len);
		return;
	}
#endif
#endif
}

void uae_slirp_output(const uint8_t *pkt, int pkt_len)
{
#if 0
	write_log(_T("uae_slirp_output pkt_len %d\n"), pkt_len);
#endif
	slirp_output(pkt, pkt_len);
}

int uae_slirp_redir(int is_udp, int host_port, struct in_addr guest_addr,
		    int guest_port)
{
#ifdef WITH_NEW_SLIRP
#else
#ifdef WITH_QEMU_SLIRP
	if (impl == QEMU_IMPLEMENTATION) {
		UAE_LOG_STUB("");
		return 0;
	}
#endif
#ifdef WITH_BUILTIN_SLIRP
	if (impl == BUILTIN_IMPLEMENTATION) {
		return slirp_redir(is_udp, host_port, guest_addr, guest_port);
	}
#endif
#endif
	return 0;
}

#ifdef WITH_NEW_SLIRP
#else
#ifdef WITH_BUILTIN_SLIRP

static volatile int slirp_thread_active;
static uae_thread_id slirp_tid;
extern uae_sem_t slirp_sem2;

static void slirp_receive_func(void *arg)
{
	slirp_thread_active = 1;
	while (slirp_thread_active == 1) {
		fd_set rfds, wfds, xfds;
		INT_PTR nfds;
		int ret, timeout;

		nfds = -1;
		FD_ZERO(&rfds);
		FD_ZERO(&wfds);
		FD_ZERO(&xfds);

		// Add timeout protection for semaphore acquisition
		if (uae_sem_trywait_delay(&slirp_sem2, 500)) {
			// Couldn't acquire semaphore within timeout
			continue;
		}

		timeout = slirp_select_fill(&nfds, &rfds, &wfds, &xfds);
		uae_sem_post(&slirp_sem2);
		if (nfds < 0) {
			/* Windows does not honour the timeout if there is not
			   descriptor to wait for */
			sleep_millis(timeout / 1000);
			ret = 0;
		} else {
			struct timeval tv;
			tv.tv_sec = 0;
			tv.tv_usec = timeout;
			ret = select(nfds + 1, &rfds, &wfds, &xfds, &tv);
			if (ret == SOCKET_ERROR) {
				write_log(_T("SLIRP socket ERR=%d\n"), WSAGetLastError());
				sleep_millis(10);
				continue;
			}
		}
		if (ret >= 0) {
			if (uae_sem_trywait_delay(&slirp_sem2, 500)) {
				continue;
			}
			slirp_select_poll(&rfds, &wfds, &xfds);
			uae_sem_post(&slirp_sem2);
		}
	}
	slirp_thread_active = -1;
}

int slirp_can_output(void)
{
	return 1;
}

#endif
#endif

bool uae_slirp_start (void)
{
#ifdef WITH_NEW_SLIRP
	uae_slirp_end();
	uae_slirp_log(_T("Slirp start\n"));
	uae_start_thread(_T("slirp-receive"), slirp_receive_func, NULL,
						&slirp_tid);
	return true;
#else
#ifdef WITH_QEMU_SLIRP
	if (impl == QEMU_IMPLEMENTATION) {
		UAE_LOG_STUB("");
		return true;
	}
#endif
#ifdef WITH_BUILTIN_SLIRP
	if (impl == BUILTIN_IMPLEMENTATION) {
		uae_slirp_end ();
		uae_start_thread(_T("slirp-receive"), slirp_receive_func, NULL,
						 &slirp_tid);
		return true;
	}
#endif
	return false;
#endif
}

void uae_slirp_end(void)
{
	uae_slirp_log(_T("Slirp end\n"));
#ifdef WITH_NEW_SLIRP
	if (slirp_thread_active > 0) {
		slirp_thread_active = 0;
		while (slirp_thread_active == 0) {
			sleep_millis (10);
		}
		uae_end_thread (&slirp_tid);
	}
	slirp_thread_active = 0;
	return;
#else
#ifdef WITH_QEMU_SLIRP
	if (impl == QEMU_IMPLEMENTATION) {
		UAE_LOG_STUB("");
		return;
	}
#endif
#ifdef WITH_BUILTIN_SLIRP
	if (impl == BUILTIN_IMPLEMENTATION) {
		if (slirp_thread_active > 0) {
			slirp_thread_active = 0;
			// Use a proper timeout instead of infinite waiting
			int wait_count = 0;
			while (slirp_thread_active == 0 && wait_count < 100) {
				sleep_millis(10);
				wait_count++;
			}

			// Force thread termination if it didn't exit cleanly
			if (slirp_thread_active == 0) {
				write_log(_T("SLIRP thread did not terminate properly, forcing exit\n"));
			}
			uae_end_thread(&slirp_tid);
		}
		slirp_thread_active = 0;
		return;
	}
#endif
#endif
}

#endif /* WITH_SLIRP */
