/*
 * MP3 decoder for CD audio tracks (cue sheets with MP3 files), using libmpg123. This is the FS-UAE
 * counterpart of od-win32/mp3decoder.cpp, which uses the Windows ACM codec. The class is declared
 * in od-win32/mp3decoder.h.
 *
 * libmpg123 does all the work: it reads the file through a zfile, finds the exact length
 * (mpg123_scan, with encoder delay and padding removed when the file has a LAME/Info tag), and
 * decodes to the CD audio format, converting mono and other sample rates as needed.
 *
 * Copyright (c) 2026 Frode Solheim
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "sysconfig.h"
#include "sysdeps.h"

#include "zfile.h"
#include "mp3decoder.h"

#include <mpg123.h>

#include <climits>
#include <mutex>

// CD audio: 44.1 kHz, stereo, 16-bit signed samples in native byte order (the same as the FLAC
// decoder in blkdev_cdimage.cpp).
#define MP3_RATE 44100
#define MP3_BYTES_PER_SAMPLE 4

// The 64-bit reader API (mpg123_reader64, mpg123_length64) is available since API version 48
// (mpg123 1.32), mpg123_open_handle64 since 49 (mpg123 1.33). Older versions (e.g. 1.31 in
// Debian 12) only have the off_t API.
#if MPG123_API_VERSION >= 48
#define MP3_READER64 1
#endif

static bool mp3_init()
{
    static std::once_flag once;
    static bool ok;
    // Does nothing since mpg123 1.27, but older versions need it once, and it is not thread-safe
    // there.
    std::call_once(once, [] { ok = mpg123_init() == MPG123_OK; });
    return ok;
}

static int64_t mp3_seek(struct zfile *zf, int64_t offset, int whence)
{
    if (zfile_fseek(zf, offset, whence) < 0) {
        return -1;
    }
    return zfile_ftell(zf);
}

#ifdef MP3_READER64

static int mp3_read_cb(void *handle, void *buf, size_t count, size_t *done)
{
    *done = zfile_fread(buf, 1, count, static_cast<struct zfile *>(handle));
    return 0;
}

static int64_t mp3_seek_cb(void *handle, int64_t offset, int whence)
{
    return mp3_seek(static_cast<struct zfile *>(handle), offset, whence);
}

#else

static ssize_t mp3_read_cb(void *handle, void *buf, size_t count)
{
    return zfile_fread(buf, 1, count, static_cast<struct zfile *>(handle));
}

static off_t mp3_seek_cb(void *handle, off_t offset, int whence)
{
    return mp3_seek(static_cast<struct zfile *>(handle), offset, whence);
}

#endif

static void mp3_close(mpg123_handle *mh)
{
    mpg123_close(mh);
    mpg123_delete(mh);
}

// Opens the zfile for decoding from the start, with output fixed to the CD audio format. Returns
// nullptr (after logging why) on failure.
static mpg123_handle *mp3_open(struct zfile *zf)
{
    if (!mp3_init()) {
        write_log(_T("MP3: failed to initialize libmpg123\n"));
        return nullptr;
    }
    int err = MPG123_OK;
    mpg123_handle *mh = mpg123_new(nullptr, &err);
    if (mh == nullptr) {
        write_log(_T("MP3: mpg123_new failed: %s\n"), mpg123_plain_strerror(err));
        return nullptr;
    }
    // Don't let libmpg123 print to stderr; errors are logged below.
    mpg123_param(mh, MPG123_ADD_FLAGS, MPG123_QUIET, 0);

    err = mpg123_format_none(mh);
    if (err == MPG123_OK) {
        err = mpg123_format(mh, MP3_RATE, MPG123_STEREO, MPG123_ENC_SIGNED_16);
    }
    if (err == MPG123_OK) {
#ifdef MP3_READER64
        err = mpg123_reader64(mh, mp3_read_cb, mp3_seek_cb, nullptr);
#else
        err = mpg123_replace_reader_handle(mh, mp3_read_cb, mp3_seek_cb, nullptr);
#endif
    }
    if (err == MPG123_OK) {
        zfile_fseek(zf, 0, SEEK_SET);
#if MPG123_API_VERSION >= 49
        err = mpg123_open_handle64(mh, zf);
#else
        err = mpg123_open_handle(mh, zf);
#endif
    }
    if (err != MPG123_OK) {
        write_log(_T("MP3: could not open '%s': %s\n"), zfile_getname(zf), mpg123_strerror(mh));
        mpg123_delete(mh);
        return nullptr;
    }
    return mh;
}

mp3decoder::mp3decoder() : g_mp3stream(nullptr)
{
}

mp3decoder::~mp3decoder()
{
}

uae_u8 *mp3decoder::get(struct zfile *zf, uae_u8 *outbuf, int maxsize)
{
    write_log(_T("MP3: decoding '%s'..\n"), zfile_getname(zf));
    mpg123_handle *mh = mp3_open(zf);
    if (mh == nullptr) {
        // The caller has allocated (and cleared) outbuf. Returning it, as silence, rather than
        // nullptr, avoids leaking it and retrying.
        return outbuf;
    }
    size_t size = maxsize > 0 ? maxsize : 0;
    size_t offset = 0;
    int ret = MPG123_OK;
    while (offset < size) {
        size_t done = 0;
        ret = mpg123_read(mh, outbuf + offset, size - offset, &done);
        offset += done;
        // MPG123_NEW_FORMAT comes once, before the first samples.
        if (ret != MPG123_OK && ret != MPG123_NEW_FORMAT) {
            break;
        }
    }
    if (ret != MPG123_OK && ret != MPG123_NEW_FORMAT && ret != MPG123_DONE) {
        write_log(_T("MP3: decoding error: %s\n"), mpg123_strerror(mh));
    }
    write_log(_T("MP3: unpacked size %zu bytes\n"), offset);
    mp3_close(mh);
    return outbuf;
}

uae_u32 mp3decoder::getsize(struct zfile *zf)
{
    mpg123_handle *mh = mp3_open(zf);
    if (mh == nullptr) {
        return 0;
    }
    int64_t samples = -1;
    if (mpg123_scan(mh) == MPG123_OK) {
#ifdef MP3_READER64
        samples = mpg123_length64(mh);
#else
        samples = mpg123_length(mh);
#endif
    }
    if (samples < 0) {
        write_log(_T("MP3: could not get length of '%s': %s\n"), zfile_getname(zf),
                  mpg123_strerror(mh));
    }
    mp3_close(mh);
    if (samples <= 0) {
        return 0;
    }
    // The caller allocates size + 2352 bytes and passes size on as an int.
    const int64_t max_samples = (INT_MAX - 2352) / MP3_BYTES_PER_SAMPLE;
    if (samples > max_samples) {
        samples = max_samples;
    }
    return static_cast<uae_u32>(samples * MP3_BYTES_PER_SAMPLE);
}
