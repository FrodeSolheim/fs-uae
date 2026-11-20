/*
 * Motion smoothness test for SDL3.
 *
 * Renders a vertical bar moving across the window at a constant speed, to check whether the
 * host (display, compositor, GPU driver) presents frames evenly, independently of FS-UAE.
 *
 * By default the program is paced at a fixed 50 Hz, like an emulated PAL Amiga, on an absolute
 * timeline of 20 ms slots. The position of the bar advances by a fixed step per frame. If the
 * display has a different refresh rate, what you see then depends on vsync / VRR / the
 * compositor, which is the point of the test.
 *
 * Each iteration of the main loop does, in this order:
 *
 *     1. wait until the slot starts (sleep most of the time, spin the last 2 ms)
 *     2. present the frame rendered in the previous iteration
 *     3. handle input events, print statistics
 *     4. render the next frame and flush it to the GPU
 *
 * so that nothing but the present itself happens between the end of the wait and the present.
 * Input handling, statistics and rendering use the time left until the next slot, and their
 * duration never moves a present. With VRR, this matters: a monitor with a 48 Hz minimum
 * refresh rate gives 50 Hz frames only 0.83 ms of margin (20.833 ms - 20 ms) before it repeats
 * the previous frame by itself, which delays the next frame and shows as a stutter.
 *
 * Frame repeat (--repeat / R) presents each frame several times, at that multiple of the pacing
 * rate (50 Hz with repeat 2 presents at 100 Hz), with the bar moving once per frame, not per
 * present. This keeps the present interval well inside the VRR range (10 ms against a 20.833 ms
 * maximum), like the monitor's low framerate compensation does below its minimum rate.
 *
 * Black frame insertion (--bfi / B) is the alternative to repeating a frame: the frame is shown
 * on the first present only, and the other presents of the frame are black (with repeat 2: the
 * frame for 10 ms, then black for 10 ms). This shortens the time each frame stays on screen,
 * which reduces the motion blur from the eye following the bar (sample-and-hold blur), at the
 * cost of brightness and visible flicker at the frame rate. It needs a repeat of 2 or more, so
 * turning it on with repeat 1 sets repeat 2.
 *
 * Build (Linux):
 *
 *     make        (or: cc -O2 -o motion-test motion-test.c $(pkg-config --cflags --libs sdl3) -lm)
 *
 * Usage:
 *
 *     ./motion-test [--hz <rate>] [--repeat <n>] [--bfi] [--vrr-min <rate>] [--log <file.csv>]
 *
 *     --hz       Initial pacing rate (default 50; 0 = free-running, i.e. display-synced)
 *     --repeat   Present each frame n times (1-4, default 1)
 *     --bfi      Black frame insertion: black instead of the frame for the repeated presents
 *     --vrr-min  Minimum refresh rate of the monitor's VRR range, e.g. 48. Present intervals
 *                longer than 1 / rate are counted as "vrr miss" (the monitor will have repeated
 *                the previous frame by itself)
 *     --log      Write one CSV row per present (time, interval, pacing wake lateness, time
 *                spent in the present call), useful for analysing or sharing the results
 *
 * Keys:
 *
 *     F11 / Alt+Enter  Toggle fullscreen
 *     P                Cycle pacing: 50 Hz -> 49.92 Hz (PAL Amiga) -> 60 Hz -> display/3 ->
 *                      display/2 -> free-running. (display/N is an exact divisor of the display
 *                      refresh rate, which should be perfectly smooth with vsync; free-running
 *                      = no timer pacing, present as fast as vsync allows)
 *     R                Cycle frame repeat: 1 -> 2 -> 3 -> 4 (2 -> 3 -> 4 with black frame
 *                      insertion)
 *     B                Toggle black frame insertion
 *     V                Cycle vsync: on -> off -> adaptive (VRR friendly, if supported)
 *     M                Toggle motion mode:
 *                        per-frame: the bar moves a fixed step per frame, like an emulator
 *                                   (judder shows any uneven presentation)
 *                        time:      the position is computed from the time the frame is due
 *     Up / Down        Change speed
 *     Esc / Q          Quit
 *
 * Statistics about the interval between presents are printed once per second (and shown in the
 * window title), plus the longest time from a present to the next wait ("work": input,
 * statistics and rendering), how late the pacing wait woke up ("wake": timer oversleep, or work
 * that ran past the slot) and the longest time spent in the present call ("present"). The --log
 * CSV has the same per present, so a late present can be traced to its cause. In a good setup the
 * interval is stable (stdev well below 0.1 ms when pacing at the display rate) and there are no
 * late frames or VRR misses.
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
#define MAX_REPEAT 4

// The pacing wait sleeps until this long before the slot and spins the rest
#define SPIN_NS 2000000ULL

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
    int late;             // Intervals more than 1.5x the expected interval
    int vrr_miss;         // Intervals longer than the VRR range allows (with --vrr-min)
    double max_wake_ms;   // Worst lateness of the pacing wait
    double max_present_ms;  // Longest time spent in SDL_RenderPresent
    double max_work_ms;     // Longest time from a present to the next wait (input, rendering)
} stats_t;

static void stats_reset(stats_t* s) {
    memset(s, 0, sizeof(*s));
    s->min_ms = 1e9;
}

// Renders the bar at pos, or a black frame (for black frame insertion)
static void render_frame(SDL_Renderer* renderer, double pos, bool black) {
    if (black) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_FlushRenderer(renderer);
        return;
    }
    int w = 0, h = 0;
    SDL_GetCurrentRenderOutputSize(renderer, &w, &h);
    float bar_w = (float)h / 8.0f;
    float bar_h = (float)h * 0.6f;
    float x = (float)pos - bar_w;
    float y = ((float)h - bar_h) / 2.0f;

    SDL_SetRenderDrawColor(renderer, 24, 24, 32, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
    SDL_FRect bar = {x, y, bar_w, bar_h};
    SDL_RenderFillRect(renderer, &bar);
    // Submit the drawing now, so that the present after the wait has as little to do as possible
    SDL_FlushRenderer(renderer);
}

// The distance the bar travels before it wraps around: the window width plus the bar width
static double travel_for(SDL_Renderer* renderer) {
    int w = 0, h = 0;
    SDL_GetCurrentRenderOutputSize(renderer, &w, &h);
    return (double)w + (double)h / 8.0;
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
    printf("Keys: F11 fullscreen, V vsync, P pacing, R repeat, B black frame insertion, "
           "M motion mode, Up/Down speed, Esc quit\n");

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
    int repeat = 1;
    bool bfi = false;
    double vrr_min_hz = 0.0;
    FILE* log_file = NULL;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--bfi") == 0) {
            bfi = true;
            continue;
        }
        if (i + 1 >= argc) {
            break;
        }
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
        } else if (strcmp(argv[i], "--repeat") == 0) {
            repeat = atoi(argv[i + 1]);
            if (repeat < 1 || repeat > MAX_REPEAT) {
                fprintf(stderr, "--repeat must be 1-%d\n", MAX_REPEAT);
                return 1;
            }
        } else if (strcmp(argv[i], "--vrr-min") == 0) {
            vrr_min_hz = atof(argv[i + 1]);
        } else if (strcmp(argv[i], "--log") == 0) {
            log_file = fopen(argv[i + 1], "w");
            if (!log_file) {
                fprintf(stderr, "Could not open %s for writing\n", argv[i + 1]);
            } else {
                fprintf(log_file,
                        "present,time_ms,interval_ms,pace_hz,repeat,bfi,vsync,work_ms,wake_ms,"
                        "present_ms\n");
            }
        }
    }
    if (bfi && repeat < 2) {
        repeat = 2;
    }
    double vrr_max_ms = vrr_min_hz > 0.0 ? 1000.0 / vrr_min_hz : 0.0;
    if (vrr_min_hz > 0.0) {
        printf("VRR minimum: %.3f Hz (longest interval %.3f ms)\n", vrr_min_hz, vrr_max_ms);
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
    uint64_t next_at_ns = 0;  // Absolute pacing timeline: when the next present is due
    double pos = 0.0;         // Position of the bar in the rendered frame, in pixels
    int sub = 0;              // Which present of the current frame is next (0 .. repeat - 1)

    printf("Pacing: %s, repeat %d%s\n",
           pace_rates[pace_index] > 0.0 ? "fixed rate" : "free-running", repeat,
           bfi ? " with black frame insertion" : "");
    uint64_t present_no = 0;
    uint64_t work_start_ns = 0;  // When the previous present returned

    render_frame(renderer, pos, false);

    while (running) {
        double pace_hz = pace_rates[pace_index];

        // 1. Wait until the present is due, on an absolute timeline: sleep most of the time,
        // spin the last bit.
        double work_ms = 0.0;
        if (work_start_ns != 0) {
            work_ms = (double)(SDL_GetTicksNS() - work_start_ns) / 1e6;
            if (work_ms > stats.max_work_ms) {
                stats.max_work_ms = work_ms;
            }
        }
        double wake_ms = 0.0;
        if (pace_hz > 0.0) {
            uint64_t period_ns = (uint64_t)(1e9 / (pace_hz * repeat));
            uint64_t now = SDL_GetTicksNS();
            if (next_at_ns == 0 || now > next_at_ns + 2 * period_ns) {
                next_at_ns = now;  // First frame, or too far behind: re-base the timeline
            }
            while ((now = SDL_GetTicksNS()) < next_at_ns) {
                uint64_t left = next_at_ns - now;
                if (left > SPIN_NS) {
                    SDL_DelayNS(left - SPIN_NS);
                }
            }
            wake_ms = (double)(now - next_at_ns) / 1e6;
            if (wake_ms > stats.max_wake_ms) {
                stats.max_wake_ms = wake_ms;
            }
            next_at_ns += period_ns;
        } else {
            next_at_ns = 0;
        }

        // 2. Present the frame rendered at the end of the previous iteration.
        uint64_t call_ns = SDL_GetTicksNS();
        SDL_RenderPresent(renderer);
        uint64_t now_ns = SDL_GetTicksNS();
        double present_ms = (double)(now_ns - call_ns) / 1e6;
        if (present_ms > stats.max_present_ms) {
            stats.max_present_ms = present_ms;
        }
        work_start_ns = now_ns;

        // 3. Statistics and input, in the time left until the next present.
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
                fprintf(log_file, "%llu,%.3f,%.3f,%.3f,%d,%d,%d,%.3f,%.3f,%.3f\n",
                        (unsigned long long)present_no, (double)(now_ns - start_ns) / 1e6, ms,
                        pace_hz, repeat, bfi ? 1 : 0, vsync, work_ms, wake_ms, present_ms);
            }
            double expected_ms = pace_hz > 0.0
                                     ? 1000.0 / (pace_hz * repeat)
                                     : (display_hz > 0.0 ? 1000.0 / display_hz : 1000.0 / 60.0);
            if (ms > expected_ms * 1.5) {
                stats.late++;
            }
            if (vrr_max_ms > 0.0 && ms > vrr_max_ms) {
                stats.vrr_miss++;
            }
        }
        last_present_ns = now_ns;
        present_no++;

        if (now_ns - last_report_ns >= 1000000000ULL) {
            if (stats.count > 0) {
                double avg = stats.sum_ms / stats.count;
                double var = stats.sum_sq_ms / stats.count - avg * avg;
                double stdev = var > 0.0 ? sqrt(var) : 0.0;
                char pace[32];
                if (pace_hz > 0.0) {
                    snprintf(pace, sizeof(pace), "%.2f Hz x%d%s", pace_hz, repeat,
                             bfi ? " bfi" : "");
                } else {
                    snprintf(pace, sizeof(pace), "free x%d%s", repeat, bfi ? " bfi" : "");
                }
                char vrr[32] = "";
                if (vrr_max_ms > 0.0) {
                    snprintf(vrr, sizeof(vrr), " | vrr miss %d", stats.vrr_miss);
                }
                char line[400];
                snprintf(line, sizeof(line),
                         "pace %s | %s | vsync %s | %.2f fps | interval ms avg %.3f stdev %.3f "
                         "min %.3f max %.3f | late %d%s | work max %.3f | wake max %.3f | "
                         "present max %.3f ms",
                         pace, mode_names[mode], vsync_name(vsync), 1000.0 / avg, avg, stdev,
                         stats.min_ms, stats.max_ms, stats.late, vrr, stats.max_work_ms,
                         stats.max_wake_ms, stats.max_present_ms);
                printf("%s\n", line);
                SDL_SetWindowTitle(window, line);
            }
            stats_reset(&stats);
            last_report_ns = now_ns;
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
                } else if (key == SDLK_R) {
                    repeat = repeat % MAX_REPEAT + 1;
                    if (bfi && repeat < 2) {
                        repeat = 2;
                    }
                    sub = 0;
                    printf("Repeat: %d\n", repeat);
                    stats_reset(&stats);
                    last_present_ns = 0;
                    next_at_ns = 0;
                } else if (key == SDLK_B) {
                    bfi = !bfi;
                    if (bfi && repeat < 2) {
                        repeat = 2;
                    }
                    sub = 0;
                    printf("Black frame insertion: %s, repeat %d\n", bfi ? "on" : "off", repeat);
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
        if (!running) {
            break;
        }

        // 4. Render the next frame (after input, so that changes show in it). The bar moves on
        // the first present of each frame and stays put for the repeats, which are black with
        // black frame insertion. The backbuffer is undefined after a present, so a repeat is
        // rendered again.
        pace_hz = pace_rates[pace_index];
        double travel = travel_for(renderer);
        if (++sub >= repeat) {
            sub = 0;
        }
        if (sub == 0) {
            if (mode == MODE_PER_FRAME) {
                // Fixed step per frame, independent of when the frame is actually presented
                double frame_hz = pace_hz > 0.0
                                      ? pace_hz
                                      : (display_hz > 0.0 ? display_hz : 60.0) / repeat;
                pos += speed / frame_hz;
            } else {
                // Position at the time the frame is due (now, when free-running)
                uint64_t due_ns = next_at_ns != 0 ? next_at_ns : SDL_GetTicksNS();
                pos = (double)(due_ns - start_ns) / 1e9 * speed;
            }
            pos = fmod(pos, travel);
        }
        render_frame(renderer, pos, bfi && sub != 0);
    }

    if (log_file) {
        fclose(log_file);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
