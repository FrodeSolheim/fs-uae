#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>
#include <string.h>

#include <uae/uae.h>
#include <fs/emu.h>
#include <fs/emu/actions.h>
#include "fs-uae.h"
#include "config-common.h"

fs_uae_input_port g_fs_uae_input_ports[FS_UAE_NUM_INPUT_PORTS] = {};

#define NO_ACTION 65536

#define COMMON_ACTIONS \
    { "menu_alt", FS_EMU_ACTION_MENU_ALT }, \
    { "pause", FS_EMU_ACTION_PAUSE }

#define CUSTOM_ACTIONS \
    { "left", NO_ACTION }, \
    { "right", NO_ACTION }, \
    { "up", NO_ACTION }, \
    { "down", NO_ACTION }, \
    { "1", NO_ACTION }, \
    { "2", NO_ACTION }

#define END_ACTIONS { NULL, 0 }

static fs_emu_input_mapping g_joystick_port_0_mapping[] = {
    { "left", INPUTEVENT_JOY1_LEFT },
    { "right", INPUTEVENT_JOY1_RIGHT },
    { "up", INPUTEVENT_JOY1_UP },
    { "down", INPUTEVENT_JOY1_DOWN },
    { "1", INPUTEVENT_JOY1_FIRE_BUTTON },
    { "2", INPUTEVENT_JOY1_2ND_BUTTON },
    { "3", INPUTEVENT_JOY1_3RD_BUTTON },
    { "red", INPUTEVENT_JOY1_CD32_RED },
    { "yellow", INPUTEVENT_JOY1_CD32_YELLOW },
    { "green", INPUTEVENT_JOY1_CD32_GREEN },
    { "blue", INPUTEVENT_JOY1_CD32_BLUE },
    { "rewind", INPUTEVENT_JOY1_CD32_RWD },
    { "forward", INPUTEVENT_JOY1_CD32_FFW },
    { "play", INPUTEVENT_JOY1_CD32_PLAY },
    { "autofire", INPUTEVENT_JOY1_AUTOFIRE_BUTTON },
    { "toggle_autofire", INPUTEVENT_AMIGA_JOYPORT_0_AUTOFIRE },
    COMMON_ACTIONS,
    END_ACTIONS
};

static fs_emu_input_mapping g_joystick_port_1_mapping[] = {
    { "left", INPUTEVENT_JOY2_LEFT },
    { "right", INPUTEVENT_JOY2_RIGHT },
    { "up", INPUTEVENT_JOY2_UP },
    { "down", INPUTEVENT_JOY2_DOWN },
    { "1", INPUTEVENT_JOY2_FIRE_BUTTON },
    { "2", INPUTEVENT_JOY2_2ND_BUTTON },
    { "3", INPUTEVENT_JOY2_3RD_BUTTON },
    { "red", INPUTEVENT_JOY2_CD32_RED },
    { "yellow", INPUTEVENT_JOY2_CD32_YELLOW },
    { "green", INPUTEVENT_JOY2_CD32_GREEN },
    { "blue", INPUTEVENT_JOY2_CD32_BLUE },
    { "rewind", INPUTEVENT_JOY2_CD32_RWD },
    { "forward", INPUTEVENT_JOY2_CD32_FFW },
    { "play", INPUTEVENT_JOY2_CD32_PLAY },
    { "autofire", INPUTEVENT_JOY2_AUTOFIRE_BUTTON },
    { "toggle_autofire", INPUTEVENT_AMIGA_JOYPORT_1_AUTOFIRE },
    COMMON_ACTIONS,
    END_ACTIONS
};

static fs_emu_input_mapping g_parallel_port_1st_mapping[] = {
    { "left", INPUTEVENT_PAR_JOY1_LEFT },
    { "right", INPUTEVENT_PAR_JOY1_RIGHT },
    { "up", INPUTEVENT_PAR_JOY1_UP },
    { "down", INPUTEVENT_PAR_JOY1_DOWN },
    { "1", INPUTEVENT_PAR_JOY1_FIRE_BUTTON },
    { "2", INPUTEVENT_PAR_JOY1_2ND_BUTTON },
    { "autofire", INPUTEVENT_PAR_JOY1_AUTOFIRE_BUTTON },
    { "toggle_autofire", INPUTEVENT_AMIGA_JOYPORT_2_AUTOFIRE },
    COMMON_ACTIONS,
    END_ACTIONS
};

static fs_emu_input_mapping g_parallel_port_2nd_mapping[] = {
    { "left", INPUTEVENT_PAR_JOY2_LEFT },
    { "right", INPUTEVENT_PAR_JOY2_RIGHT },
    { "up", INPUTEVENT_PAR_JOY2_UP },
    { "down", INPUTEVENT_PAR_JOY2_DOWN },
    { "1", INPUTEVENT_PAR_JOY2_FIRE_BUTTON },
    { "2", INPUTEVENT_PAR_JOY2_2ND_BUTTON },
    { "autofire", INPUTEVENT_PAR_JOY2_AUTOFIRE_BUTTON },
    { "toggle_autofire", INPUTEVENT_AMIGA_JOYPORT_3_AUTOFIRE },
    COMMON_ACTIONS,
    END_ACTIONS
};

static fs_emu_input_mapping g_port_4_mapping[] = {
    CUSTOM_ACTIONS,
    END_ACTIONS
};

static fs_emu_input_mapping g_port_5_mapping[] = {
    CUSTOM_ACTIONS,
    END_ACTIONS
};

static fs_emu_input_mapping g_port_6_mapping[] = {
    CUSTOM_ACTIONS,
    END_ACTIONS
};

static fs_emu_input_mapping g_port_7_mapping[] = {
    CUSTOM_ACTIONS,
    END_ACTIONS
};

static fs_emu_input_mapping *g_joystick_mappings[] = {
    g_joystick_port_0_mapping,
    g_joystick_port_1_mapping,
    g_parallel_port_1st_mapping,
    g_parallel_port_2nd_mapping,
    /* Custom ports (maps to UAE actions, if configured) */
    g_port_4_mapping,
    g_port_5_mapping,
    g_port_6_mapping,
    g_port_7_mapping,
};

void fs_uae_read_override_actions_for_port(int port)
{
    fs_log("fs_uae_read_override_actions_for_port %d\n", port);
    fs_emu_input_mapping *mapping = g_joystick_mappings[port];
    for (int i = 0; mapping[i].name != NULL; i++) {
        const char* name = mapping[i].name;
        if (strcmp(name, "1") == 0) {
            name = "primary";
        } else if (strcmp(name, "2") == 0) {
            name = "secondary";
        }
        char *key = g_strdup_printf("joystick_port_%d_%s", port, name);
        const char *value = fs_config_get_const_string(key);
        if (value == NULL) {
            continue;
        }
        fs_log("check %s = %s\n", key, value);
        int action = fs_emu_input_action_from_string(value);
        if (action > -1) {
            fs_log("override %s => %s (%d)\n", key, value, action);
            mapping[i].action = action;
        }
        free(key);
    }
}

static void map_mouse(const char *device_name, int port)
{
    fs_log("mapping mouse to port %d\n", port);
    if (port == 0) {
        fs_emu_configure_mouse(device_name, INPUTEVENT_MOUSE1_HORIZ,
                INPUTEVENT_MOUSE1_VERT, INPUTEVENT_JOY1_FIRE_BUTTON,
                INPUTEVENT_JOY1_3RD_BUTTON, INPUTEVENT_JOY1_2ND_BUTTON,
                INPUTEVENT_MOUSE1_WHEEL);
    } else if (port == 1) {
        fs_emu_configure_mouse(device_name, INPUTEVENT_MOUSE2_HORIZ,
                INPUTEVENT_MOUSE2_VERT, INPUTEVENT_JOY2_FIRE_BUTTON,
                INPUTEVENT_JOY2_3RD_BUTTON, INPUTEVENT_JOY2_2ND_BUTTON,
                0);
    }
    else {
        fs_log("WARNING: cannot map mouse to this port\n");
    }
}

static void auto_joystick(
    fs_uae_input_port *p, int port, int mode, const char *type)
{
    fs_emu_log("trying to auto-configure joystick in port %d\n", port);
    p->new_mode = mode;
    int result = fs_emu_configure_joystick(
            "JOYSTICK", type, g_joystick_mappings[port], 1,
            g_fs_uae_input_ports[port].device, MAX_DEVICE_NAME_LEN, false);
    if (!result) {
        result = fs_emu_configure_joystick(
            "JOYSTICK #2", type, g_joystick_mappings[port], 1,
            g_fs_uae_input_ports[port].device, MAX_DEVICE_NAME_LEN, false);
    }
    if (!result) {
        fs_emu_log("could not auto-configure joystick,"
                   "using keyboard emulation\n");
        strcpy(p->device, "KEYBOARD");
    }
}

static void configure_joystick_port(
    int port, const char *value, const char *port_name, const char *joy_dev)
{
    fs_emu_log("configuring joystick port %d (%s)\n", port, value);
    fs_uae_input_port *p = g_fs_uae_input_ports + port;

    const char *auto_type = "amiga";
    int auto_mode = AMIGA_JOYPORT_DJOY;
    if (g_fs_uae_amiga_model == MODEL_CD32) {
        auto_type = "cd32";
        auto_mode = AMIGA_JOYPORT_CD32JOY;
    }
    char *key = g_strdup_printf("joystick_port_%d_mode", port);
    const char *mode_string = fs_config_get_const_string(key);
    free(key);

    if (g_ascii_strcasecmp(value, "nothing") == 0
            || g_ascii_strcasecmp(value, "none") == 0) {
        fs_emu_log("nothing connected to port\n");
        strcpy(p->device, "");
        p->new_mode = AMIGA_JOYPORT_NONE;
    } else if (g_ascii_strcasecmp(value, "auto") == 0 && port < 2) {
        if (port == 0) {
            if (!mode_string
                    || g_ascii_strcasecmp(mode_string, "mouse") == 0) {
                p->new_mode = AMIGA_JOYPORT_MOUSE;
                strcpy(p->device, "MOUSE");
            } else if (g_ascii_strcasecmp(mode_string, "joystick") == 0) {
                auto_joystick(p, port, AMIGA_JOYPORT_DJOY, "amiga");
            } else if (g_ascii_strcasecmp(mode_string, "cd32 gamepad") == 0) {
                auto_joystick(p, port, AMIGA_JOYPORT_CD32JOY, "cd32");
            }
        } else {
            auto_joystick(p, port, auto_mode, auto_type);
#if 0
            fs_emu_log("trying to auto-configure joystick 1 in port 1\n");
            p->new_mode = auto_mode;
            int result = fs_emu_configure_joystick("JOYSTICK", auto_type,
                    g_joystick_mappings[port], 1,
                    g_fs_uae_input_ports[port].device, MAX_DEVICE_NAME_LEN);
            if (!result) {
                fs_emu_log("could not auto-configure joystick 1, "
                        "using keyboard emulation\n");
                strcpy(p->device, "KEYBOARD");
            }
#endif
        }
    } else if (g_ascii_strcasecmp(value, "dummy mouse") == 0) {
        p->new_mode = AMIGA_JOYPORT_MOUSE;
    } else if (g_ascii_strcasecmp(value, "dummy joystick") == 0) {
        p->new_mode = AMIGA_JOYPORT_DJOY;
    } else if (g_ascii_strcasecmp(value, "mouse") == 0) {
        strcpy(p->device, "MOUSE");
        p->new_mode = AMIGA_JOYPORT_MOUSE;
    }
    // deprecated
    else if (g_ascii_strcasecmp(value, "amiga_mouse") == 0) {
        p->new_mode = AMIGA_JOYPORT_MOUSE;
    }
    // deprecated
    else if (g_ascii_strcasecmp(value, "amiga_joystick") == 0) {
        p->new_mode = AMIGA_JOYPORT_DJOY;
    } else {
        p->new_mode = auto_mode;
        fs_emu_configure_joystick(value, auto_type,
                g_joystick_mappings[port], 1,
                p->device, MAX_DEVICE_NAME_LEN, false);
    }

    if (mode_string) {
        char *mode_lower = g_ascii_strdown(mode_string, -1);
        if (strcmp(mode_lower, "joystick") == 0) {
            p->new_mode = AMIGA_JOYPORT_DJOY;
        } else if (strcmp(mode_lower, "mouse") == 0) {
            p->new_mode = AMIGA_JOYPORT_MOUSE;
        } else if (strcmp(mode_lower, "cd32 gamepad") == 0) {
            p->new_mode = AMIGA_JOYPORT_CD32JOY;
        } else if (strcmp(mode_lower, "nothing") == 0) {
            p->new_mode = AMIGA_JOYPORT_NONE;
        } else if (strcmp(mode_lower, "none") == 0) {
            p->new_mode = AMIGA_JOYPORT_NONE;
        } else if (strcmp(mode_lower, "custom") == 0) {
            // FIXME: custom is not fully implemented as its own type
            p->new_mode = AMIGA_JOYPORT_DJOY;
        } else {
            fs_log("unknown joystick port mode: %s\n", mode_lower);
        }
        free(mode_lower);
    }

    if (port < 4) {
        // port 4 is "custom joystick"
        key = g_strdup_printf("joystick_port_%d_autofire", port);
        if (fs_config_get_boolean(key) == 1) {
            p->new_autofire_mode = 1;
            p->autofire_mode = 1;
            amiga_set_joystick_port_autofire(port, 1);
        }
        free(key);
    } else {
        /* This is a fake joystick, can be used to map keyboard pressed
         * for example, mode is not set throught input actions, since
         * this need not be synchronized in net play. */
        p->mode = p->new_mode;
    }
}

void fs_uae_configure_input()
{
    fs_uae_configure_mouse();
    fs_emu_log("configuring joystick ports:\n");
    amiga_set_option("joyport0", "none");
    amiga_set_option("joyport1", "none");

    fs_uae_configure_actions();

    char *value;
    value = fs_config_get_string("joystick_port_1");
    if (!value) {
        value = g_strdup("auto");
    }
    configure_joystick_port(1, value, "joyport1", "joy1");
    g_free(value);
    value = fs_config_get_string("joystick_port_0");
    if (!value) {
        value = g_strdup("auto");
    }
    configure_joystick_port(0, value, "joyport0", "joy0");
    g_free(value);

    for (int i = 2; i < FS_UAE_NUM_INPUT_PORTS; i++) {
        gchar *key = g_strdup_printf("joystick_port_%d", i);
        value = fs_config_get_string(key);
        g_free(key);
        if (value == NULL) {
            value = g_strdup("nothing");
        }
        gchar *key2 = g_strdup_printf("joyport%d", i);
        gchar *key3 = g_strdup_printf("joy%d", i);
        configure_joystick_port(i, value, key2, key3);
        g_free(key3);
        g_free(key2);
        g_free(value);
    }
}

void fs_uae_reconfigure_input_ports_amiga()
{
    fs_emu_log("fs_uae_reconfigure_input_ports_amiga\n");
    int modes = INPUTEVENT_AMIGA_JOYPORT_MODE_0_LAST -
            INPUTEVENT_AMIGA_JOYPORT_MODE_0_NONE + 1;
    //for (int i = 0; i < FS_UAE_NUM_INPUT_PORTS; i++) {
    // only the 4 real ports are reconfigured via input events, the 5th
    // custom joystick port is a local virtual joystick for mapping custom
    // input events only
    for (int i = 0; i < 4; i++) {
        fs_uae_input_port *port = g_fs_uae_input_ports + i;
        if (port->new_mode != port->mode) {
            fs_log("sending event to set port %d to mode %d\n", i, port->new_mode);
            int action = INPUTEVENT_AMIGA_JOYPORT_MODE_0_NONE + modes * i + \
                    port->new_mode;
            fs_emu_queue_action(action, 1);
        }
        if (port->new_autofire_mode != port->autofire_mode) {
            fs_log("sending event to set port %d to autofire mode %d\n", i,
                    port->new_autofire_mode);
            int action = INPUTEVENT_AMIGA_JOYPORT_0_AUTOFIRE + i;
            fs_emu_queue_action(action, 1);
        }
    }
}

void fs_uae_reconfigure_input_ports_host()
{
    fs_emu_log("fs_uae_reconfigure_input_ports_host\n");
    fs_emu_reset_input_mapping();
    fs_uae_map_keyboard();

    int mouse_mapped_to_port = -1;

    for (int i = 0; i < FS_UAE_NUM_INPUT_PORTS; i++) {
        fs_uae_input_port *port = g_fs_uae_input_ports + i;

        fs_log("configuring joystick port %d\n", i);
        if (port->mode == AMIGA_JOYPORT_NONE) {
            fs_log("* nothing in port\n");
            fs_log("* FIXME\n");
        }
        else if (port->mode == AMIGA_JOYPORT_MOUSE) {
            fs_log("* amiga mouse\n");
            // if (strcmp(port->device, "MOUSE") == 0) {
                // fs_log("* using host mouse\n");
                fs_log("* using device %s\n", port->device);
                map_mouse(port->device, i);
                mouse_mapped_to_port = i;
            // }
            // else {
            //     fs_log("* not mapping host device to amiga mouse\n");
            // }
        }
        else if (port->mode == AMIGA_JOYPORT_DJOY) {
            fs_log("* amiga joystick\n");
            if (strcmp(port->device, "MOUSE") == 0) {
                fs_log("* cannot map mouse to joystick\n");
            }
            else {
                fs_log("* using device %s\n", port->device);
                fs_emu_configure_joystick(port->device, "amiga",
                        g_joystick_mappings[i], 1, NULL, 0, true);
            }
        }
        else if (port->mode == AMIGA_JOYPORT_CD32JOY) {
            fs_log("* amiga cd32 gamepad\n");
            if (strcmp(port->device, "MOUSE") == 0) {
                fs_log("* cannot map mouse to cd32 gamepad\n");
            }
            else {
                fs_log("* using device %s\n", port->device);
                fs_emu_configure_joystick(port->device, "cd32",
                        g_joystick_mappings[i], 1, NULL, 0, true);
            }
        }
    }

    fs_uae_input_port *port0 = g_fs_uae_input_ports;

#if 0
    int autoswitch = fs_config_get_boolean("joystick_port_0_autoswitch");
    if (autoswitch == FS_CONFIG_NONE) {
        autoswitch = 0;
    }

    if (!autoswitch) {
        if (mouse_mapped_to_port == -1 && port0->mode != AMIGA_JOYPORT_NONE) {
            // there is a device in port 0, but mouse is not use in either
            // port
            fs_log("additionally mapping mouse buttons to port 0\n");
            fs_emu_configure_mouse("MOUSE", 0, 0, INPUTEVENT_JOY1_FIRE_BUTTON,
                    0, INPUTEVENT_JOY1_2ND_BUTTON, 0);
        }
    }
#endif

    if (mouse_mapped_to_port == -1 && port0->mode == AMIGA_JOYPORT_DJOY) {
        fs_log("additionally mapping mouse to port 0\n");
        map_mouse("mouse", 0);
    }

#if 0
    if (autoswitch && !fs_emu_netplay_enabled()) {
        // auto-select for other devices when not in netplay mode

        if (mouse_mapped_to_port == -1) {
            // FIXME: device "9" is a bit of a hack here, should promote
            fs_emu_configure_mouse("MOUSE",
                    0, 0, INPUTEVENT_AMIGA_JOYPORT_0_DEVICE_9, 0, 0, 0);
        }

        int count;
        fs_emu_input_device *input_device = fs_emu_get_input_devices(&count);
        for (int i = 0; i < count; i++) {
            //printf("---- %d %s\n", i, input_device->name);
            //printf("usage: %d\n", input_device->usage);
            if (input_device->usage) {
                // this device is used, so we don't want auto-select for this
                // device
                input_device++;
                continue;
            }

            if (strcmp(input_device->name, "KEYBOARD") == 0) {
                continue;
            }

            int input_event = INPUTEVENT_AMIGA_JOYPORT_0_DEVICE_0 + i;
            fs_emu_input_mapping mapping[] = {
                { "1", input_event },
                { NULL, 0 },
            };
            fs_emu_configure_joystick(input_device->name, "amiga",
                    mapping, 0, NULL, 0);
            input_device++;
        };
    }
#endif

    fs_emu_map_custom_actions();
}
