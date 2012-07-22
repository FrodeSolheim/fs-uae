#include "sysconfig.h"
#include "sysdeps.h"

#include "cda_play.h"
#include "libamiga_internal.h"

static int (*g_audio_callback)(int16_t *buffer, int size) = NULL;

int amiga_set_cd_audio_callback(audio_callback func) {
    g_audio_callback = func;
    return 1;
}

cda_audio::~cda_audio() {
    write_log("cda_audio::~cda_audio()\n");
    wait(0);
    wait(1);
#if 0
    write_log("waiting for cdda thread\n");
    mStopThread = 1;
    uae_wait_thread(mThread);
    write_log("done waiting for cdda thread\n");
#endif
    for (int i = 0; i < 2; i++) {
        xfree (buffers[i]);
        buffers[i] = NULL;
    }
}

#if 0
#define BYTES_PER_SEC (44100 * 2 * 2)

static void *audio_thread(void *cda_pointer) {
    cda_audio *cda = (cda_audio *) cda_pointer;

    int bufnum = 0;
    while (!cda->mStopThread) {
        if (g_amiga_paused) {
            Sleep(10);
            continue;
        }
        if (cda->mBufferDone[bufnum]) {
            // waiting for buffer to become ready to play
            Sleep(1);
            continue;
        }

        uae_s16 *p = (uae_s16*)(cda->buffers[bufnum]);
        if (g_audio_callback) {
            for (int i = 0; i < cda->num_sectors; i++) {
                g_audio_callback(p, 2352);
                p += (2352 / 2);
                // FIXME: use a clock here as well, to time the sleeps
                // better..
                Sleep(1000 * 2352.0 / BYTES_PER_SEC);
            }
        }

        cda->mBufferDone[bufnum] = 1;
        bufnum = !bufnum;
    }
    write_log("end of cdda thread\n");
    return NULL;
}
#endif

cda_audio::cda_audio(int num_sectors) {
    write_log("cda_audio::cda_audio(num_sectors=%d)\n", num_sectors);
#if 0
    mStopThread = 0;
    mBufferDone[0] = 1;
    mBufferDone[1] = 1;
    uae_start_thread("cdda", audio_thread, this, &mThread);
#endif

    active = false;
    playing = false;
    volume[0] = volume[1] = 0;

    bufsize = num_sectors * 2352;
    for (int i = 0; i < 2; i++) {
        buffer_ids[i] = 0;
        buffers[i] = xcalloc (uae_u8, num_sectors * 4096);
    }
    this->num_sectors = num_sectors;
    active = true;
    playing = true;

}

void cda_audio::setvolume(int master, int left, int right) {
    for (int j = 0; j < 2; j++) {
        volume[j] = j == 0 ? left : right;
        volume[j] = (100 - master) * volume[j] / 100;
        if (volume[j])
            volume[j]++;
        if (volume[j] >= 32768)
            volume[j] = 32768;
    }
}

bool cda_audio::play(int bufnum) {
    if (!active) {
        return false;
    }

    uae_s16 *p = (uae_s16*)(buffers[bufnum]);
    for (int i = 0; i < num_sectors * 2352 / 4; i++) {
        p[i * 2 + 0] = p[i * 2 + 0] * volume[0] / 32768;
        p[i * 2 + 1] = p[i * 2 + 1] * volume[1] / 32768;
    }

    if (g_audio_callback) {
        buffer_ids[bufnum] = g_audio_callback(p, num_sectors * 2352);
    }
    else {
        buffer_ids[bufnum] = 0;
    }

#if 0
    mBufferDone[bufnum] = 0;
#endif
    return true;

}

void cda_audio::wait(int bufnum) {
    if (!active || !playing) {
        return;
    }

    if (buffer_ids[bufnum] == 0) {
        return;
    }

    // calling g_audio_callback with NULL parameter to check status
    while (!g_audio_callback(NULL, buffer_ids[bufnum])) {
        Sleep (10);
    }

#if 0
    while (!mBufferDone[bufnum]) {
        Sleep (10);
    }
#endif
}
