/*
 * The emulation core's clock (uae/time.h) for FS-UAE: uae_time(), uae_time_us(), uae_time_ns()
 * and read_processor_time(), replacing support/time.cpp (which only WinUAE builds now).
 *
 * The clock is SDL_GetTicksNS(): nanoseconds since SDL's ticks were initialised, from
 * SDL_GetPerformanceCounter() (QueryPerformanceCounter on Windows, mach_absolute_time on macOS,
 * CLOCK_MONOTONIC_RAW elsewhere). uae_time() and read_processor_time() count in nanoseconds, and
 * syncbase (the clock's ticks per second, which the core scales by) is set to match.
 *
 * Copyright (c) 2026 Frode Solheim
 */

#include "sysconfig.h"
#include "sysdeps.h"

#include <SDL3/SDL.h>

#include "custom.h"
#include "uae/time.h"

void uae_time_init(void)
{
    // Starts SDL's ticks if nothing has yet, so that the epoch is not later than this.
    SDL_GetTicksNS();
    syncbase = SDL_NS_PER_SECOND;
}

uae_time_t uae_time(void)
{
    return (uae_time_t)SDL_GetTicksNS();
}

int64_t uae_time_us(void)
{
    return (int64_t)SDL_NS_TO_US(SDL_GetTicksNS());
}

int64_t uae_time_ns(void)
{
    return (int64_t)SDL_GetTicksNS();
}

void uae_deterministic_amiga_time(int *days, int *mins, int *ticks)
{
    // FIXME: Would be nice if the netplay server could broadcast a suitable start time.
    // FIXME: Also integrate this with battery clock emulation
    // FIXME: This works quite well for PAL (ticks = 1/50 sec). Should tune for NTSC...

    long t = vsync_counter;

    int ticks_per_min = 50 * 60;
    int ticks_per_day = 24 * 60 * ticks_per_min;

    *days = t / ticks_per_day;
    t -= *days * ticks_per_day;
    *mins = t / ticks_per_min;
    t -= *mins * ticks_per_min;
    *ticks = t;
    // Start at day one so time looks more valid for certain programs?
    // *days += 1;
}
