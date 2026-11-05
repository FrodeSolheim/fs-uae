/*
 * Motion smoothness test for SDL3.
 *
 * Renders a vertical bar moving across the window at a constant speed, to check whether the
 * host (display, compositor, GPU driver) presents frames evenly, independently of FS-UAE.
 * See od-fs/docs/problems/unstable-frame-rate.md.
 *
 * By default the program is paced at a fixed 50 Hz, like an emulated PAL Amiga: the loop waits
 * until the next 20 ms slot on an absolute timeline, renders one frame, and presents it. The
 * position of the bar advances by a fixed step per frame. If the display has a different refresh
 * rate, what you see then depends on vsync / VRR / the compositor, which is the point of the test.
 *
 * Build (Linux):
 *
 *     make        (or: cc -O2 -o motion-test motion-test.c $(pkg-config --cflags --libs sdl3) -lm)
 *
 * Usage:
 *
 *     ./motion-test [--hz <rate>] [--log <file.csv>]
 *
 *     --hz   Initial pacing rate (default 50; 0 = free-running, i.e. display-synced)
 *     --log  Write one CSV row per presented frame (time, interval, pacing wake lateness),
 *            useful for analysing or sharing the results
 *
 * Keys:
 *
 *     F11 / Alt+Enter  Toggle fullscreen
 *     P                Cycle pacing: 50 Hz -> 49.92 Hz (PAL Amiga) -> 60 Hz -> display/3 ->
 *                      display/2 -> free-running. (display/N is an exact divisor of the display
 *                      refresh rate, which should be perfectly smooth with vsync; free-running
 *                      = no timer pacing, present as fast as vsync allows)
 *     V                Cycle vsync: on -> off -> adaptive (VRR friendly, if supported)
 *     M                Toggle motion mode:
 *                        per-frame: the bar moves a fixed step per presented frame, like an
 *                                   emulator (judder shows any uneven presentation)
 *                        time:      position is computed from the clock on every frame
 *     Up / Down        Change speed
 *     Esc / Q          Quit
 *
 * Statistics about the interval between presented frames are printed once per second (and
 * shown in the window title), plus how late the pacing wait woke up ("wake") which tells how
 * good the host's timers are. In a good setup the frame interval is stable (stdev well below
 * 0.1 ms when pacing at the display rate) and there are no late frames.
 *
 * The SDL render driver can be selected with e.g. SDL_RENDER_DRIVER=opengl (FS-UAE uses
 * OpenGL), SDL_RENDER_DRIVER=vulkan or SDL_RENDER_DRIVER=software.
 */

#include <SDL3/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_WIDTH 1280
#define INITIAL_HEIGHT 720

enum { MODE_PER_FRAME, MODE_TIME, MODE_COUNT };
static const char* mode_names[MODE_COUNT] = {"per-frame steps", "time-based"};

// Pacing rates to cycle through; 0 = free-running (display-synced). Built at startup, since some
// entries depend on the display refresh rate. The last entry is always 0 (free-running).
#define MAX_PACE_RATES 10
static double pace_rates[MAX_PACE_RATES];
static int pace_count = 0;

static void add_pace_rate(double hz) {
    if (pace_count < MAX_PACE_RATES - 1) {
        pace_rates[pace_count++] = hz;
    }
}

static const char* vsync_name(int vsync) {
    switch (vsync) {
        case 0:
            return "off";
        case 1:
            return "on";
        case SDL_RENDERER_VSYNC_ADAPTIVE:
            return "adaptive";
        default:
            return "other";
    }
}

typedef struct {
    int count;
    double sum_ms;
    double sum_sq_ms;
    double min_ms;
    double max_ms;
    int late;  // Intervals more than 1.5x the expected interval
    double max_wake_ms;  // Worst lateness of the pacing wait
} stats_t;

static void stats_reset(stats_t* s) {
    memset(s, 0, sizeof(*s));
    s->min_ms = 1e9;
}

int main(int argc, char** argv) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    if (!SDL_CreateWindowAndRenderer("motion-test", INITIAL_WIDTH, INITIAL_HEIGHT,
                                     SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        fprintf(stderr, "SDL_CreateWindowAndRenderer failed: %s\n", SDL_GetError());
        return 1;
    }

    int vsync = 1;
    if (!SDL_SetRenderVSync(renderer, vsync)) {
        fprintf(stderr, "Could not enable vsync: %s\n", SDL_GetError());
        vsync = 0;
    }

    printf("SDL video driver: %s\n", SDL_GetCurrentVideoDriver());
    printf("SDL render driver: %s\n", SDL_GetRendererName(renderer));
    const SDL_DisplayMode* dm = SDL_GetCurrentDisplayMode(SDL_GetDisplayForWindow(window));
    double display_hz = 0.0;
    if (dm) {
        display_hz = dm->refresh_rate;
        printf("Display mode: %dx%d @ %.3f Hz\n", dm->w, dm->h, dm->refresh_rate);
    }
    const char* session = SDL_getenv("XDG_SESSION_TYPE");
    printf("Session type: %s\n", session ? session : "(unknown)");
    printf("Keys: F11 fullscreen, V vsync, P pacing, M motion mode, Up/Down speed, Esc quit\n");

    // Pacing rates: Amiga-like rates first, then exact divisors of the display rate (which should
    // be perfectly smooth with vsync if the host presents evenly), then free-running.
    add_pace_rate(50.0);
    add_pace_rate(49.920410);
    add_pace_rate(60.0);
    if (display_hz > 0.0) {
        add_pace_rate(display_hz / 3.0);
        add_pace_rate(display_hz / 2.0);
    }
    add_pace_rate(0.0);
    int free_index = pace_count - 1;

    int mode = MODE_PER_FRAME;
    int pace_index = 0;
    FILE* log_file = NULL;
    for (int i = 1; i + 1 < argc; i++) {
        if (strcmp(argv[i], "--hz") == 0) {
            double hz = atof(argv[i + 1]);
            pace_index = pace_count - 1;
            for (int k = 0; k < pace_count; k++) {
                if (fabs(pace_rates[k] - hz) < 0.001) {
                    pace_index = k;
                }
            }
            if (hz > 0.0 && pace_index == free_index) {
                // Custom rate: insert before the free-running entry
                pace_rates[pace_count - 1] = hz;
                pace_index = pace_count - 1;
                pace_rates[pace_count++] = 0.0;
            }
        } else if (strcmp(argv[i], "--log") == 0) {
            log_file = fopen(argv[i + 1], "w");
            if (!log_file) {
                fprintf(stderr, "Could not open %s for writing\n", argv[i + 1]);
            } else {
                fprintf(log_file, "frame,time_ms,interval_ms,pace_hz,vsync,wake_ms\n");
            }
        }
    }
    // Pixels per second (before any window scaling)
    double speed = 800.0;
    bool fullscreen = false;
    bool running = true;

    stats_t stats;
    stats_reset(&stats);
    uint64_t start_ns = SDL_GetTicksNS();
    uint64_t last_present_ns = 0;
    uint64_t last_report_ns = start_ns;
    uint64_t next_at_ns = 0;  // Absolute pacing timeline
    double frame_pos = 0.0;   // Per-frame mode position, in pixels

    printf("Pacing: %s\n", pace_rates[pace_index] > 0.0 ? "fixed rate" : "free-running");
    uint64_t frame_no = 0;
    double last_wake_ms = 0.0;

    while (running) {
        double pace_hz = pace_rates[pace_index];

        // Pace to an absolute timeline: sleep most of the time, spin the last bit.
        if (pace_hz > 0.0) {
            uint64_t period_ns = (uint64_t)(1e9 / pace_hz);
            uint64_t now = SDL_GetTicksNS();
            if (next_at_ns == 0 || now > next_at_ns + 2 * period_ns) {
                next_at_ns = now;  // First frame, or too far behind: re-base the timeline
            }
            while ((now = SDL_GetTicksNS()) < next_at_ns) {
                uint64_t left = next_at_ns - now;
                if (left > 2000000ULL) {
                    SDL_DelayNS(left - 2000000ULL);
                }
            }
            double wake_ms = (double)(now - next_at_ns) / 1e6;
            last_wake_ms = wake_ms;
            if (wake_ms > stats.max_wake_ms) {
                stats.max_wake_ms = wake_ms;
            }
            next_at_ns += period_ns;
        } else {
            next_at_ns = 0;
        }

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            } else if (event.type == SDL_EVENT_KEY_DOWN && !event.key.repeat) {
                SDL_Keycode key = event.key.key;
                if (key == SDLK_ESCAPE || key == SDLK_Q) {
                    running = false;
                } else if (key == SDLK_F11 ||
                           (key == SDLK_RETURN && (event.key.mod & SDL_KMOD_ALT))) {
                    fullscreen = !fullscreen;
                    if (!SDL_SetWindowFullscreen(window, fullscreen)) {
                        fprintf(stderr, "SDL_SetWindowFullscreen failed: %s\n", SDL_GetError());
                    }
                    printf("Fullscreen: %s\n", fullscreen ? "on" : "off");
                    stats_reset(&stats);
                    last_present_ns = 0;
                    next_at_ns = 0;
                } else if (key == SDLK_V) {
                    int next = vsync == 1 ? 0 : (vsync == 0 ? SDL_RENDERER_VSYNC_ADAPTIVE : 1);
                    if (!SDL_SetRenderVSync(renderer, next)) {
                        fprintf(stderr, "Vsync mode %s not supported: %s\n", vsync_name(next),
                                SDL_GetError());
                    }
                    // Remember the requested mode even if unsupported, so the cycle continues
                    vsync = next;
                    printf("Vsync: %s\n", vsync_name(vsync));
                    stats_reset(&stats);
                    last_present_ns = 0;
                } else if (key == SDLK_P) {
                    pace_index = (pace_index + 1) % pace_count;
                    if (pace_rates[pace_index] > 0.0) {
                        printf("Pacing: %.3f Hz\n", pace_rates[pace_index]);
                    } else {
                        printf("Pacing: free-running\n");
                    }
                    stats_reset(&stats);
                    last_present_ns = 0;
                    next_at_ns = 0;
                } else if (key == SDLK_M) {
                    mode = (mode + 1) % MODE_COUNT;
                    printf("Motion mode: %s\n", mode_names[mode]);
                    stats_reset(&stats);
                } else if (key == SDLK_UP) {
                    speed *= 1.25;
                    printf("Speed: %.0f px/s\n", speed);
                } else if (key == SDLK_DOWN) {
                    speed /= 1.25;
                    printf("Speed: %.0f px/s\n", speed);
                }
            }
        }

        int w = 0, h = 0;
        SDL_GetCurrentRenderOutputSize(renderer, &w, &h);

        float bar_w = (float)h / 8.0f;
        float bar_h = (float)h * 0.6f;
        double travel = (double)w + bar_w;

        double pos;
        if (mode == MODE_PER_FRAME) {
            // Fixed step per frame, independent of when the frame is actually presented.
            double step_hz = pace_hz > 0.0 ? pace_hz : (display_hz > 0.0 ? display_hz : 60.0);
            frame_pos += speed / step_hz;
            if (frame_pos >= travel) {
                frame_pos = fmod(frame_pos, travel);
            }
            pos = frame_pos;
        } else {
            double t = (double)(SDL_GetTicksNS() - start_ns) / 1e9;
            pos = fmod(t * speed, travel);
        }
        float x = (float)pos - bar_w;
        float y = ((float)h - bar_h) / 2.0f;

        SDL_SetRenderDrawColor(renderer, 24, 24, 32, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
        SDL_FRect bar = {x, y, bar_w, bar_h};
        SDL_RenderFillRect(renderer, &bar);
        SDL_RenderPresent(renderer);

        uint64_t now_ns = SDL_GetTicksNS();
        if (last_present_ns != 0) {
            double ms = (double)(now_ns - last_present_ns) / 1e6;
            stats.count++;
            stats.sum_ms += ms;
            stats.sum_sq_ms += ms * ms;
            if (ms < stats.min_ms) {
                stats.min_ms = ms;
            }
            if (ms > stats.max_ms) {
                stats.max_ms = ms;
            }
            if (log_file) {
                fprintf(log_file, "%llu,%.3f,%.3f,%.3f,%d,%.3f\n", (unsigned long long)frame_no,
                        (double)(now_ns - start_ns) / 1e6, ms, pace_hz, vsync, last_wake_ms);
            }
            double expected_ms = pace_hz > 0.0 ? 1000.0 / pace_hz
                                               : (display_hz > 0.0 ? 1000.0 / display_hz : 1000.0 / 60.0);
            if (ms > expected_ms * 1.5) {
                stats.late++;
            }
        }
        last_present_ns = now_ns;
        frame_no++;

        if (now_ns - last_report_ns >= 1000000000ULL) {
            if (stats.count > 0) {
                double avg = stats.sum_ms / stats.count;
                double var = stats.sum_sq_ms / stats.count - avg * avg;
                double stdev = var > 0.0 ? sqrt(var) : 0.0;
                char pace[32];
                if (pace_hz > 0.0) {
                    snprintf(pace, sizeof(pace), "%.2f Hz", pace_hz);
                } else {
                    snprintf(pace, sizeof(pace), "free");
                }
                char line[300];
                snprintf(line, sizeof(line),
                         "pace %s | %s | vsync %s | %.2f fps | frame ms avg %.3f stdev %.3f "
                         "min %.3f max %.3f | late %d | wake max %.3f ms",
                         pace, mode_names[mode], vsync_name(vsync), 1000.0 / avg, avg, stdev,
                         stats.min_ms, stats.max_ms, stats.late, stats.max_wake_ms);
                printf("%s\n", line);
                SDL_SetWindowTitle(window, line);
            }
            stats_reset(&stats);
            last_report_ns = now_ns;
        }
    }

    if (log_file) {
        fclose(log_file);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
