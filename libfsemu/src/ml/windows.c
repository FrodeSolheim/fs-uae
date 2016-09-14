#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef WINDOWS
#define _WIN32_WINNT 0x0501
#include <Windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <stdio.h>
#include <stddef.h>
#include <glib.h>
#include <SDL.h>
#include <SDL_syswm.h>

#include <fs/log.h>
#include "fs/ml.h"
#include "ml_internal.h"

HICON hIconSmall = 0;
HICON hIconBig = 0;

void fs_ml_configure_window(void)
{
    fs_log("fs_ml_configure_window\n");

    SDL_SysWMinfo info;
    SDL_VERSION(&info.version); // this is important!
#if SDL_VERSION_ATLEAST(2, 0, 0)
    if (!SDL_GetWindowWMInfo(g_fs_ml_window, &info)) {
#else
    if (!SDL_GetWMInfo(&info)) {
#endif
        fs_log("error getting window information\n");
        return;
    }

    if (hIconSmall == 0) {
        hIconSmall = (HICON) LoadImage(GetModuleHandle(NULL),
                "icon", IMAGE_ICON, 16, 16, 0);
        fs_log("hIconSmall = %d\n", hIconSmall);
    }
    if (hIconBig == 0) {
        hIconBig = (HICON) LoadImage(GetModuleHandle(NULL),
                 "icon", IMAGE_ICON, 32, 32, 0);
        fs_log("hIconBig = %d\n", hIconBig);
    }
    if (hIconSmall != 0) {
#if SDL_VERSION_ATLEAST(2, 0, 0)
        SendMessage(info.info.win.window, WM_SETICON, ICON_SMALL, (LPARAM) hIconSmall);
#else
        SendMessage(info.window, WM_SETICON, ICON_SMALL, (LPARAM) hIconSmall);
#endif
    }
    if (hIconBig != 0) {
#if SDL_VERSION_ATLEAST(2, 0, 0) 
        SendMessage(info.info.win.window, WM_SETICON, ICON_BIG, (LPARAM) hIconBig);
#else
        SendMessage(info.window, WM_SETICON, ICON_BIG, (LPARAM) hIconBig);
#endif

    }
}

void fs_ml_prevent_power_saving(void)
{
    // on Windows we only need to call a function once to change the thread
    // state
    static int initialized = 0;
    if (initialized) {
        return;
    }
    fs_log("SetThreadExecutionState(ES_CONTINUOUS | ES_DISPLAY_REQUIRED)\n");
    if (!SetThreadExecutionState(ES_CONTINUOUS | ES_DISPLAY_REQUIRED)) {
        fs_log("SetThreadExecutionState failed\n");
    }
    initialized = 1;
}

void fs_ml_activate_window_switcher_impl(void)
{
    fs_log("FIXME: Active window switcher not implemented\n");
}

// not used
int fs_ml_scancode_to_key(int scancode) {
    return 0;
}

#if 0
static int64_t g_syncbase = 0;

int64_t fs_ml_read_clock(void) {
    LARGE_INTEGER result;
    QueryPerformanceCounter(&result);
    return result.QuadPart;
}

void fs_ml_calibrate_clock(void) {
    //SetProcessAffinityMask(GetCurrentProcess(), 1);
    LARGE_INTEGER result;
    QueryPerformanceFrequency(&result);
    g_syncbase = result.QuadPart;
    fs_log("calibrating clock: frequency is %lld\n", g_syncbase);
}

int64_t fs_ml_monotonic_time() {
    // FIXME: QueryPerformanceFrequency seems to give incorrect result
    // on my computer. Using "less accurate" timeGetTime instead, -but
    // beware, value will wrap around after about 47 days...

    //fs_log("%lld\n", g_syncbase);
    //fs_log("%lld\n", 1000000);
    //return (1000000 * fs_ml_read_clock()) / g_syncbase;
    //return (fs_ml_read_clock() * 1000000) / g_syncbase;
    return ((int64_t) timeGetTime()) * 1000;
    //return ((double)fs_ml_read_clock() / (double)g_syncbase) * 1000000;
}
#endif

void fs_ml_usleep(int usec) {
    SDL_Delay(usec / 1000);
}

//void fs_ml_set_fullscreen_extra() {
//}

#if 0
int fs_ml_video_mode_get_current(fs_ml_video_mode *mode)
{
    DEVMODE devmode;
    devmode.dmSize = sizeof(DEVMODE);
    devmode.dmDriverExtra = 0;
    int result = EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devmode);
    if (result == 0) {
        return 1;
    }
    mode->width = devmode.dmPelsWidth;
    mode->height = devmode.dmPelsHeight;
    mode->fps = devmode.dmDisplayFrequency;
    mode->bpp = devmode.dmBitsPerPel;
    mode->flags = devmode.dmDisplayFlags;
    return 0;
}
#endif

static STICKYKEYS g_StartupStickyKeys = {sizeof(STICKYKEYS), 0};
static TOGGLEKEYS g_StartupToggleKeys = {sizeof(TOGGLEKEYS), 0};
static FILTERKEYS g_StartupFilterKeys = {sizeof(FILTERKEYS), 0};

static void disable_accessibility_shortcuts() {

    SystemParametersInfo(SPI_GETSTICKYKEYS, sizeof(STICKYKEYS),
            &g_StartupStickyKeys, 0);
    SystemParametersInfo(SPI_GETTOGGLEKEYS, sizeof(TOGGLEKEYS),
            &g_StartupToggleKeys, 0);
    SystemParametersInfo(SPI_GETFILTERKEYS, sizeof(FILTERKEYS),
            &g_StartupFilterKeys, 0);

    // Disable StickyKeys/etc shortcuts but if the accessibility feature is on,
    // then leave the settings alone as its probably being usefully used

    STICKYKEYS skOff = g_StartupStickyKeys;
    if ((skOff.dwFlags & SKF_STICKYKEYSON) == 0) {
        skOff.dwFlags &= ~SKF_HOTKEYACTIVE;
        skOff.dwFlags &= ~SKF_CONFIRMHOTKEY;
        SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &skOff, 0);
    }

    TOGGLEKEYS tkOff = g_StartupToggleKeys;
    if ((tkOff.dwFlags & TKF_TOGGLEKEYSON) == 0) {
        tkOff.dwFlags &= ~TKF_HOTKEYACTIVE;
        tkOff.dwFlags &= ~TKF_CONFIRMHOTKEY;
        SystemParametersInfo(SPI_SETTOGGLEKEYS, sizeof(TOGGLEKEYS), &tkOff, 0);
    }

    FILTERKEYS fkOff = g_StartupFilterKeys;
    if ((fkOff.dwFlags & FKF_FILTERKEYSON) == 0) {
        fkOff.dwFlags &= ~FKF_HOTKEYACTIVE;
        fkOff.dwFlags &= ~FKF_CONFIRMHOTKEY;
        SystemParametersInfo(SPI_SETFILTERKEYS, sizeof(FILTERKEYS), &fkOff, 0);
    }
}

void fs_ml_initialize_keymap() {
    // FIXME: should ideally re-enable the shortcuts on exit, of they
    // were enabled in the first place.
    disable_accessibility_shortcuts();
}

#ifdef WITH_WINDOWS_VIDEO

#include "video.h"
// #include <fs/glee.>
#include <Ddraw.h>

/*
 * Background information:
 * http://www.opengl.org/wiki/Platform_specifics:_Windows
 * http://www.opengl.org/wiki/SwapInterval_aka_vsync
 * http://www.equalizergraphics.com/documents/Developer/API-0.9/wglWindow_8cpp-source.html
 */

static HGLRC g_hrc;
static HDC g_hdc;

static void setup_pixel_format(HDC hdc) {
    static PIXELFORMATDESCRIPTOR pfd = {
            sizeof(PIXELFORMATDESCRIPTOR),          //size of structure
            1,                                      //default version
            PFD_DRAW_TO_WINDOW |                    //window drawing support
            PFD_SUPPORT_OPENGL |                    //opengl support
            PFD_DOUBLEBUFFER,                       //double buffering support
            PFD_TYPE_RGBA,                          //RGBA color mode
            32,                                     //32 bit color mode
            0, 0, 0, 0, 0, 0,                       //ignore color bits
            0,                                      //no alpha buffer
            0,                                      //ignore shift bit
            0,                                      //no accumulation buffer
            0, 0, 0, 0,                             //ignore accumulation bits
            16,                                     //16 bit z-buffer size
            0,                                      //no stencil buffer
            0,                                      //no aux buffer
            PFD_MAIN_PLANE,                         //main drawing plane
            0,                                      //reserved
            0, 0, 0 };                              //layer masks ignored
    // choose best matching format
    int nPixelFormat = ChoosePixelFormat(hdc, &pfd);
    fs_log("pixel format flags: %d\n", pfd.dwFlags);
    fs_log("double buffering: %d\n", (pfd.dwFlags & PFD_DOUBLEBUFFER) != 0);
    // set the pixel format to the device context
    SetPixelFormat(hdc, nPixelFormat, &pfd);
}

static int get_fullscreen_width() {
    return GetSystemMetrics(SM_CXSCREEN);
}

static int get_fullscreen_height() {
    return GetSystemMetrics(SM_CYSCREEN);
}

static void post_mouse_button_event(int button, int state) {
    fs_ml_input_event *event = fs_ml_input_event_alloc();
    if (state) {
        event->type = FS_ML_INPUT_EVENT_MOUSE_BUTTON_DOWN;
    }
    else {
        event->type = FS_ML_INPUT_EVENT_MOUSE_BUTTON_UP;
    }
    event->mouse.button = button;
    fs_ml_input_event_post(event);
}

static void process_mouse_input(LPRAWINPUT raw_input) {
    LPRAWMOUSE raw_mouse = &(raw_input->data.mouse);
    //fs_log("flags: %d %d %d\n", raw_mouse->usFlags, MOUSE_MOVE_RELATIVE, MOUSE_MOVE_ABSOLUTE);
    if (raw_mouse->usFlags & MOUSE_MOVE_ABSOLUTE) {

    }
    else {
        if (raw_mouse->lLastX != 0 || raw_mouse->lLastY != 0) {
            fs_ml_input_event *event = fs_ml_input_event_alloc();
            event->type = FS_ML_INPUT_EVENT_MOUSE_MOTION;
            event->mouse.dx = raw_mouse->lLastX;
            event->mouse.dy = raw_mouse->lLastY;
            fs_ml_input_event_post(event);
        }
    }
    int flags = raw_mouse->usButtonFlags;
    if (flags) {
        if (flags & RI_MOUSE_LEFT_BUTTON_DOWN) {
            post_mouse_button_event(1, 1);
        }
        if (flags & RI_MOUSE_LEFT_BUTTON_UP) {
            post_mouse_button_event(1, 0);
        }
        if (flags & RI_MOUSE_MIDDLE_BUTTON_DOWN) {
            post_mouse_button_event(2, 1);
        }
        if (flags & RI_MOUSE_MIDDLE_BUTTON_UP) {
            post_mouse_button_event(2, 0);
        }
        if (flags & RI_MOUSE_RIGHT_BUTTON_DOWN) {
            post_mouse_button_event(3, 1);
        }
        if (flags & RI_MOUSE_RIGHT_BUTTON_UP) {
            post_mouse_button_event(3, 0);
        }
    }
}

#define NUM_VKEYS 256
#define MAX_VKEY (3 * NUM_VKEYS - 1)
// first NUM_VKEYS entries contain vkeys, then vkeys for E0 and E1
#define E0 NUM_VKEYS
#define E1 (NUM_VKEYS * 2)

static char g_key_state[3 * NUM_VKEYS] = {};
static int16_t g_key_mapping[3 * NUM_VKEYS] = {};

static void process_keyboard_input(LPRAWINPUT raw_input) {
    //LPRAWKEYBOARD raw_keyboard = &(raw_input->data.keyboard);
    //int vkey = raw_keyboard->VKey;
    //int vkey = raw_keyboard->VKey;
    //int flags = raw_keyboard->Flags; //
    int vkey = raw_input->data.keyboard.VKey;
    int flags = raw_input->data.keyboard.Flags;
    int make_code = raw_input->data.keyboard.MakeCode;
    // special cases
    if (make_code == 54) {
        // for some reason, flags is not set properly for right shift
        vkey = VK_SHIFT + E0;
    }
    else if (flags & RI_KEY_E0) {
        vkey += NUM_VKEYS;
    }
    else if (flags & RI_KEY_E1) {
        vkey += 2 * NUM_VKEYS;
    }
    fs_log("vkey...: %d %d %d flags: %d\n", vkey,
            raw_input->data.keyboard.MakeCode,
            raw_input->data.keyboard.ExtraInformation, flags);
    if (flags & RI_KEY_BREAK) {
        // key is up
        if (vkey <= MAX_VKEY && g_key_state[vkey]) {
            g_key_state[vkey] = 0;
            fs_ml_input_event *event = fs_ml_input_event_alloc();
            event->type = FS_ML_INPUT_EVENT_KEY_UP;
            event->key.code = g_key_mapping[vkey];
            event->key.mod = 0;
            fs_ml_input_event_post(event);
        }
    }
    else {
        // key is down
        if (vkey <= MAX_VKEY && !g_key_state[vkey]) {
            //fs_log("down\n");
            g_key_state[vkey] = 1;
            fs_ml_input_event *event = fs_ml_input_event_alloc();
            event->type = FS_ML_INPUT_EVENT_KEY_DOWN;
            event->key.code = g_key_mapping[vkey];
            event->key.mod = 0;
            //fs_log("code: %d\n", event->key.code);
            fs_ml_input_event_post(event);
        }
    }
}

static void process_input(void *data) {
    //fs_log("process_input\n");
    LPRAWINPUT raw_input = data;
    if (raw_input->header.dwType == RIM_TYPEMOUSE) {
        process_mouse_input(raw_input);
    }
    else if (raw_input->header.dwType == RIM_TYPEKEYBOARD) {
        process_keyboard_input(raw_input);
    }
}

#define RAW_INPUT_MAX_SIZE 1024
static unsigned char *g_raw_input_data[RAW_INPUT_MAX_SIZE];

static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam,
        LPARAM lparam) {
    //fs_log("WndProc %d\n", message);

    //static HGLRC hRC; //rendering context
    //static HDC hDC;   //device context
    HRAWINPUT raw_input_handle;

    switch (message) {
    case WM_CREATE:
        g_hdc = GetDC(hwnd);    //get the device context for window
        if (g_hdc == NULL) {
            fs_log("could not get window dc\n");
        }
        setup_pixel_format(g_hdc);
        g_hrc = wglCreateContext(g_hdc);    //create rendering context
        if (g_hrc == NULL) {
            fs_log("could not create wgl context\n");
        }
        //make rendering context current
        if (!wglMakeCurrent(g_hdc, g_hrc)) {
            fs_log("could not set current wgl context in main thread\n");
        }
        break;
    case WM_DESTROY:
        wglMakeCurrent(g_hdc, NULL); //deselect rendering context
        wglDeleteContext(g_hrc); //delete rendering context
        PostQuitMessage(0); //send wm_quit
        break;
    case WM_ERASEBKGND:
        break;
    case WM_INPUT:
        raw_input_handle = (HRAWINPUT) lparam;
        /*
        unsigned int size;
        if (GetRawInputData(raw_input_handle, RID_INPUT, NULL, &size,
                sizeof(RAWINPUTHEADER)) != -1) {
            void *data = malloc(size);
            if (GetRawInputData(raw_input_handle, RID_INPUT, data, &size,
                    sizeof(RAWINPUTHEADER)) != -1) {
                process_input(data);
            }
            free(data);
        }
        */
        unsigned int size = RAW_INPUT_MAX_SIZE;
        if (GetRawInputData(raw_input_handle, RID_INPUT,
                &g_raw_input_data, &size, sizeof(RAWINPUTHEADER)) != -1) {
            process_input(&g_raw_input_data);
        }
        // must call DefWindowProc according to http://msdn.microsoft.com/
        // en-us/library/windows/desktop/ms645590(v=vs.85).aspx
        return DefWindowProc(hwnd, message, wparam, lparam);
    default:
        return DefWindowProc(hwnd, message, wparam, lparam);
    }
    return 0;
}

static GCond *g_video_cond = NULL;
static GMutex *g_video_mutex = NULL;

void fs_ml_wait_vblank() {
    if (fs_ml_video_vsync()) {
        g_mutex_lock(g_video_mutex);
        g_cond_wait(g_video_cond, g_video_mutex);
        g_mutex_unlock(g_video_mutex);
    }
    else {
        g_cond_signal(g_video_cond);
    }
}

//HRESULT (*WaitForVerticalBlank)(DWORD dwFlags, HANDLE hEvent) = NULL;

/*
void fs_ml_video_frame_ready() {

}
*/

static gpointer video_thread_entry(gpointer data) {

    fs_log("video thread started\n");

    if (!wglMakeCurrent(g_hdc, g_hrc)) {
        fs_log("could not set current wgl context for video thread\n");
    }

    // this function must be called from the video thread
    fs_emu_video_init_opengl();
    if (fs_ml_video_vsync()) {
        wglSwapIntervalEXT(1);
    }
    else {
        wglSwapIntervalEXT(0);
    }

    while (1) {
        //g_mutex_lock(g_video_mutex);

        // ...
        //fs_log("c\n");

        if (!fs_ml_video_vsync()) {
            // when vsync is off, we wait until a new frame is ready and
            // then we display it immediately
            g_mutex_lock(g_video_mutex);
            g_cond_wait(g_video_cond, g_video_mutex);
            g_mutex_unlock(g_video_mutex);
        }

        fs_emu_render();
        //fs_log("d\n");


        //g_mutex_unlock(g_video_mutex);

        //fs_emu_msleep(5);
        //fs_ml_swap_buffers();
        /*
        if (WaitForVerticalBlank) {
            fs_log("wait\n");
            int result = WaitForVerticalBlank(DDWAITVB_BLOCKBEGIN, NULL);
            if (result != DD_OK) {
                fs_log("wait error %d\n", result);
            }
        }
        */
        if (fs_ml_video_vsync()) {
            fs_gl_finish();
            SwapBuffers(g_hdc);
            fs_gl_finish();
            g_cond_signal(g_video_cond);
        }
        else {
            SwapBuffers(g_hdc);
            fs_gl_finish();
        }
        fs_emu_swap_buffers();
    }
    return NULL;
}

void fs_ml_swap_buffers() {
    SwapBuffers(g_hdc);
}

int fs_ml_video_create_window() {

/*
int WINAPI fs_emu_winmain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow, void (*main_function)()) {
    */
    fs_log("fs_ml_video_create_window\n");

    /*
    HMODULE direct_draw = LoadLibrary("Ddraw.dll");
    if (direct_draw) {
        WaitForVerticalBlank = GetProcAddress(direct_draw,
                "WaitForVerticalBlank");
        fs_log("WaitForVerticalBlank at %p\n", WaitForVerticalBlank);
    }
    else {
        fs_log("could not load Ddraw.dll\n");
    }
    */

    g_video_cond = g_cond_new();
    g_video_mutex = g_mutex_new();

    static TCHAR szWindowClass[] = _T("libfsemu-window");
    static TCHAR szTitle[] = _T("fs-emu");

    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = g_fs_ml_hinstance;
    wcex.hIcon          = NULL; //LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = NULL; //LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

    if (!RegisterClassEx(&wcex)) {
        MessageBox(NULL,
            _T("Call to RegisterClassEx failed!"),
            _T("Win32 Guided Tour"), 0);
        return 1;
    }

    DWORD dwStyle;
    DWORD dwExStyle;

    RECT windowRect;
    windowRect.left = 0;
    //windowRect.right = 1680;
    windowRect.right = 1024;
    windowRect.top = 0;
    //windowRect.bottom = 1050;
    windowRect.bottom = 600;

    int fullscreen = g_fs_emu_video_fullscreen;

    if (fullscreen) {
        windowRect.right = get_fullscreen_width();
        windowRect.bottom = get_fullscreen_height();

        dwExStyle = WS_EX_APPWINDOW;    //window extended style
            dwStyle = WS_POPUP;                             //windows style
            ShowCursor(FALSE);                              //hide mouse pointer
    }
    else {
            dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE; //window extended style
            dwStyle = WS_OVERLAPPEDWINDOW;                                  //windows style
    }

    AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

    // The parameters to CreateWindow explained:
    // szWindowClass: the name of the application
    // szTitle: the text that appears in the title bar
    // WS_OVERLAPPEDWINDOW: the type of window to create
    // CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
    // 500, 100: initial size (width, length)
    // NULL: the parent of this window
    // NULL: this application does not have a menu bar
    // hInstance: the first parameter from WinMain
    // NULL: not used in this application
    HWND hWnd = CreateWindow(
        szWindowClass,
        szTitle,
        dwStyle,
        CW_USEDEFAULT, CW_USEDEFAULT,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        NULL,
        NULL,
        g_fs_ml_hinstance,
        NULL
    );
    if (!hWnd) {
        fs_log("%d\n", GetLastError());
        MessageBox(NULL,
            _T("Call to CreateWindow failed!"),
            _T("Win32 Guided Tour"), 0);
        return 1;
    }

    ShowWindow(hWnd, g_fs_ml_ncmdshow);
    UpdateWindow(hWnd);

    RAWINPUTDEVICE rid;
    // mice
    rid.usUsagePage = 0x01;
    rid.usUsage = 0x02;
    rid.dwFlags = 0;
    //rid.dwFlags = RIDEV_INPUTSINK;
    //rid.dwFlags = RIDEV_NOLEGACY;
    rid.hwndTarget = hWnd;
    RegisterRawInputDevices(&rid, 1, sizeof(rid));
    // keyboards
    rid.usUsagePage = 0x01;
    rid.usUsage = 0x06;
    rid.dwFlags = 0;
    //rid.dwFlags = RIDEV_INPUTSINK;
    //rid.dwFlags = RIDEV_NOLEGACY;
    rid.hwndTarget = hWnd;
    RegisterRawInputDevices(&rid, 1, sizeof(rid));

    GError *error = NULL;
    wglMakeCurrent(g_hdc, NULL); //deselect rendering context
    //GThread *video_thread =
    g_thread_create(video_thread_entry, NULL, FALSE, &error);
    if (error != NULL) {
        fs_log("error starting video thread\n");
        // FIXME: ERROR MESSAGE HERE
        // FIXME: FATAL
        return 1;
    }
    return 0;

    /*
    // Main message loop:
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
    */
}

int fs_ml_main_loop() {
    // Main message loop:
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int) msg.wParam;
}

/*
int main(int argc, char *argv[]) {
    fs_log("heisann\n");
    return 0;
}
*/

//UINT WINAPI GetRawInputDeviceList(
        //  __out_opt  PRAWINPUTDEVICELIST pRawInputDeviceList,
  //  __inout    PUINT puiNumDevices,
  //__in       UINT cbSize
//);

void fs_ml_error(const char *msg) {
    fs_log("ERROR: %s\n", msg);
    int error = GetLastError();
    fs_log("GetLastError returned %d (not necessarily related)\n", error);
}

void fs_ml_fatal(const char *msg) {
    fs_log("FATAL: %s\n", msg);
    int error = GetLastError();
    fs_log("GetLastError returned %d (not necessarily related)\n", error);
    fs_log("exiting...\n");
    exit(1);
}

void fs_ml_fatal_malloc() {
    fs_ml_fatal("could not allocate memory");
}

static void list_input_devices() {
    UINT num_devices;
    PRAWINPUTDEVICELIST pRawInputDeviceList;
    if (GetRawInputDeviceList(NULL, &num_devices,
            sizeof(RAWINPUTDEVICELIST)) != 0) {
        fs_ml_fatal("GetRawInputDeviceList (count devices) failed ");
    }
    if ((pRawInputDeviceList = malloc(sizeof(RAWINPUTDEVICELIST) *
            num_devices)) == NULL) {
        fs_ml_fatal_malloc();
    }
    if (GetRawInputDeviceList(pRawInputDeviceList, &num_devices,
            sizeof(RAWINPUTDEVICELIST)) == -1) {
        fs_ml_fatal("GetRawInputDeviceList (get devices) failed ");
    }

    for (int i = 0; i < num_devices; i++) {
        fs_log("device index: %d\n", i);
        fs_log("device type: %d\n", pRawInputDeviceList[i].dwType);
        char device_name[1024];
        unsigned int data_size = 1023;
        if (GetRawInputDeviceInfo(
                pRawInputDeviceList[i].hDevice,
                RIDI_DEVICENAME,
                &device_name, &data_size) == -1) {
            fs_ml_error("GetRawInputDeviceInfo (RIDI_DEVICENAME) failed");
            continue;
        }
        fs_log("       name: %s\n", device_name);
        RID_DEVICE_INFO device_info;
        device_info.cbSize = sizeof(RID_DEVICE_INFO);
        data_size = sizeof(RID_DEVICE_INFO);
        if (GetRawInputDeviceInfo(
                pRawInputDeviceList[i].hDevice,
                RIDI_DEVICEINFO,
                &device_info, &data_size) == -1) {
            fs_ml_error("GetRawInputDeviceInfo failed");
            continue;
        }
        //fs_log("       type: %d\n", device_info.dwType);
    }

    free(pRawInputDeviceList);
}


void fs_ml_video_init() {

}

void fs_ml_input_init() {
    list_input_devices();
    // http://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=VS.85).aspx
    g_key_mapping[VK_BACK] = FS_ML_KEY_BACKSPACE;
    g_key_mapping[VK_TAB] = FS_ML_KEY_TAB;
    g_key_mapping[VK_CLEAR] = FS_ML_KEY_CLEAR;
    g_key_mapping[VK_RETURN] = FS_ML_KEY_RETURN;
    // FIXME: RSHIFT
    g_key_mapping[VK_SHIFT] = FS_ML_KEY_LSHIFT;
    g_key_mapping[VK_CONTROL] = FS_ML_KEY_LCTRL;
    g_key_mapping[E0 + VK_CONTROL] = FS_ML_KEY_RCTRL;
    g_key_mapping[VK_MENU] = FS_ML_KEY_LALT;
    g_key_mapping[VK_PAUSE] = FS_ML_KEY_PAUSE;
    // ... FIXME ...
    g_key_mapping[E0 + VK_LEFT] = FS_ML_KEY_LEFT;
    g_key_mapping[E0 + VK_UP] = FS_ML_KEY_UP;
    g_key_mapping[E0 + VK_RIGHT] = FS_ML_KEY_RIGHT;
    g_key_mapping[E0 + VK_DOWN] = FS_ML_KEY_DOWN;
    // ... FIXME ...
    g_key_mapping[VK_F1] = FS_ML_KEY_F1;
    g_key_mapping[VK_F2] = FS_ML_KEY_F2;
    g_key_mapping[VK_F3] = FS_ML_KEY_F3;
    g_key_mapping[VK_F4] = FS_ML_KEY_F4;
    g_key_mapping[VK_F5] = FS_ML_KEY_F5;
    g_key_mapping[VK_F6] = FS_ML_KEY_F6;
    g_key_mapping[VK_F7] = FS_ML_KEY_F7;
    g_key_mapping[VK_F8] = FS_ML_KEY_F8;
    g_key_mapping[VK_F9] = FS_ML_KEY_F9;
    g_key_mapping[VK_F10] = FS_ML_KEY_F10;
    g_key_mapping[VK_F11] = FS_ML_KEY_F11;
    g_key_mapping[VK_F12] = FS_ML_KEY_F12;
}

#endif // WITH_VIDEO_WINDOWS

#else

int libfsemu_ml_windows_dummy;

#endif // WINDOWS

