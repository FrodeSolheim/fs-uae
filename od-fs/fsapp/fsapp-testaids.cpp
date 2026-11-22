/*
 * Test aids for headless runs of the main loop (see fsapp-testaids.h).
 *
 * Copyright (c) 2026 Frode Solheim
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "fsapp-testaids.h"

#include <SDL3/SDL.h>

#include "fsapp-time.h"

// The simulated display: set when the main loop starts, and only read after that.
static struct {
    bool enabled;
    double refresh_rate_hz;
    // Tick 0, when the main loop started.
    int64_t first_tick_ns;
    // How late the swap returns on the late ticks (FSAPP_SIMULATED_DISPLAY_LATE_US), 0 for never.
    int64_t late_swap_ns;
    // The compositor's lead (FSAPP_SIMULATED_DISPLAY_LEAD_US), 0 for none, and the swap held
    // before (its number and tick, 0 for none), whose presentation the next frame record reports.
    int64_t lead_ns;
    int64_t held_swap_number;
    int64_t held_tick_ns;
} simulated_display;

// The simulated drawing delay: set when the main loop starts.
static struct {
    // The delay, 0 when it is off.
    int64_t delay_ns;
    // The one swap that is delayed, or 0 for every swap.
    int64_t swap_number;
} simulated_render;

// Test input: set when the main loop starts, then used by its timer's callback only.
static struct {
    SDL_Scancode scancode;
    // Whether the next event is the press (else the release).
    bool press;
    // The timer that pushes the events, 0 when the test input is off.
    SDL_TimerID timer;
} test_input;

// The test display scale: read at the first call, 0 when it is off.
static struct {
    bool read;
    float scale;
} test_display_scale;

// Turns the simulated display on when FSAPP_SIMULATED_DISPLAY_HZ gives a refresh rate (such as 50
// or 59.94), with its first tick now.
static void simulated_display_init(void)
{
    const char *value = SDL_getenv("FSAPP_SIMULATED_DISPLAY_HZ");
    if (!value || !value[0]) {
        return;
    }
    double refresh_rate_hz = SDL_strtod(value, NULL);
    if (!(refresh_rate_hz >= 1.0 && refresh_rate_hz <= 1000.0)) {
        SDL_Log("FSAPP_SIMULATED_DISPLAY_HZ=%s is not a refresh rate (1 to 1000 Hz), ignored",
                value);
        return;
    }
    simulated_display.enabled = true;
    simulated_display.refresh_rate_hz = refresh_rate_hz;
    simulated_display.first_tick_ns = (int64_t)SDL_GetTicksNS();
    SDL_Log("Simulated display: %.3f Hz (FSAPP_SIMULATED_DISPLAY_HZ), the swap waits for its next "
            "tick unless the swap interval is 0",
            refresh_rate_hz);
    const char *late = SDL_getenv("FSAPP_SIMULATED_DISPLAY_LATE_US");
    if (late && late[0]) {
        long long late_us = SDL_strtoll(late, NULL, 10);
        if (late_us < 1 || late_us >= 1000000.0 / refresh_rate_hz) {
            SDL_Log("FSAPP_SIMULATED_DISPLAY_LATE_US=%s is not a time in microseconds shorter than "
                    "the refresh period, ignored",
                    late);
        } else {
            simulated_display.late_swap_ns = (int64_t)SDL_US_TO_NS(late_us);
            SDL_Log("Simulated display: the swap returns %lld us late on about half the ticks "
                    "(FSAPP_SIMULATED_DISPLAY_LATE_US)",
                    late_us);
        }
    }
    const char *lead = SDL_getenv("FSAPP_SIMULATED_DISPLAY_LEAD_US");
    if (lead && lead[0]) {
        long long lead_us = SDL_strtoll(lead, NULL, 10);
        if (lead_us < 1 || lead_us >= 1000000.0 / refresh_rate_hz / 2) {
            SDL_Log("FSAPP_SIMULATED_DISPLAY_LEAD_US=%s is not a time in microseconds shorter than "
                    "half the refresh period, ignored",
                    lead);
        } else {
            simulated_display.lead_ns = (int64_t)SDL_US_TO_NS(lead_us);
            SDL_Log(
                "Simulated display: the swap returns %lld us before its tick, which is reported "
                "as its presentation (FSAPP_SIMULATED_DISPLAY_LEAD_US)",
                lead_us);
        }
    }
}

// Turns the simulated drawing delay on when FSAPP_SIMULATED_RENDER_US gives a delay in microseconds
// (up to a second), for every swap, or followed by @ and the number of the one swap to delay (such
// as 3000 or 8000@60).
static void simulated_render_init(void)
{
    const char *value = SDL_getenv("FSAPP_SIMULATED_RENDER_US");
    if (!value || !value[0]) {
        return;
    }
    char *end = NULL;
    long long delay_us = SDL_strtoll(value, &end, 10);
    long long swap_number = 0;
    bool valid = end != value && delay_us >= 1 && delay_us <= 1000000;
    if (valid && *end == '@') {
        const char *swap = end + 1;
        swap_number = SDL_strtoll(swap, &end, 10);
        valid = end != swap && swap_number >= 1;
    }
    if (!valid || *end != '\0') {
        SDL_Log("FSAPP_SIMULATED_RENDER_US=%s is not a delay in microseconds (1 to 1000000), "
                "optionally followed by @ and a swap number, ignored",
                value);
        return;
    }
    simulated_render.delay_ns = (int64_t)SDL_US_TO_NS(delay_us);
    simulated_render.swap_number = swap_number;
    if (swap_number) {
        SDL_Log("Simulated drawing delay: rendering finished %lld us late on swap %lld "
                "(FSAPP_SIMULATED_RENDER_US)",
                delay_us, swap_number);
    } else {
        SDL_Log("Simulated drawing delay: rendering finished %lld us late on every swap "
                "(FSAPP_SIMULATED_RENDER_US)",
                delay_us);
    }
}

// The test input's timer (on SDL's timer thread): pushes the next key event, as input that comes
// from outside the main loop does, whatever the main thread is doing. Returns the period, so that
// it is called again then.
static Uint64 SDLCALL test_input_push(void *userdata, SDL_TimerID timer_id, Uint64 interval_ns)
{
    (void)userdata;
    (void)timer_id;
    SDL_Event event;
    SDL_zero(event);
    event.type = test_input.press ? SDL_EVENT_KEY_DOWN : SDL_EVENT_KEY_UP;
    event.key.scancode = test_input.scancode;
    event.key.key = SDL_GetKeyFromScancode(test_input.scancode, SDL_KMOD_NONE, false);
    event.key.down = test_input.press;
    test_input.press = !test_input.press;
    SDL_PushEvent(&event);
    return interval_ns;
}

// Turns the test input on when FSAPP_TEST_INPUT gives its period in milliseconds, and optionally
// SDL's name of the key after a comma (such as 37 or "37,Right Ctrl").
static void test_input_init(void)
{
    const char *value = SDL_getenv("FSAPP_TEST_INPUT");
    if (!value || !value[0]) {
        return;
    }
    int period_ms = SDL_atoi(value);
    const char *comma = SDL_strchr(value, ',');
    SDL_Scancode scancode = comma ? SDL_GetScancodeFromName(comma + 1) : SDL_SCANCODE_LSHIFT;
    if (period_ms < 1 || scancode == SDL_SCANCODE_UNKNOWN) {
        SDL_Log("FSAPP_TEST_INPUT=%s is not a period in milliseconds (and a key name), ignored",
                value);
        return;
    }
    test_input.scancode = scancode;
    test_input.press = true;
    Uint64 period_ns = (Uint64)period_ms * SDL_NS_PER_MS;
    test_input.timer = SDL_AddTimerNS(period_ns, test_input_push, NULL);
    if (test_input.timer == 0) {
        SDL_Log("Test input: could not add its timer: %s", SDL_GetError());
        return;
    }
    SDL_Log("Test input: %s pressed or released every %d ms (FSAPP_TEST_INPUT)",
            SDL_GetScancodeName(scancode), period_ms);
}

void fsapp_testaids_init(void)
{
    simulated_display_init();
    simulated_render_init();
    test_input_init();
}

bool fsapp_testaids_is_display_simulated(void)
{
    return simulated_display.enabled;
}

double fsapp_testaids_get_simulated_refresh_rate_hz(void)
{
    return simulated_display.refresh_rate_hz;
}

int64_t fsapp_testaids_get_next_simulated_tick_ns(int64_t now_ns)
{
    double period_ns = (double)SDL_NS_PER_SECOND / simulated_display.refresh_rate_hz;
    int64_t tick =
        (int64_t)SDL_floor((double)(now_ns - simulated_display.first_tick_ns) / period_ns);
    int64_t tick_ns;
    do {
        tick++;
        tick_ns = simulated_display.first_tick_ns + (int64_t)SDL_floor(tick * period_ns + 0.5);
    } while (tick_ns <= now_ns);
    return tick_ns;
}

int64_t fsapp_testaids_get_simulated_late_swap_ns(int64_t tick_ns)
{
    if (simulated_display.late_swap_ns == 0) {
        return 0;
    }
    double period_ns = (double)SDL_NS_PER_SECOND / simulated_display.refresh_rate_hz;
    uint32_t tick =
        (uint32_t)SDL_lround((double)(tick_ns - simulated_display.first_tick_ns) / period_ns);
    // The top bit of a multiplicative hash of the tick's number, so the pattern is irregular but
    // the same in every run.
    uint32_t hash = tick * 2654435761u;
    return hash >> 31 ? simulated_display.late_swap_ns : 0;
}

int64_t fsapp_testaids_get_simulated_lead_ns(void)
{
    return simulated_display.lead_ns;
}

bool fsapp_testaids_take_simulated_presentation(int64_t swap_number, int64_t tick_ns,
                                                fsapp_frame_presented_t *presented)
{
    bool taken = false;
    if (simulated_display.held_tick_ns != 0) {
        SDL_zerop(presented);
        presented->swap_number = simulated_display.held_swap_number;
        presented->presented_ns = simulated_display.held_tick_ns;
        presented->refresh_ns =
            (int64_t)SDL_round((double)SDL_NS_PER_SECOND / simulated_display.refresh_rate_hz);
        presented->sequence = (uint64_t)simulated_display.held_swap_number;
        presented->flags = FSAPP_FRAME_PRESENTED_VSYNC | FSAPP_FRAME_PRESENTED_HW_CLOCK |
                           FSAPP_FRAME_PRESENTED_HW_COMPLETION;
        presented->mode = FSAPP_FRAME_PRESENTED_MODE_NONE;
        taken = true;
    }
    simulated_display.held_swap_number = swap_number;
    simulated_display.held_tick_ns = tick_ns;
    return taken;
}

int64_t fsapp_testaids_get_simulated_render_delay_ns(int64_t swap_number)
{
    if (simulated_render.swap_number != 0 && swap_number != simulated_render.swap_number) {
        return 0;
    }
    return simulated_render.delay_ns;
}

// REVIEW: Only the display scale is replaced, not the host window's size or pixel density, so the
// window is as large in pixels as on a display at that scale with a pixel density of 1 (Windows,
// X11), not as on macOS or Wayland.
float fsapp_testaids_override_display_scale(float display_scale)
{
    if (!test_display_scale.read) {
        test_display_scale.read = true;
        const char *value = SDL_getenv("FSAPP_TEST_DISPLAY_SCALE");
        if (value && value[0]) {
            float scale = (float)SDL_atof(value);
            if (scale > 0.0f) {
                test_display_scale.scale = scale;
                SDL_Log("Test display scale: %g (FSAPP_TEST_DISPLAY_SCALE)", scale);
            } else {
                SDL_Log("FSAPP_TEST_DISPLAY_SCALE=%s is not a scale more than 0, ignored", value);
            }
        }
    }
    return test_display_scale.scale > 0.0f ? test_display_scale.scale : display_scale;
}

void fsapp_testaids_quit(void)
{
    if (test_input.timer != 0) {
        SDL_RemoveTimer(test_input.timer);
        test_input.timer = 0;
    }
}
