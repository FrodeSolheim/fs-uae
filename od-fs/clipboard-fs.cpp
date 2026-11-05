/*
 * FS-UAE host side of the Amiga clipboard (text only).
 *
 * The generic clipboard.device handshake and IFF encoding/decoding is shared
 * with the other ports (od-generic/clipboard.cpp). This file is the FS-UAE
 * host adapter, comparable to od-unix/clipboard.cpp, but instead of touching
 * the host clipboard directly (which must be done from the main thread), it
 * exchanges text with the host/GUI side through a mutex protected mailbox:
 *
 *  - uae_clipboard_put_text()  host -> Amiga (called from the main thread)
 *  - uae_clipboard_get_text()  Amiga -> host (called from the main thread)
 *
 * The Amiga side (amiga_clipboard_*, clipboard_vsync, ...) runs on the UAE
 * thread.
 */

#include "sysconfig.h"
#include "sysdeps.h"

#include <mutex>
#include <string>
#include <vector>

#include "traps.h"
#include "clipboard.h"
#include "clipboard_common.h"
#include "keybuf.h"
#include "native2amiga_api.h"
#include "uae.h"
#include "uae/uae.h"

extern bool filesys_heartbeat(void);

static clipboard_transfer transfer;
static bool clip_disabled;
// True when the first host to Amiga transfer after amiga_clipboard_init
// should use the smaller initial size limit (like the other ports do).
static bool initial_transfer;

static std::mutex host_mutex;
static bool host_ready;
// Last text received from the host, kept so that it can be pasted to the
// Amiga keyboard buffer
static std::string from_host_text;
// True when from_host_text has not yet been announced to the Amiga
static bool from_host_changed;
// Text from the Amiga waiting to be picked up by the host
static std::string to_host_text;
static bool to_host_pending;

void clipboard_init(void)
{
	std::lock_guard<std::mutex> lock(host_mutex);
	write_log("clipboard_init\n");
	host_ready = true;
}

UAE_EXTERN_C
char *uae_clipboard_get_text(void)
{
	std::lock_guard<std::mutex> lock(host_mutex);
	if (!to_host_pending) {
		return NULL;
	}
	to_host_pending = false;
	char *text = strdup(to_host_text.c_str());
	to_host_text.clear();
	return text;
}

UAE_EXTERN_C
void uae_clipboard_free_text(char *text)
{
	if (text) {
		free(text);
	} else {
		write_log("WARNING: uae_clipboard_free_text called with NULL pointer\n");
	}
}

UAE_EXTERN_C
void uae_clipboard_put_text(const char *text)
{
	std::lock_guard<std::mutex> lock(host_mutex);
	if (!host_ready) {
		// clipboard_init not called yet
		return;
	}
	if (!text) {
		text = "";
	}
	if (from_host_text == text) {
		return;
	}
	from_host_text = text;
	from_host_changed = true;
}

static void from_iff_text(const uae_u8 *addr, uae_u32 len)
{
	std::string text;
	if (!clipboard_decode_text(addr, len, &text, false)) {
		return;
	}
	std::lock_guard<std::mutex> lock(host_mutex);
	to_host_text = text;
	to_host_pending = true;
}

static void from_iff(TrapContext *ctx, uaecptr data, uae_u32 len)
{
	if (len < 12 || !trap_valid_address(ctx, data, len)) {
		return;
	}
	std::vector<uae_u8> buffer(len);
	trap_get_bytes(ctx, buffer.data(), data, len);
	if (memcmp(buffer.data(), "FORM", 4) != 0) {
		return;
	}
	if (memcmp(buffer.data() + 8, "FTXT", 4) == 0) {
		from_iff_text(buffer.data(), len);
	} else if (memcmp(buffer.data() + 8, "ILBM", 4) == 0) {
		write_log("clipboard: Bitmap clipboard sharing not implemented\n");
	}
}

// Announces new text from the host to the Amiga, if any
static void clipboard_poll_host(void)
{
	if (clip_disabled || transfer.phase) {
		return;
	}
	std::string text;
	{
		std::lock_guard<std::mutex> lock(host_mutex);
		if (!from_host_changed) {
			return;
		}
		from_host_changed = false;
		text = from_host_text;
	}
	if (text.empty()) {
		return;
	}
	const size_t limit = initial_transfer ? CLIP_SIZE_LIMIT_INIT : CLIP_SIZE_LIMIT;
	initial_transfer = false;
	if (text.size() > limit) {
		write_log("clipboard: host text too large (%d bytes), ignoring\n", (int) text.size());
		return;
	}
	transfer.pending = clipboard_encode_text(text);
	transfer.start();
}

void clipboard_vsync(void)
{
	if (!filesys_heartbeat() || !transfer.guest_data) {
		return;
	}
	// The guest waits for this signal before opening clipboard.device.
	transfer.vsync(false);
	if (!transfer.initialized) {
		return;
	}
	clipboard_poll_host();
}

// Called from target_reset (od-fs/uaemisc.cpp)
void clipboard_reset(void)
{
	transfer.reset();
}

void clipboard_host_changed(void)
{
	// New host text is picked up by clipboard_vsync via the mailbox.
}

void clipboard_unsafeperiod(void)
{
	transfer.unsafe_period();
}

void clipboard_disable(bool disabled)
{
	clip_disabled = disabled;
}

uaecptr amiga_clipboard_proc_start(TrapContext *)
{
	transfer.signaling = 1;
	transfer.start();
	return transfer.guest_data;
}

void amiga_clipboard_task_start(TrapContext *, uaecptr data)
{
	transfer.guest_data = data;
	transfer.signaling = 1;
	write_log("clipboard task init: %08x\n", transfer.guest_data);
	transfer.start();
}

int amiga_clipboard_want_data(TrapContext *ctx)
{
	return transfer.want_data(ctx);
}

void amiga_clipboard_got_data(TrapContext *ctx, uaecptr data, uae_u32, uae_u32 actual)
{
	if (!transfer.initialized) {
		return;
	}
	from_iff(ctx, data, actual);
}

void amiga_clipboard_die(TrapContext *)
{
	transfer.signaling = 0;
	write_log("clipboard not initialized\n");
}

void amiga_clipboard_init(TrapContext *)
{
	transfer.signaling = 0;
	transfer.initialized = 1;
	initial_transfer = true;
	write_log("clipboard initialized\n");
	// Anything the host sent earlier (since from_host_changed is still set)
	// is announced from the next clipboard_vsync.
}

void target_paste_to_keyboard(void)
{
	std::string text;
	{
		std::lock_guard<std::mutex> lock(host_mutex);
		text = from_host_text;
	}
	// Convert CR / CRLF to LF, as expected by keybuf_inject
	std::string normalized;
	normalized.reserve(text.size());
	for (size_t i = 0; i < text.size(); i++) {
		if (text[i] == '\r') {
			if (i + 1 < text.size() && text[i + 1] == '\n') {
				continue;
			}
			normalized.push_back('\n');
		} else {
			normalized.push_back(text[i]);
		}
	}
	if (!normalized.empty()) {
		keybuf_inject(normalized.c_str());
	}
}
