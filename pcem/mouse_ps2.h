void *mouse_ps2_init();
void mouse_ps2_poll(int x, int y, int z, int b, void *p);
void *mouse_intellimouse_init();

extern mouse_t mouse_ps2_2_button;
extern mouse_t mouse_intellimouse;
