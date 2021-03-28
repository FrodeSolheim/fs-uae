#ifndef FS_UAE_OD_FS_CALLBACKS_H
#define FS_UAE_OD_FS_CALLBACKS_H

typedef uintptr_t (uae_callback_function)(void *data);

static inline void uae_callback(uae_callback_function function,
	    const void *data) {
    if (function) {
        function((void *) data);
    }
}

extern uae_callback_function *uae_on_save_state_finished;
extern uae_callback_function *uae_on_restore_state_finished;

extern uae_callback_function *uae_on_update_leds;

#endif // FS_UAE_OD_FS_CALLBACKS_H
