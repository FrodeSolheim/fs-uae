#define FSUAE_INTERNAL
#include "fsuae-oskeyboard.h"

#include "fsuae-action.h"
#include "fsuae.h"

static void fsuae_oskeyboard_configure_default(fsemu_oskeyboard_t *keyboard)
{
    fsemu_oskeyboard_row_t *row;
    fsemu_oskeyboard_key_t *key;

    // Bottom row

    row = fsemu_oskeyboard_new_row(keyboard);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "LALT");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_LALT);
    fsemu_oskeyboard_key_set_left_margin(key, 75);
    fsemu_oskeyboard_key_set_width(key, 125);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "LAMIGA");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_LAMIGA);
    fsemu_oskeyboard_key_set_width(key, 125);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "SPACE");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_SPACE);
    fsemu_oskeyboard_key_set_width(key, 900);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "RAMIGA");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_RAMIGA);
    fsemu_oskeyboard_key_set_width(key, 125);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "RALT");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_RALT);
    fsemu_oskeyboard_key_set_width(key, 125);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "KP0");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_KP0);
    fsemu_oskeyboard_key_set_left_margin(key, 475);
    fsemu_oskeyboard_key_set_width(key, 200);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "KPPERIOD");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_KPPERIOD);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "KPENTER");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_KPENTER);
    fsemu_oskeyboard_key_set_height(key, 200);

    // Bottom letter row

    row = fsemu_oskeyboard_new_row(keyboard);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "LSHIFT");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_LSHIFT);
    fsemu_oskeyboard_key_set_width(key, 175);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "30");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_30);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "Z");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_Z);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "X");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_X);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "C");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_C);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "V");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_V);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "B");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_B);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "N");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_N);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "M");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_M);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "COMMA");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_COMMA);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "PERIOD");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_PERIOD);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "SLASH");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_SLASH);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "RSHIFT");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_RSHIFT);
    fsemu_oskeyboard_key_set_width(key, 275);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "LEFT");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_LEFT);
    fsemu_oskeyboard_key_set_left_margin(key, 50);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "DOWN");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_DOWN);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "RIGHT");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_RIGHT);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "KP1");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_KP1);
    fsemu_oskeyboard_key_set_left_margin(key, 50);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "KP2");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_KP2);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "KP3");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_KP3);

    // Middle letter row

    row = fsemu_oskeyboard_new_row(keyboard);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "CTRL");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_CTRL);
    fsemu_oskeyboard_key_set_width(key, 125);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "CAPSLOCK");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_CAPSLOCK);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "A");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_A);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "S");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_S);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "D");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_D);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "F");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_F);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "G");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_G);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "H");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_H);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "J");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_J);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "K");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_K);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "L");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_L);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "SEMICOLON");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_SEMICOLON);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "HASH");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_HASH);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "2B");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_2B);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "RETURN");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_RETURN);
    fsemu_oskeyboard_key_set_width(key, 125);
    fsemu_oskeyboard_key_set_height(key, 200);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "UP");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_UP);
    fsemu_oskeyboard_key_set_left_margin(key, 150);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "KP4");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_KP4);
    fsemu_oskeyboard_key_set_left_margin(key, 150);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "KP5");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_KP5);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "KP6");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_KP6);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "KPPLUS");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_KPPLUS);

    // Top letter row

    row = fsemu_oskeyboard_new_row(keyboard);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "TAB");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_TAB);
    fsemu_oskeyboard_key_set_width(key, 200);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "Q");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_Q);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "W");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_W);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "E");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_E);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "R");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_R);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "T");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_T);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "Y");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_Y);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "U");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_U);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "I");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_I);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "O");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_O);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "P");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_P);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "LBRACKET");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_LBRACKET);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "RBRACKET");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_RBRACKET);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "KP7");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_KP7);
    fsemu_oskeyboard_key_set_left_margin(key, 550);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "KP8");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_KP8);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "KP9");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_KP9);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "KPMINUS");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_KPMINUS);

    // Numbers row

    row = fsemu_oskeyboard_new_row(keyboard);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "BACKTICK");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_BACKTICK);
    fsemu_oskeyboard_key_set_width(key, 150);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "1");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_1);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "2");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_2);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "3");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_3);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "4");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_4);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "5");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_5);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "6");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_6);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "7");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_7);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "8");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_8);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "9");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_9);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "0");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_0);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "MINUS");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_MINUS);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "EQUALS");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_EQUALS);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "BACKSLASH");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_BACKSLASH);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "BACKSPACE");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_BACKSPACE);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "DEL");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_DEL);
    fsemu_oskeyboard_key_set_left_margin(key, 50);
    fsemu_oskeyboard_key_set_width(key, 150);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "HELP");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_HELP);
    fsemu_oskeyboard_key_set_width(key, 150);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "KPLPAREN");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_KPLPAREN);
    fsemu_oskeyboard_key_set_left_margin(key, 50);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "KPRPAREN");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_KPRPAREN);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "KPDIV");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_KPDIV);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "KPMUL");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_KPMUL);

    // Function key row

    row = fsemu_oskeyboard_new_row(keyboard);
    fsemu_oskeyboard_row_set_bottom_margin(row, 50);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "ESCAPE");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_ESCAPE);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "F1");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_F1);
    fsemu_oskeyboard_key_set_left_margin(key, 50);
    fsemu_oskeyboard_key_set_width(key, 125);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "F2");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_F2);
    fsemu_oskeyboard_key_set_width(key, 125);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "F3");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_F3);
    fsemu_oskeyboard_key_set_width(key, 125);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "F4");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_F4);
    fsemu_oskeyboard_key_set_width(key, 125);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "F5");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_F5);
    fsemu_oskeyboard_key_set_width(key, 125);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "F6");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_F6);
    fsemu_oskeyboard_key_set_left_margin(key, 50);
    fsemu_oskeyboard_key_set_width(key, 125);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "F7");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_F7);
    fsemu_oskeyboard_key_set_width(key, 125);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "F8");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_F8);
    fsemu_oskeyboard_key_set_width(key, 125);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "F9");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_F9);
    fsemu_oskeyboard_key_set_width(key, 125);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "F10");
    fsemu_oskeyboard_key_set_action(key, ACTION_KEY_F10);
    fsemu_oskeyboard_key_set_width(key, 125);
}

void fsuae_oskeyboard_init(void)
{
    fsuae_log("fsuae_oskeyboard_init\n");
    fsemu_oskeyboard_t *keyboard;
    // fsemu_oskeyboard_layout_t *layout;
    // fsemu_oskeyboard_init();

    // FIXME: Auto-set as active keyboard?
    keyboard = fsemu_oskeyboard_new();

    // FIXME: Auto-set as active layout?
    // layout = fsemu_oskeyboard_new_layout();
    // fsemu_oskeyboard_add_layout(keyboard, layout);

    fsemu_oskeyboard_add(keyboard);
    fsemu_oskeyboard_set_active(keyboard);

    fsuae_oskeyboard_configure_default(keyboard);
}
