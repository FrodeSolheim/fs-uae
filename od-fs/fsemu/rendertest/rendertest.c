#include <SDL.h>
#include <SDL_opengl.h>

static char *image_rgba;
static SDL_Window *sdl_window;
static SDL_GLContext sdl_glcontext;
static int want_quit;
static int fullscreen;
static GLuint gl_texture;
static GLuint gl_textures[3];
static int screen_width;
static int screen_height;
static int frames;

static void create_images(void)
{
    image_rgba = malloc(1024 * 768 * 4);
    char *p = image_rgba;
    for (int y = 0; y < 768; y++) {
        for (int x = 0; x < 1024; x++) {
            if ((x / 32) % 2 == 0) {
                *p++ = 0xff;
                *p++ = 0x00;
                *p++ = 0xff;
            } else {
                *p++ = 0x00;
                *p++ = 0xff;
                *p++ = 0x00;
            }
            *p++ = 0xff;
        }
    }
}

static int check_opengl_error(const char *operation)
{
    int error = glGetError();
    if (error != 0) {
        printf("%s OpenGL error: %d\n", operation, error);
    }
    return error;
}

static void create_sdl_window(int opengl)
{
    int flags = SDL_WINDOW_RESIZABLE;
    if (opengl) {
        flags |= SDL_WINDOW_OPENGL;
    }
    if (fullscreen) {
        flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    }
    sdl_window = SDL_CreateWindow("Test",
                                  SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED,
                                  screen_width,
                                  screen_height,
                                  flags);
    if (sdl_window == NULL) {
        printf("%s\n", SDL_GetError());
    }
    if (opengl) {
        SDL_GLContext sdl_glcontext = SDL_GL_CreateContext(sdl_window);
        glViewport(0, 0, screen_width, screen_height);
        check_opengl_error("glViewport");
        glClearColor(1.0f, 0.0f, 1.0f, 0.0f);
        check_opengl_error("glClearColor");
        glClear(GL_COLOR_BUFFER_BIT);
        check_opengl_error("glClear");
    }
    SDL_ShowWindow(sdl_window);
}

void handle_events(void)
{
    SDL_Event Event;
    while (SDL_PollEvent(&Event)) {
        if (Event.type == SDL_KEYDOWN) {
            switch (Event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    want_quit = 1;
                    break;
                case 'f':
                    fullscreen = !fullscreen;
                    if (fullscreen) {
                        SDL_SetWindowFullscreen(sdl_window,
                                                SDL_WINDOW_FULLSCREEN_DESKTOP);
                    } else {
                        SDL_SetWindowFullscreen(sdl_window, 0);
                    }
                    break;
                default:
                    break;
            }
        } else if (Event.type == SDL_QUIT) {
            want_quit = 1;
        }
    }
}

static void create_texture(int n)
{
    glGenTextures(1, gl_textures + n);
    check_opengl_error("glGenTextures");
    glBindTexture(GL_TEXTURE_2D, gl_textures[n]);
    check_opengl_error("glBindTexture");
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB565,
                 // GL_RGBA,
                 // GL_RGB,
                 768,
                 576,
                 0,
                 GL_RGB,
                 GL_UNSIGNED_SHORT_5_6_5,
                 // GL_RGBA,
                 // GL_UNSIGNED_BYTE,
                 image_rgba);
    check_opengl_error("glTexImage2D");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    check_opengl_error("glTexParameteri");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    check_opengl_error("glTexParameteri");
}

static void setup_opengl(void)
{
    SDL_GL_SetSwapInterval(0);
    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    create_texture(0);
    create_texture(1);
    create_texture(2);
    glColor3f(1.0, 1.0, 1.0);
    check_opengl_error("glColor3f");
}

static void update_texture(void)
{
    //    int n = frames % 3;
    //    glBindTexture(GL_TEXTURE_2D, gl_textures[n]);

    int offset = frames % 64;
    int bpp = 4;
    glTexSubImage2D(GL_TEXTURE_2D,
                    0,
                    0,
                    0,
                    768,
                    576,
                    GL_RGB,
                    GL_UNSIGNED_SHORT_5_6_5,
                    // GL_RGBA,
                    // GL_UNSIGNED_BYTE,
                    image_rgba
                    // image_rgba + offset * bpp
    );
}

static void render_texture(void)
{
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex2f(-1.0, -1.0);
    glTexCoord2f(1.0, 0.0);
    glVertex2f(1.0, -1.0);
    glTexCoord2f(1.0, 1.0);
    glVertex2f(1.0, 1.0);
    glTexCoord2f(0.0, 1.0);
    glVertex2f(-1.0, 1.0);
    glEnd();
}

double benchmark(GLint internal_format, GLenum format, GLenum type)
{
    int width = 768;
    int height = 576;
    const void *data = image_rgba;
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 internal_format,
                 width,
                 height,
                 0,
                 format,
                 type,
                 data);
    int error = glGetError();
    if (error) {
        glDeleteTextures(1, &texture);
        return error;
    }
    check_opengl_error("glTexImage2D");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    check_opengl_error("glTexParameteri");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    check_opengl_error("glTexParameteri");

    int iterations = 100;
    int t1 = SDL_GetTicks();
    for (int i = 0; i < iterations; i++) {
        glTexSubImage2D(
            GL_TEXTURE_2D, 0, 0, 0, width, height, format, type, data);
        glFinish();
    }
    int t2 = SDL_GetTicks();
    double result = 1.0 * (t2 - t1) / iterations;
    printf("%5.1f ", result);

    glDisable(GL_DEPTH_TEST);
    t1 = SDL_GetTicks();
    for (int i = 0; i < iterations; i++) {
        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0);
        glVertex2f(-1.0, -1.0);
        glTexCoord2f(1.0, 0.0);
        glVertex2f(1.0, -1.0);
        glTexCoord2f(1.0, 1.0);
        glVertex2f(1.0, 1.0);
        glTexCoord2f(0.0, 1.0);
        glVertex2f(-1.0, 1.0);
        glEnd();
        glFinish();
    }
    t2 = SDL_GetTicks();
    check_opengl_error("GL_QUAD");
    result = 1.0 * (t2 - t1) / iterations;

    printf("%5.1f   ", result);
    glDeleteTextures(1, &texture);
    return 0;
}

void benchmark3(GLint iformat,
                const char *iformat_desc,
                GLenum format,
                const char *format_desc,
                GLenum type,
                const char *type_desc)
{
    int error = benchmark(iformat, format, type);
    if (error == 0) {
        printf("%s %s %s\n", iformat_desc, format_desc, type_desc);
    }
}

void benchmark2(GLint iformat,
                const char *iformat_desc,
                GLenum format,
                const char *format_desc)
{
    benchmark3(iformat,
               iformat_desc,
               format,
               format_desc,
               GL_UNSIGNED_BYTE,
               "GL_UNSIGNED_BYTE");
    benchmark3(iformat,
               iformat_desc,
               format,
               format_desc,
               GL_UNSIGNED_SHORT_5_6_5,
               "GL_UNSIGNED_SHORT_5_6_5");
    benchmark3(iformat,
               iformat_desc,
               format,
               format_desc,
               GL_UNSIGNED_SHORT_5_6_5_REV,
               "GL_UNSIGNED_SHORT_5_6_5_REV");
    benchmark3(iformat,
               iformat_desc,
               format,
               format_desc,
               GL_UNSIGNED_INT_8_8_8_8,
               "GL_UNSIGNED_INT_8_8_8_8");
    benchmark3(iformat,
               iformat_desc,
               format,
               format_desc,
               GL_UNSIGNED_INT_8_8_8_8_REV,
               "GL_UNSIGNED_INT_8_8_8_8_REV");
}

void benchmark1(GLint iformat, const char *iformat_desc)
{
    benchmark2(iformat, iformat_desc, GL_RGB, "GL_RGB");
    benchmark2(iformat, iformat_desc, GL_RGBA, "GL_RGBA");
    benchmark2(iformat, iformat_desc, GL_BGRA, "GL_BGRA");
}

void benchmark0(void)
{
    benchmark1(GL_RGB565, "GL_RGB565");
    benchmark1(GL_RGB5, "GL_RGB5");
    benchmark1(GL_RGB5_A1, "GL_RGB5_A1");
    benchmark1(GL_RGB, "GL_RGB");
    benchmark1(GL_RGBA, "GL_RGBA");
    benchmark1(GL_BGRA_EXT, "GL_BGRA_EXT");
}

void benchmarks(void)
{
    benchmark0();
#if 0
    double dt;
    dt = benchmark(GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
    printf("GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE: %0.1f ms\n", dt);

    dt = benchmark(GL_RGBA, GL_RGB, GL_UNSIGNED_SHORT_5_6_5);
    printf("GL_RGBA, GL_RGB, GL_UNSIGNED_SHORT_5_6_5: %0.1f ms\n", dt);

    dt = benchmark(GL_RGB, GL_RGB, GL_UNSIGNED_SHORT_5_6_5);
    printf("GL_RGB, GL_RGB, GL_UNSIGNED_SHORT_5_6_5: %0.1f ms\n", dt);

    dt = benchmark(GL_RGB565, GL_RGB, GL_UNSIGNED_SHORT_5_6_5);
    printf("GL_RGB565, GL_RGB, GL_UNSIGNED_SHORT_5_6_5: %0.1f ms\n", dt);
#endif
}

int main(int argc, char *argv[])
{
    int mode = 0;
    if (argc >= 2) {
        mode = argv[1][0] - '0';
    }
    printf("Mode: %d\n", mode);

    screen_width = 1920;
    screen_height = 1080;
    fullscreen = 1;

    create_images();
    printf("Done\n");

    create_sdl_window(1);
    setup_opengl();

    int t0 = SDL_GetTicks();
    while (SDL_GetTicks() - t0 < 1000) {
        SDL_GL_SwapWindow(sdl_window);
        glFinish();
    }

    benchmarks();
    return 0;

    int t1 = SDL_GetTicks();
    for (int i = 0; i < 50; i++) {
        if (want_quit) {
            break;
        }
        // printf("a\n");
        handle_events();
        // printf("b\n");
        glClear(GL_COLOR_BUFFER_BIT);

        int n = frames % 3;
        glBindTexture(GL_TEXTURE_2D, gl_textures[n]);
        // glBindTexture(GL_TEXTURE_2D, gl_textures[0]);

        int u0 = SDL_GetTicks();
        int iterations = 1;
        for (int i = 0; i < iterations; i++) {
            update_texture();
            // glFinish();
        }
        int u1 = SDL_GetTicks();
        printf("   %0.1f ms ", 1.0 * (u1 - u0) / iterations);

        // printf("c\n");
        render_texture();

        // glFinish();
        int u2 = SDL_GetTicks();
        printf("-> %2d ms\n", u2 - u1);

        // glFlush();
        // glFinish();
        SDL_GL_SwapWindow(sdl_window);

        frames += 1;
    }
    int t2 = SDL_GetTicks();
    double fps = frames / ((t2 - t1) / 1000.0);
    printf("%d frames in %d ms = %0.2f FPS\n", frames, t2 - t1, fps);
}
