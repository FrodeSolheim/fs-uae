#include "fsapp-channel.h"

#include <Python.h>
#include <SDL3/SDL.h>
#include <glib.h>

#include "fsemu-util.h"

typedef struct fsapp_channel_message {
    int type;
    char* data;
} fsapp_channel_message_t;

struct fsapp_channel {
    SDL_Mutex* mutex;
    GList* messages;
    fsapp_channel_message_t* read_message;
};

void fsapp_channel_lock(fsapp_channel_t* channel) {
    SDL_LockMutex(channel->mutex);
}

void fsapp_channel_unlock(fsapp_channel_t* channel) {
    SDL_UnlockMutex(channel->mutex);
}

void fsapp_channel_add_message(fsapp_channel_t* channel, int type, const char* data) {
    fsapp_channel_message_t* message = FSEMU_UTIL_MALLOC0(fsapp_channel_message_t);
    message->type = type;
    message->data = g_strdup(data);

    fsapp_channel_lock(channel);
    channel->messages = g_list_append(channel->messages, message);
    fsapp_channel_unlock(channel);
}

bool fsapp_channel_next_message(fsapp_channel_t* channel, int* type, const char** data) {
    fsapp_channel_message_t* message = channel->read_message;

    if (message) {
        g_free(message->data);
        free(message);
        channel->read_message = NULL;
    }

    if (channel->messages) {
        message = channel->messages->data;
        *type = message->type;
        *data = message->data;
        channel->read_message = message;

        GList* first = channel->messages;
        channel->messages = g_list_remove_link(first, channel->messages);
        g_list_free_1(first);
        return true;
    }
    return false;
}

static void fsapp_channel_python_destructor(PyObject* font_capsule) {
#ifdef FSGUI_DEBUG_CLEANUP
    printf("fsapp_channel_python_destructor FIXME: Not implemented\n");
#endif

    // FIXME: Maybe unref fsapp_channel
}

static PyObject* fsapp_channel_python_create(PyObject* self, PyObject* args) {
    if (!PyArg_ParseTuple(args, ":create")) {
        return NULL;
    }

    fsapp_channel_t* channel = FSEMU_UTIL_MALLOC0(fsapp_channel_t);
    channel->mutex = SDL_CreateMutex();
    channel->messages = NULL;

    // FIXME: fslib_refable?
    // FIXME: Maybe make fsapp_channel refable!
    // fslib_refable_init_with_finalizer(font, fsgui_font_finalize);

    PyObject* capsule = PyCapsule_New(channel, "fsapp_channel_t", fsapp_channel_python_destructor);
    return capsule;
}

static PyObject* fsapp_channel_python_add_message(PyObject* self, PyObject* args) {
    PyObject* channel_capsule;
    int type;
    const char* data;
    if (!PyArg_ParseTuple(args, "Ois:add_message", &channel_capsule, &type, &data)) {
        return NULL;
    }
    fsapp_channel_t* channel =
        (fsapp_channel_t*)PyCapsule_GetPointer(channel_capsule, "fsapp_channel_t");
    if (channel == NULL) {
        return NULL;
    }
    fsapp_channel_add_message(channel, type, data);
    Py_RETURN_NONE;
}

static PyMethodDef fsapp_channel_python_methods[] = {
    {"create", fsapp_channel_python_create, METH_VARARGS, "..."},
    {"add_message", fsapp_channel_python_add_message, METH_VARARGS, "..."},
    {NULL, NULL, 0, NULL}};

static PyModuleDef fsapp_events_python_module = {PyModuleDef_HEAD_INIT,
                                                 "fsapp_events",
                                                 NULL,
                                                 -1,
                                                 fsapp_channel_python_methods,
                                                 NULL,
                                                 NULL,
                                                 NULL,
                                                 NULL};

static PyObject* fsapp_channel_python_init(void) {
    return PyModule_Create(&fsapp_events_python_module);
}

void fsapp_channel_init_module(void) {
    static bool initialized;
    if (initialized) {
        return;
    }
    initialized = true;

    PyImport_AppendInittab("fsapp_channel", &fsapp_channel_python_init);
}
