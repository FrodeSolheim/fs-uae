#include "fsapp-events.h"

#include <Python.h>
#include <SDL3/SDL.h>
#include <glib.h>

// #define fslib_list_new g_list_new
// or fslist_new

// ----------------------------------------------------------------------------

static struct {
    SDL_Mutex* mutex;
    GList* events;
} fsapp_events;

struct fsapp_events_event;
typedef struct fsapp_events_event fsapp_events_event_t;

struct fsapp_events_event {
    const char* type;
    char* strdata;
    char* strdata2;
    int intdata;
    int intdata2;
    float floatdata;
    float floatdata2;
};

// ----------------------------------------------------------------------------

void fsapp_events_push(const char* type, const char* strdata, const char* strdata2, int intdata,
                       int intdata2, float floatdata, float floatdata2) {
    // FIXME: This will be called from the main thread...

    fsapp_events_event_t* event = (fsapp_events_event_t*)SDL_malloc(sizeof(fsapp_events_event_t));
    event->type = type;
    if (strdata) {
        event->strdata = SDL_strdup(strdata);
    } else {
        event->strdata = NULL;
    }
    if (strdata2) {
        event->strdata2 = SDL_strdup(strdata2);
    } else {
        event->strdata2 = NULL;
    }
    event->intdata = intdata;
    event->intdata2 = intdata2;
    event->floatdata = floatdata;
    event->floatdata2 = floatdata;

    SDL_assert(fsapp_events.mutex != NULL);
    SDL_LockMutex(fsapp_events.mutex);
    fsapp_events.events = g_list_append(fsapp_events.events, event);
    SDL_UnlockMutex(fsapp_events.mutex);
}

// ----------------------------------------------------------------------------

void fsapp_events_push_string(const char* type, const char* strdata) {
    fsapp_events_push(type, strdata, NULL, 0, 0, 0.0f, 0.0f);
}

// ----------------------------------------------------------------------------

void fsapp_events_push_string_int(const char* type, const char* strdata, int intdata) {
    fsapp_events_push(type, strdata, NULL, intdata, 0, 0.0f, 0.0f);
}

// ----------------------------------------------------------------------------

static PyObject* fsapp_events_python_get(PyObject* self, PyObject* args) {
    if (!PyArg_ParseTuple(args, ":get")) {
        return NULL;
    }
    SDL_assert(fsapp_events.mutex != NULL);
    SDL_LockMutex(fsapp_events.mutex);
    int count = g_list_length(fsapp_events.events);
    PyObject* list = PyList_New(count);
    GList* item = fsapp_events.events;
    int k = 0;
    while (item) {
        fsapp_events_event_t* event = (fsapp_events_event_t*)item->data;
        PyObject* dict = Py_BuildValue(
            "{s:s,s:s,s:s:,s:i,s:i,s:f,s:f}", "type", event->type, "strdata", event->strdata,
            "strdata2", event->strdata2, "intdata", event->intdata, "intdata2", event->intdata2,
            "floatdata", event->floatdata, "floatdata2", event->floatdata2);
        PyList_SetItem(list, k, dict);  // or PyList_SET_ITEM(list, k, tuple);
        if (event->strdata) {
            SDL_free(event->strdata);
        }
        if (event->strdata2) {
            SDL_free(event->strdata2);
        }
        free(event);
        item = item->next;
        k += 1;
    }
    g_list_free(fsapp_events.events);
    fsapp_events.events = NULL;
    SDL_UnlockMutex(fsapp_events.mutex);
    return list;
}

// ----------------------------------------------------------------------------

static PyMethodDef fsapp_events_python_methods[] = {
    {"get", fsapp_events_python_get, METH_VARARGS, "..."}, {NULL, NULL, 0, NULL}};

static PyModuleDef fsapp_events_python_module = {PyModuleDef_HEAD_INIT,
                                                 "fsapp_events",
                                                 NULL,
                                                 -1,
                                                 fsapp_events_python_methods,
                                                 NULL,
                                                 NULL,
                                                 NULL,
                                                 NULL};

static PyObject* fsapp_events_python_initfunc(void) {
    return PyModule_Create(&fsapp_events_python_module);
}

// ----------------------------------------------------------------------------

void fsapp_events_init_module(void) {
    static bool initialized;
    if (initialized) {
        return;
    }
    initialized = true;

    fsapp_events.mutex = SDL_CreateMutex();

    PyImport_AppendInittab("fsapp_events", &fsapp_events_python_initfunc);
}
