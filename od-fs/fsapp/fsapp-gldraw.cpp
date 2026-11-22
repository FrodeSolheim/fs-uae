/*
 * The compositor's GL drawing: small GLSL programs and one static vertex buffer with a unit quad,
 * with which every rectangle is drawn.
 *
 * Copyright (c) 2026 Frode Solheim
 */

#include "fsapp-gldraw.h"

#include <string>

#include "fsapp-gl.h"
#include "fsapp-mainthread.h"

// The attribute location of the unit quad's corners, bound before linking.
#define POSITION_LOCATION 0

// The unit quad, as a triangle fan from its bottom left corner (y up, as in normalized device
// coordinates): the vertex shader stretches it to the rectangle.
static const GLfloat g_unit_quad[] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f};

// The rows of the content are from the top, so the texture coordinate t is at the top of the
// rectangle at its smallest. u_texrect is the part of the texture drawn into the rectangle (s and t
// of its top left corner, and its size): the whole texture (0, 0, 1, 1) unless the drawing says
// otherwise (fsapp_gldraw_draw_texture_part). The quad is at z = 0: its depth is the depth range's
// (see set_depth).
static const char *g_vertex_shader = R"(#version 120
attribute vec2 a_position;
uniform vec4 u_rect;
uniform vec4 u_texrect;
varying vec2 v_texcoord;
void main()
{
    v_texcoord = u_texrect.xy + vec2(a_position.x, 1.0 - a_position.y) * u_texrect.zw;
    gl_Position = vec4(u_rect.xy + a_position * u_rect.zw, 0.0, 1.0);
}
)";

static const char *g_texture_opaque_shader = R"(#version 120
uniform sampler2D u_texture;
varying vec2 v_texcoord;
void main()
{
    gl_FragColor = vec4(texture2D(u_texture, v_texcoord).rgb, 1.0);
}
)";

static const char *g_texture_translucent_shader = R"(#version 120
uniform sampler2D u_texture;
varying vec2 v_texcoord;
void main()
{
    gl_FragColor = texture2D(u_texture, v_texcoord);
}
)";

// The streamed-content program: draws the front texture of streamed content (the emulated
// picture) into its picture rectangle, opaque (alpha 1; the content's fourth byte is ignored),
// sampled with the content's filter (set on the texture). The texture coordinates are clamped to
// u_clamp (s and t of the top left corner, then of the bottom right corner): the whole source
// rectangle inset by half a texel on every side, also when the picture is clipped, so that linear
// filtering never blends the picture's edges with what lies outside the source rectangle (other
// pixels of the upload region, or what the texture held before). The mapping of the source to the
// picture is unchanged (only the outer half of each edge texel samples the edge texel's centre);
// with nearest filtering the clamp changes nothing.
// When shaders come back, their passes run here, in fsapp: between the content's texture and this
// draw into the picture rectangle (fsemu or the top only chooses a shader and hands fsapp its GLSL
// and parameters).
static const char *g_streamed_shader = R"(#version 120
uniform sampler2D u_texture;
uniform vec4 u_clamp;
varying vec2 v_texcoord;
void main()
{
    vec2 texcoord = clamp(v_texcoord, u_clamp.xy, u_clamp.zw);
    gl_FragColor = vec4(texture2D(u_texture, texcoord).rgb, 1.0);
}
)";

static const char *g_fill_shader = R"(#version 120
uniform vec4 u_color;
void main()
{
    gl_FragColor = u_color;
}
)";

typedef struct {
    const char *name;
    const char *fragment_shader;
} program_source_t;

// By fsapp_gldraw_program_t.
static const program_source_t g_program_sources[FSAPP_GLDRAW_PROGRAM_COUNT] = {
    {"texture opaque", g_texture_opaque_shader},
    {"texture translucent", g_texture_translucent_shader},
    {"fill", g_fill_shader},
    {"streamed content", g_streamed_shader},
};

typedef struct {
    GLuint program;
    // The uniforms' locations (-1 for one the program does not have).
    GLint rect;
    GLint texrect;
    GLint texture;
    GLint color;
    GLint clamp;
} program_t;

static struct {
    program_t programs[FSAPP_GLDRAW_PROGRAM_COUNT];
    GLuint vertex_buffer;
    // While a program is begun: it, and the drawable size in pixels that rectangles are given in.
    const program_t *current;
    int pixel_width;
    int pixel_height;
} gldraw;

// -------------------------------------------------------------------------------------------------

static std::string get_shader_info_log(GLuint shader)
{
    GLint length = 0;
    fsapp_gl.GetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    std::string log(length > 0 ? length : 1, '\0');
    fsapp_gl.GetShaderInfoLog(shader, (GLsizei)log.size(), NULL, log.data());
    log.resize(SDL_strlen(log.c_str()));
    return log;
}

static std::string get_program_info_log(GLuint program)
{
    GLint length = 0;
    fsapp_gl.GetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    std::string log(length > 0 ? length : 1, '\0');
    fsapp_gl.GetProgramInfoLog(program, (GLsizei)log.size(), NULL, log.data());
    log.resize(SDL_strlen(log.c_str()));
    return log;
}

static char g_error_message[256];

// Compiles the shader (described as what, for the log); on failure logs its info log, deletes it
// and returns 0.
static GLuint compile_shader(GLenum type, const char *source, const char *what)
{
    GLuint shader = fsapp_gl.CreateShader(type);
    fsapp_gl.ShaderSource(shader, 1, &source, NULL);
    fsapp_gl.CompileShader(shader);
    GLint compiled = GL_FALSE;
    fsapp_gl.GetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        SDL_Log("The compositor's OpenGL %s does not compile:\n%s", what,
                get_shader_info_log(shader).c_str());
        SDL_snprintf(g_error_message, sizeof(g_error_message),
                     "The compositor's OpenGL %s does not compile (see the log)", what);
        fsapp_gl.DeleteShader(shader);
        return 0;
    }
    return shader;
}

// Links the program from the shared vertex shader and its fragment shader, with the attribute
// location bound before linking, and looks up its uniforms. On failure logs the info log and
// returns false.
static bool make_program(program_t *program, GLuint vertex_shader, const program_source_t *source)
{
    std::string what = std::string(source->name) + " fragment shader";
    GLuint fragment_shader =
        compile_shader(GL_FRAGMENT_SHADER, source->fragment_shader, what.c_str());
    if (fragment_shader == 0) {
        return false;
    }
    GLuint handle = fsapp_gl.CreateProgram();
    fsapp_gl.AttachShader(handle, vertex_shader);
    fsapp_gl.AttachShader(handle, fragment_shader);
    fsapp_gl.BindAttribLocation(handle, POSITION_LOCATION, "a_position");
    fsapp_gl.LinkProgram(handle);
    // Flagged for deletion, and deleted with the program.
    fsapp_gl.DeleteShader(fragment_shader);
    GLint linked = GL_FALSE;
    fsapp_gl.GetProgramiv(handle, GL_LINK_STATUS, &linked);
    if (!linked) {
        SDL_Log("The compositor's OpenGL %s program does not link:\n%s", source->name,
                get_program_info_log(handle).c_str());
        SDL_snprintf(g_error_message, sizeof(g_error_message),
                     "The compositor's OpenGL %s program does not link (see the log)",
                     source->name);
        fsapp_gl.DeleteProgram(handle);
        return false;
    }
    program->program = handle;
    program->rect = fsapp_gl.GetUniformLocation(handle, "u_rect");
    program->texrect = fsapp_gl.GetUniformLocation(handle, "u_texrect");
    program->texture = fsapp_gl.GetUniformLocation(handle, "u_texture");
    program->color = fsapp_gl.GetUniformLocation(handle, "u_color");
    program->clamp = fsapp_gl.GetUniformLocation(handle, "u_clamp");
    fsapp_gl.UseProgram(handle);
    if (program->texture >= 0) {
        // The texture is always on texture unit 0.
        fsapp_gl.Uniform1i(program->texture, 0);
    }
    if (program->texrect >= 0) {
        // The whole texture, unless a draw says otherwise (fsapp_gldraw_draw_texture_part).
        fsapp_gl.Uniform4f(program->texrect, 0.0f, 0.0f, 1.0f, 1.0f);
    }
    fsapp_gl.UseProgram(0);
    return true;
}

const char *fsapp_gldraw_init(void)
{
    FSAPP_MAINTHREAD_ASSERT_DRAWING_SIDE();
    GLuint vertex_shader = compile_shader(GL_VERTEX_SHADER, g_vertex_shader, "vertex shader");
    if (vertex_shader == 0) {
        return g_error_message;
    }
    for (int i = 0; i < FSAPP_GLDRAW_PROGRAM_COUNT; i++) {
        if (!make_program(&gldraw.programs[i], vertex_shader, &g_program_sources[i])) {
            fsapp_gl.DeleteShader(vertex_shader);
            return g_error_message;
        }
    }
    fsapp_gl.DeleteShader(vertex_shader);

    fsapp_gl.GenBuffers(1, &gldraw.vertex_buffer);
    fsapp_gl.BindBuffer(GL_ARRAY_BUFFER, gldraw.vertex_buffer);
    fsapp_gl.BufferData(GL_ARRAY_BUFFER, sizeof(g_unit_quad), g_unit_quad, GL_STATIC_DRAW);
    fsapp_gl.BindBuffer(GL_ARRAY_BUFFER, 0);
    SDL_Log("OpenGL programs and vertex buffer made (%d programs)", FSAPP_GLDRAW_PROGRAM_COUNT);
    return NULL;
}

// -------------------------------------------------------------------------------------------------

void fsapp_gldraw_begin_program(fsapp_gldraw_program_t program, int pixel_width, int pixel_height)
{
    SDL_assert(program >= 0 && program < FSAPP_GLDRAW_PROGRAM_COUNT);
    SDL_assert(gldraw.current == NULL);
    gldraw.current = &gldraw.programs[program];
    gldraw.pixel_width = pixel_width;
    gldraw.pixel_height = pixel_height;
    fsapp_gl.UseProgram(gldraw.current->program);
    fsapp_gl.ActiveTexture(GL_TEXTURE0);
    fsapp_gl.BindBuffer(GL_ARRAY_BUFFER, gldraw.vertex_buffer);
    fsapp_gl.VertexAttribPointer(POSITION_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    fsapp_gl.EnableVertexAttribArray(POSITION_LOCATION);
}

// Sets the rectangle (pixels, y down) uniform. The rectangle goes to the shader in
// normalized device coordinates as its bottom left corner and its size, computed as the
// compositor's immediate mode computed its corners.
static void set_rect(const SDL_FRect *rect)
{
    const double pixel_width = gldraw.pixel_width;
    const double pixel_height = gldraw.pixel_height;
    fsapp_gl.Uniform4f(gldraw.current->rect, (float)(-1.0 + 2.0 * rect->x / pixel_width),
                       (float)(-1.0 + 2.0 * (pixel_height - rect->h - rect->y) / pixel_height),
                       (float)(2.0 * rect->w / pixel_width), (float)(2.0 * rect->h / pixel_height));
}

// Places the quad (at z = 0) at the window depth: with the depth range collapsed to it, every
// fragment gets exactly that depth, so the emulation surface's fill and its streamed content store
// the same value.
static void set_depth(double depth)
{
    glDepthRange(depth, depth);
}

void fsapp_gldraw_draw_texture(GLuint texture, const SDL_FRect *rect, double depth)
{
    SDL_assert(gldraw.current != NULL && gldraw.current->texture >= 0);
    glBindTexture(GL_TEXTURE_2D, texture);
    set_rect(rect);
    set_depth(depth);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void fsapp_gldraw_draw_texture_part(GLuint texture, const SDL_FRect *rect, double depth,
                                    const SDL_FRect *part, const SDL_FRect *clamp)
{
    SDL_assert(gldraw.current != NULL && gldraw.current->texture >= 0 &&
               gldraw.current->texrect >= 0 && gldraw.current->clamp >= 0);
    glBindTexture(GL_TEXTURE_2D, texture);
    fsapp_gl.Uniform4f(gldraw.current->texrect, part->x, part->y, part->w, part->h);
    fsapp_gl.Uniform4f(gldraw.current->clamp, clamp->x, clamp->y, clamp->x + clamp->w,
                       clamp->y + clamp->h);
    set_rect(rect);
    set_depth(depth);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void fsapp_gldraw_fill_rectangle(const SDL_FRect *rect, double depth, float r, float g, float b,
                                 float a)
{
    SDL_assert(gldraw.current != NULL && gldraw.current->color >= 0);
    fsapp_gl.Uniform4f(gldraw.current->color, r, g, b, a);
    set_rect(rect);
    set_depth(depth);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void fsapp_gldraw_end_program(void)
{
    SDL_assert(gldraw.current != NULL);
    glDepthRange(0.0, 1.0);
    fsapp_gl.DisableVertexAttribArray(POSITION_LOCATION);
    fsapp_gl.BindBuffer(GL_ARRAY_BUFFER, 0);
    fsapp_gl.UseProgram(0);
    gldraw.current = NULL;
}

// -------------------------------------------------------------------------------------------------

void fsapp_gldraw_check_errors(const char *step)
{
#if SDL_ASSERT_LEVEL >= 2
    // At most a few: a lost context can report an error for ever.
    for (int i = 0; i < 8; i++) {
        GLenum error = glGetError();
        if (error == GL_NO_ERROR) {
            break;
        }
        SDL_Log("OpenGL error 0x%04x after %s", error, step);
    }
#else
    (void)step;
#endif
}
