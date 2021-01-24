#include "sysconfig.h"
#include "sysdeps.h"

#include "uae.h"
#include "gui.h"
#include "uae/fs.h"

amiga_led_function g_amiga_led_function = NULL;
struct _uae_led_data g_uae_led_data = {};

void od_fs_update_leds(void) {
    for (int i = 0; i < 4; i++) {
        int track = gui_data.drive_track[i];
        int a = 0;
        int b = 0;
        if (!gui_data.drive_disabled[i]) {
            a = track / 10;
            b = track % 10;
            if (a == 0) {
                a = 10;
            }
            if (b == 0) {
                b = 10;
            }
        }
        g_uae_led_data.df_t1[i] = a;
        g_uae_led_data.df_t0[i] = b;
    }
    uae_callback(uae_on_update_leds, &g_uae_led_data);
}

static void gui_flicker_led2 (int led, int unitnum, int status)
{
    static int resetcounter[LED_MAX];
    static uae_s8 gui_data_hd, gui_data_cd, gui_data_md;
    uae_s8 old;
    uae_s8 *p;

    if (led == LED_HD)
            p = &gui_data_hd;
    else if (led == LED_CD)
            p = &gui_data_cd;
    else if (led == LED_MD)
            p = &gui_data_md;
    else
            return;
    old = *p;
    if (status < 0) {
            if (old < 0) {
                    *p = 0;
                    gui_led (led, 0, -1);
            }
            return;
    }
    if (status == 0 && old < 0) {
           resetcounter[led] = 0;
            return;
    }
    if (status == 0) {
            resetcounter[led]--;
            if (resetcounter[led] > 0)
                    return;
    }
    *p = status;
    resetcounter[led] = 6;
    if (old != *p)
            gui_led (led, *p, -1);
}

void gui_flicker_led (int led, int unitnum, int status) {
    if (led < 0) {
        gui_flicker_led2 (LED_HD, 0, 0);
        gui_flicker_led2 (LED_CD, 0, 0);
        gui_flicker_led2 (LED_MD, 0, 0);
    }
    else {
        gui_flicker_led2 (led, unitnum, status);
    }
}

void gui_led (int led, int state, int brightness)
{
    //STUB("led %d state %d", led, state);
    int out_led = -1;
    int out_state = state;

    if (led == LED_DF0) out_led = 0;
    else if (led == LED_DF1) out_led = 1;
    else if (led == LED_DF2) out_led = 2;
    else if (led == LED_DF3) out_led = 3;
    else if (led == LED_POWER) {
        //printf("POWER %d b %d\n", state, gui_data.powerled_brightness);
        printf("POWER LED %d b %d\n", state, brightness);
        out_led = 8;
    }
    else if (led == LED_HD) out_led = 9;
    else if (led == LED_CD) {
        out_led = 10;
        if (state == 0) {
            out_state = 0;
        }
        else if (state == 4) {
            out_state = 2;
        }
        else {
            out_state = 1;
        }
    }
    else if (led == LED_MD) out_led = 11;

    if (led >= LED_DF0 && led <= LED_DF3) {
        if (gui_data.drive_writing[led - 1]) {
            out_state = 2;
        }
    }

    // if (g_amiga_led_function && out_led > -1) {
    //     g_amiga_led_function(out_led, out_state);
    // }

    if (g_amiga_led_function) {
        if (brightness == -1) {
            brightness = 100;
        } else {
            brightness = 100 * brightness / 255;
        }
        g_amiga_led_function(led, out_state, brightness);
    }
}

extern "C" {

void amiga_on_update_leds(amiga_callback_function *function) {
    uae_on_update_leds = function;
}

void amiga_set_led_function(amiga_led_function function) {
    g_amiga_led_function = function;
}

} // extern "C"
