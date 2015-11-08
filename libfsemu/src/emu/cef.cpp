#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef WITH_CEF

#include <fs/emu/cef.h>
#include <include/cef_app.h>
#include <include/cef_client.h>
#include <include/cef_render_handler.h>
#include <stdlib.h>

extern "C" {

int cef_init(int argc, char *argv[])
{
    CefMainArgs args(argc, argv);
    int result = CefExecuteProcess(args, NULL, NULL);
    if (result >= 0) {
        /* child proccess has endend, so exit */
        exit(result);
    }

    CefSettings settings;

    if (!CefInitialize(args, settings, NULL, NULL)) {
        return -1;
    }

    return 0;
}

void cef_destroy(void)
{
    CefShutdown();
}

class RenderHandler : public CefRenderHandler
{
public:

    RenderHandler()
    {

    }

    bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) {
        int width = 1280;
        int height = 720;
        rect = CefRect(0, 0, width, height);
        return true;
    }

    void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type,
                 const RectList &dirtyRects, const void *buffer,
                 int width, int height) {
#if 0
        memcpy(texBuf->getCurrentLock().data, buffer, width * height * 4);
#endif
    }

    IMPLEMENT_REFCOUNTING(RenderHandler)
};

class BrowserClient : public CefClient
{
public:

    BrowserClient(RenderHandler *renderHandler)
        : m_renderHandler(renderHandler) {

    }

    virtual CefRefPtr<CefRenderHandler> GetRenderHandler() {
        return m_renderHandler;
    }

    CefRefPtr<CefRenderHandler> m_renderHandler;

    IMPLEMENT_REFCOUNTING(BrowserClient)
};

CefRefPtr<CefBrowser> browser;
CefRefPtr<BrowserClient> browser_client;
CefRefPtr<RenderHandler> render_handler;

void cef_create_browser(void)
{
    CefWindowInfo window_info;
    window_info.SetAsWindowless(0, true);

    CefBrowserSettings browser_settings;

    render_handler = new RenderHandler();
    browser_client = new BrowserClient(render_handler);

    const char *url = "http://google.com/";
    browser = CefBrowserHost::CreateBrowserSync(
        window_info, browser_client.get(), url, browser_settings, NULL);
}

} /* extern "C" */

#endif /* WITH_CEF */
