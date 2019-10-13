#define FSEMU_INTERNAL
#include "fsemu-events.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#if 0

static bool fsemu_handle_sdl_keyboard_shortcut(SDL_Event* event)
{
    if (event->key.keysym.sym == SDLK_q) {
        // FIXME: optional
        if (false) {
            // SDL_Quit();
            event->type = SDL_QUIT;
            return false;
        }
    }
    return false;
}

bool fsemu_handle_sdl_event(SDL_Event* event)
{
    // printf("fsemu_handle_event\n");

    if (event->type == SDL_KEYDOWN) {
        if (event->key.keysym.mod & KMOD_LALT) {  // FIXME: macOS
            return fsemu_handle_sdl_keyboard_shortcut(event);
        }
    }
    if (event->type == SDL_WINDOWEVENT) {
        switch (event->window.event) {
            case SDL_WINDOWEVENT_SHOWN:
                SDL_Log("Window %d shown", event->window.windowID);
                break;
            case SDL_WINDOWEVENT_HIDDEN:
                SDL_Log("Window %d hidden", event->window.windowID);
                break;
            case SDL_WINDOWEVENT_EXPOSED:
                SDL_Log("Window %d exposed", event->window.windowID);
                break;
            case SDL_WINDOWEVENT_MOVED:
                SDL_Log("Window %d moved to %d,%d",
                        event->window.windowID,
                        event->window.data1,
                        event->window.data2);
                break;
            case SDL_WINDOWEVENT_RESIZED:
                SDL_Log("Window %d resized to %dx%d",
                        event->window.windowID,
                        event->window.data1,
                        event->window.data2);
                break;
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                SDL_Log("Window %d size changed to %dx%d",
                        event->window.windowID,
                        event->window.data1,
                        event->window.data2);
                int w = event->window.data1;
                int h = event->window.data2;
                glViewport(0, 0, w, h);
                glMatrixMode(GL_PROJECTION);
                // glLoadIdentity();
                // glOrtho(0, w, h, 0, 0.0, 1.0);
                glMatrixMode(GL_MODELVIEW);
                break;
            case SDL_WINDOWEVENT_MINIMIZED:
                SDL_Log("Window %d minimized", event->window.windowID);
                break;
            case SDL_WINDOWEVENT_MAXIMIZED:
                SDL_Log("Window %d maximized", event->window.windowID);
                break;
            case SDL_WINDOWEVENT_RESTORED:
                SDL_Log("Window %d restored", event->window.windowID);
                break;
            case SDL_WINDOWEVENT_ENTER:
                SDL_Log("Mouse entered window %d", event->window.windowID);
                break;
            case SDL_WINDOWEVENT_LEAVE:
                SDL_Log("Mouse left window %d", event->window.windowID);
                break;
            case SDL_WINDOWEVENT_FOCUS_GAINED:
                SDL_Log("Window %d gained keyboard focus",
                        event->window.windowID);
                break;
            case SDL_WINDOWEVENT_FOCUS_LOST:
                SDL_Log("Window %d lost keyboard focus",
                        event->window.windowID);
                break;
            case SDL_WINDOWEVENT_CLOSE:
                SDL_Log("Window %d closed", event->window.windowID);
                break;
#if SDL_VERSION_ATLEAST(2, 0, 5)
            case SDL_WINDOWEVENT_TAKE_FOCUS:
                SDL_Log("Window %d is offered a focus",
                        event->window.windowID);
                break;
            case SDL_WINDOWEVENT_HIT_TEST:
                SDL_Log("Window %d has a special hit test",
                        event->window.windowID);
                break;
#endif
            default:
                SDL_Log("Window %d got unknown event %d",
                        event->window.windowID,
                        event->window.event);
                break;
        }
    }
    return false;
}
#endif