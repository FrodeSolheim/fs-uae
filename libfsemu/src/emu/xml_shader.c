#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef WITH_XML_SHADER

#include <fs/emu.h>
#include "xml_shader.h"

#include <fs/ml/opengl.h>

#include <fs/emu.h>
#include <fs/glib.h>
#include <fs/i18n.h>
#include <fs/log.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libfsemu.h"

#ifdef USE_GLIB
#include <glib.h>
#endif
#ifdef USE_SDL2
#include <SDL.h>
#endif
#ifdef WITH_LUA
#include "emu_lua.h"
#endif

#define debug_printf(format, ...)
//#define debug_printf printf

#define FILTERING_NOTSET 0
#define FILTERING_NEAREST 1
#define FILTERING_LINEAR 2

#define SCALING_NOTSET 0
#define SCALING_FIXED 1
#define SCALING_INPUT 2
#define SCALING_OUTPUT 3

typedef struct shader_pass {
    int filtering;
    int hor_scale_method;
    float hor_scale_value;
    int ver_scale_method;
    float ver_scale_value;
    GLuint program;
    GLuint texture;
    GLuint frame_buffer;
} shader_pass;

typedef struct fs_emu_shader {
    char *path;
    int ok;
    GList *passes;
    GList *current_shaders;
} fs_emu_shader;

//static int g_shader_ok;
//static GList *g_shader_passes = NULL;
//static GList *g_current_shaders = NULL;
static fs_emu_shader *g_active_shader = NULL;

#define MAX_TEXT_SIZE 65536

typedef struct {
    gint tag_count;
} CounterData;

#define A_FILTER     (1 << 0)
#define A_SIZE_X     (1 << 1)
#define A_SIZE_Y     (1 << 2)
#define A_SIZE       (1 << 3)
#define A_SCALE_X    (1 << 4)
#define A_SCALE_Y    (1 << 5)
#define A_SCALE      (1 << 6)
#define A_OUTSCALE_X (1 << 7)
#define A_OUTSCALE_Y (1 << 8)
#define A_OUTSCALE   (1 << 9)

typedef struct parse_data {
    char *buffer;
    int length;
    int error;

    int flags;
    char *filter;
    int size_x;
    int size_y;
    int size;
    float scale_x;
    float scale_y;
    float scale;
    float outscale_x;
    float outscale_y;
    float outscale;

    fs_emu_shader *shader;
} parse_data;

#ifdef USE_GLEE
#elif defined(WITH_GLEW)
#elif defined(WITH_GLAD)
#else
static PFNGLGETSHADERIVPROC glGetShaderiv;
static PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
static PFNGLCREATESHADERPROC glCreateShader;
static PFNGLSHADERSOURCEPROC glShaderSource;
static PFNGLCOMPILESHADERPROC glCompileShader;
static PFNGLCREATEPROGRAMPROC glCreateProgram;
static PFNGLATTACHSHADERPROC glAttachShader;
static PFNGLDELETESHADERPROC glDeleteShader;
static PFNGLLINKPROGRAMPROC glLinkProgram;
static PFNGLGETPROGRAMIVPROC glGetProgramiv;
static PFNGLDELETEPROGRAMPROC glDeleteProgram;
static PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
static PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
static PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
static PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;
static PFNGLUSEPROGRAMPROC glUseProgram;
static PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
static PFNGLUNIFORM1IPROC glUniform1i;
static PFNGLUNIFORM2FPROC glUniform2f;
#endif

static void on_start_element(GMarkupParseContext *context,
        const gchar *element_name, const gchar **attribute_names,
        const gchar **attribute_values, gpointer user_data, GError **error)
{
    debug_printf("element %s\n", element_name);

    if (strcmp(element_name, "fragment") == 0) {
    }
    else if (strcmp(element_name, "vertex") == 0) {
    }
    else {
        return;
    }

    parse_data *data = user_data;
    data->length = 0;
    data->flags = 0;
    const gchar **v = attribute_values;
    for (const gchar **k = attribute_names; *k; k++, v++) {
        if (strcmp(*k, "filter") == 0) {
            data->filter = g_strdup(*v);
            data->flags |= A_FILTER;
        }
        else if (strcmp(*k, "size_x") == 0) {
            data->size_x = atoi(*v);
            data->flags |= A_SIZE_X;
        }
        else if (strcmp(*k, "size_y") == 0) {
            data->size_y = atoi(*v);
            data->flags |= A_SIZE_Y;
        }
        else if (strcmp(*k, "size") == 0) {
            data->size = atoi(*v);
            data->flags |= A_SIZE;
        }
        else if (strcmp(*k, "scale_x") == 0) {
            data->scale_x = atof(*v);
            data->flags |= A_SCALE_X;
        }
        else if (strcmp(*k, "scale_y") == 0) {
            data->scale_y = atof(*v);
            data->flags |= A_SCALE_Y;
        }
        else if (strcmp(*k, "scale") == 0) {
            data->scale = atof(*v);
            data->flags |= A_SCALE;
        }
        else if (strcmp(*k, "outscale_x") == 0) {
            data->outscale_x = atof(*v);
            data->flags |= A_OUTSCALE_X;
        }
        else if (strcmp(*k, "outscale_y") == 0) {
            data->outscale_y = atof(*v);
            data->flags |= A_OUTSCALE_Y;
        }
        else if (strcmp(*k, "outscale") == 0) {
            data->outscale = atof(*v);
            data->flags |= A_OUTSCALE;
        }
    }
}

static void on_text(GMarkupParseContext *context, const gchar *text,
        gsize text_len, gpointer user_data, GError **error) {
    char *value = g_strndup(text, text_len);
    g_strstrip(value);
    int value_len = strlen(value);
    //debug_printf("text: (%d) %s\n", value_len, value);
    if (value_len > 0) {
        parse_data *data = user_data;
        if (data->length + value_len > MAX_TEXT_SIZE) {
            fs_log("[SHADERS] Shader text length is too big\n");
            data->error = 1;
        }
        else {
            if (!data->error) {
                strncpy(data->buffer + data->length, value, value_len);
                data->length += value_len;
            }
        }
    }
    g_free(value);
}

static void log_shader_log(int shader) {
    GLint info_length = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_length);
    if (info_length > 0) {
        char *buffer = (char *) malloc(info_length + 1);
        GLsizei length;
        glGetShaderInfoLog(shader, info_length, &length, buffer);
        buffer[length] = 0;
        fs_emu_log("%s\n", buffer);
        free(buffer);
    }
    else {
        fs_log("[SHADERS] GL_INFO_LENGTH was %d\n", info_length);
    }
}

#define HASATTR(name) ((data->flags & A_ ## name))

static void handle_element(parse_data *data, const char *element,
        const char *text) {
    if (data->error) {
        return;
    }
    //int node = 0;
    int vertex_shader = 0;
    //int fragment_shader = 0;
    if (strcmp(element, "vertex") == 0) {
        vertex_shader = 1;
    }
    else if (strcmp(element, "fragment") == 0) {
        //fragment_shader = 1;
    }
    else {
        // If the element's name is not vertex or fragment, end this
        // iteration of the loop and go immediately to the next iteration.
        return;
    }
    if (vertex_shader) {
        if (data->shader->current_shaders != NULL) {
            // If "the current list of shaders" is not empty, this shader
            // file is invalid. Abort this algorithm, do not continue
            // trying to load the file. Host applications should alert
            // the user that there was a problem loading the shader.
            fs_emu_warning("Error (1) loading shader");
            data->error = 1;
            return;
        }

        // Take the text content of this element, compile it as a vertex
        // shader.

        const GLchar *source = data->buffer;
        GLuint shader = glCreateShader(GL_VERTEX_SHADER);
        CHECK_GL_ERROR();
        glShaderSource(shader, 1, &source, NULL);
        CHECK_GL_ERROR();
        glCompileShader(shader);
        CHECK_GL_ERROR();

        // Check the compilation status of the new shader. If compilation
        // failed, this shader file is invalid. Abort this algorithm, do not
        // continue trying to load the file. Host applications should alert the
        // user that there was a problem loading the file.

        GLint compile_status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
        CHECK_GL_ERROR();
        log_shader_log(shader);
        if (compile_status == GL_FALSE) {
            fs_emu_warning("[SHADERS] Failed to compile vertex shader");
            data->error = 1;
            return;
        }
        else {
            fs_log("[SHADERS] Compiled vertex shader successfully\n");
        }
        data->shader->current_shaders = g_list_append(
                data->shader->current_shaders,
                FS_UINT_TO_POINTER(shader));
        return;
    }
    // If the element has more than one attribute from the set
    // {size, size_x, scale, scale_x, outscale, outscale_x}, this shader file
    // is invalid. Abort this algorithm, do not continue trying to load the
    // file. Host applications should alert the user that there was a
    // problem loading the file.
    int count = 0;
    if (HASATTR(SIZE)) count++;
    if (HASATTR(SIZE_X)) count++;
    if (HASATTR(SCALE)) count++;
    if (HASATTR(SCALE_X)) count++;
    if (HASATTR(OUTSCALE)) count++;
    if (HASATTR(OUTSCALE_X)) count++;
    if (count > 1) {
        fs_emu_warning("[SHADERS] Error (3) loading shader");
        data->error = 1;
        return;
    }

    // If the element has more than one attribute from the set
    // {size, size_y, scale, scale_y, outscale, outscale_y}, this shader file
    // is invalid. Abort this algorithm, do not continue trying to load the
    // file. Host applications should alert the user that there was a
    // problem loading the file.
    count = 0;
    if (HASATTR(SIZE)) count++;
    if (HASATTR(SIZE_Y)) count++;
    if (HASATTR(SCALE)) count++;
    if (HASATTR(SCALE_Y)) count++;
    if (HASATTR(OUTSCALE)) count++;
    if (HASATTR(OUTSCALE_Y)) count++;
    if (count > 1) {
        fs_emu_warning("[SHADERS] Error (4) loading shader");
        data->error = 1;
        return;
    }

    int cur_hor_scale_method = SCALING_NOTSET;
    float cur_hor_scale_value = 0;
    if (HASATTR(SIZE)) {
        cur_hor_scale_method = SCALING_FIXED;
        cur_hor_scale_value = data->size;
    }
    else if (HASATTR(SIZE_X)) {
        cur_hor_scale_method = SCALING_FIXED;
        cur_hor_scale_value = data->size_x;
    }
    else if (HASATTR(SCALE)) {
        cur_hor_scale_method = SCALING_INPUT;
        cur_hor_scale_value = data->scale;
    }
    else if (HASATTR(SCALE_X)) {
        cur_hor_scale_method = SCALING_INPUT;
        cur_hor_scale_value = data->scale_x;
    }
    else if (HASATTR(OUTSCALE)) {
        cur_hor_scale_method = SCALING_OUTPUT;
        cur_hor_scale_value = data->outscale;
    }
    else if (HASATTR(OUTSCALE_X)) {
        cur_hor_scale_method = SCALING_OUTPUT;
        cur_hor_scale_value = data->outscale_x;
    }

    int cur_ver_scale_method = SCALING_NOTSET;
    float cur_ver_scale_value = 0;
    if (HASATTR(SIZE)) {
        cur_ver_scale_method = SCALING_FIXED;
        cur_ver_scale_value = data->size;
    }
    else if (HASATTR(SIZE_Y)) {
        cur_ver_scale_method = SCALING_FIXED;
        cur_ver_scale_value = data->size_y;
    }
    else if (HASATTR(SCALE)) {
        cur_ver_scale_method = SCALING_INPUT;
        cur_ver_scale_value = data->scale;
    }
    else if (HASATTR(SCALE_Y)) {
        cur_ver_scale_method = SCALING_INPUT;
        cur_ver_scale_value = data->scale_y;
    }
    else if (HASATTR(OUTSCALE)) {
        cur_ver_scale_method = SCALING_OUTPUT;
        cur_ver_scale_value = data->outscale;
    }
    else if (HASATTR(OUTSCALE_Y)) {
        cur_ver_scale_method = SCALING_OUTPUT;
        cur_ver_scale_value = data->outscale_y;
    }

    // Take the text content of this element, compile it as a fragment shader

    const GLchar *source = data->buffer;
    GLuint shader = glCreateShader(GL_FRAGMENT_SHADER);
    CHECK_GL_ERROR();
    glShaderSource(shader, 1, &source, NULL);
    CHECK_GL_ERROR();
    glCompileShader(shader);
    CHECK_GL_ERROR();

    // Check the compilation status of the new shader. If compilation
    // failed, this shader file is invalid. Abort this algorithm, do not
    // continue trying to load the file. Host applications should alert the
    // user that there was a problem loading the file.

    GLint compile_status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
    CHECK_GL_ERROR();
    log_shader_log(shader);
    if (compile_status == GL_FALSE) {
        fs_emu_warning("[SHADERS] Failed to compile fragment shader");
        data->error = 1;
        return;
    }
    else {
        fs_log("[SHADERS] Compiled fragment shader successfully\n");
    }

    // Add the shader handle to the current list of shaders.

    data->shader->current_shaders = g_list_append(
            data->shader->current_shaders,
            FS_UINT_TO_POINTER(shader));

    // Take the shader-handles from the current list of shaders and link
    // them into a shader program.

    GLuint program = glCreateProgram();
    CHECK_GL_ERROR();
    if (program == 0) {
        fs_emu_warning("[SHADERS] Failed to create shader program");
        data->error = 1;
        return;
    }

    GList *link = data->shader->current_shaders;
    while (link) {
        GLuint shader = GPOINTER_TO_UINT(link->data);
        glAttachShader(program, shader);
        // schedule shader for deletion so it will be deleted when the
        // program is deleted
        glDeleteShader(shader);
        GList *temp = link;
        link = link->next;
        g_list_free_1(temp);
    }
    data->shader->current_shaders = NULL;

    glLinkProgram(program);
    CHECK_GL_ERROR();

    // Validate the new shader program. If validation failed, this shader
    // file is invalid. Abort this algorithm, do not continue trying to load
    // the file. Host applications should alert the user that there was a
    // problem loading the file.

    // Check the link status of the new shader program. If linking failed,
    // this shader file is invalid. Abort this algorithm, do not continue
    // trying to load the file. Host applications should alert the user
    // that there was a problem loading the file.

    GLint link_status;
    glGetProgramiv(program, GL_LINK_STATUS, &link_status);
    CHECK_GL_ERROR();
    if (link_status == GL_FALSE) {
        fs_emu_warning("[SHADERS] Failed to link shader pass");
        data->error = 1;
        return;
    } else {
        fs_log("[SHADERS] linked shader program successfully\n");
    }

    // Create a new shader pass from the new shader program handle,
    // the value of the filter attribute of the current element, the
    // current horizontal scale method, the current horizontal scale value,
    // the current vertical scale method and the current vertical scale value.

    shader_pass *pass = g_new0(shader_pass, 1);
    pass->program = program;
    pass->ver_scale_method = cur_ver_scale_method;
    pass->ver_scale_value = cur_ver_scale_value;
    pass->hor_scale_method = cur_hor_scale_method;
    pass->hor_scale_value = cur_hor_scale_value;
    pass->filtering = FILTERING_NOTSET;
    if (data->filter) {
        if (strcmp(data->filter, "nearest") == 0) {
            pass->filtering = FILTERING_NEAREST;
        }
        else if (strcmp(data->filter, "linear") == 0) {
            pass->filtering = FILTERING_LINEAR;
        }
    }
    // Add the shader pass to the list of shader passes.
    data->shader->passes = g_list_append(data->shader->passes, pass);
}

static void on_end_element(
        GMarkupParseContext *context, const gchar *element_name,
        gpointer user_data, GError **error)
{
    debug_printf("[SHADERS] on_end_element %s\n", element_name);

    if (strcmp(element_name, "fragment") == 0) {
    } else if (strcmp(element_name, "vertex") == 0) {
    } else {
        return;
    }

    parse_data *data = user_data;
    data->buffer[data->length] = 0;
    if (data->error) {

    }
    else {
        debug_printf("%s\n", data->buffer);
        handle_element(data, element_name, data->buffer);
    }
    if (data->filter) {
        g_free(data->filter);
        data->filter = NULL;
    }
}

static void on_error(
        GMarkupParseContext *context, GError *error, gpointer user_data)
{
    fs_log("[SHADERS] on_error\n");
}

static GMarkupParser counter_subparser = {
        on_start_element, on_end_element, on_text,
        NULL, on_error };

void fs_emu_load_shader(fs_emu_shader *shader);

static void context_notification_handler(int notification, void *data)
{
    static int recreate = 0;
    fs_emu_shader *shader = (fs_emu_shader *) data;
    if (notification == FS_GL_CONTEXT_DESTROY) {
        fs_log("[SHADERS] FS_GL_CONTEXT_DESTROY handler for shader\n");
        GList *link = shader->passes;
        if (shader->passes == NULL) {
            return;
        }
        fs_log("[SHADERS] destroying shaders\n");
        while (link) {
            shader_pass *pass = link->data;
            glDeleteProgram(pass->program);
            g_free(pass);
            GList *delete_link = link;
            link = link->next;
            g_list_free_1(delete_link);
        }
        recreate = 1;
        shader->passes = NULL;
    }
    else if (notification == FS_GL_CONTEXT_CREATE) {
        fs_log("[SHADERS] FS_GL_CONTEXT_CREATE handler for shader\n");
        if (recreate) {
            fs_emu_load_shader(shader);
        }
    }
}

void fs_emu_load_shader(fs_emu_shader *shader)
{
    parse_data *data = g_new0(parse_data, 1);
    data->shader = shader;
    data->buffer = malloc(MAX_TEXT_SIZE);

    GMarkupParseContext *context = g_markup_parse_context_new(
            &counter_subparser, G_MARKUP_TREAT_CDATA_AS_TEXT, data, NULL);

    FILE *f = g_fopen(data->shader->path, "rb");
    if (f == NULL) {
        fs_log("[SHADERS] Could not open shader file\n");
        return;
    }
    char *buffer = malloc(8192);
    int read = fread(buffer, 1, 8192, f);
    while (read > 0) {
        g_markup_parse_context_parse(context, buffer, read, NULL);
        read = fread(buffer, 1, 8192, f);
    }
    fclose(f);
    free(buffer);

    // If the list of shader passes is empty, this shader file is invalid.
    // Abort this algorithm, do not continue trying to load the file. Host
    // applications should alert the user that there was a problem loading
    // the file.

    if (data->shader->passes == NULL) {
        fs_emu_warning("[SHADERS] No shader passes loaded");
        data->shader->ok = 0;
    } else if (data->error) {
        fs_emu_warning("[SHADERS] Error occured while loading shader");
        data->shader->ok = 0;
    } else {
        fs_log("[SHADERS] Shader OK\n");
        data->shader->ok = 1;
    }

    free(data->buffer);
    free(data);
    g_markup_parse_context_free(context);

    CHECK_GL_ERROR();

    fs_log("[SHADERS] Done loading shader\n");
    //exit(1);
}

static char *find_shader(const char *name)
{
    fs_log("[SHADERS] Checking shader %s\n", name);
    if (fs_path_exists(name)) {
        return g_strdup(name);
    }

    char *name2 = g_strconcat(name, ".shader", NULL);
    char *path = g_build_filename(fs_data_dir(), "Shaders", name2, NULL);
    g_free(name2);
    fs_log("[SHADERS] Checking shader %s\n", path);
    if (g_file_test(path, G_FILE_TEST_EXISTS)) {
        return path;
    }
    g_free(path);

    name2 = g_strconcat(name, ".shader", NULL);
    path = g_build_filename("shaders", name2, NULL);
    g_free(name2);
    fs_log("[SHADERS] Checking shader [program_data_dir]/%s\n", path);
    char *path2 = fs_get_program_data_file(path);
    g_free(path);
    if (path2) {
        return path2;
    }
    return NULL;
}

static void fs_emu_load_default_shader()
{
    const char *name = fs_config_get_const_string("shader");
    if (!name) {
        return;
    }
    char *path = find_shader(name);
    if (!path) {
        fs_emu_warning(_("Shader not found: %s"), name);
        return;
    }

    fs_emu_shader *shader= g_new0(fs_emu_shader, 1);
    shader->path = path;
    fs_emu_load_shader(shader);
    fs_gl_add_context_notification(context_notification_handler, shader);
    g_active_shader = shader;
}

static int g_frame_count = 0;

static int g_cur_input_w = 0;
static int g_cur_input_h = 0;
static int g_cur_texture_w = 0;
static int g_cur_texture_h = 0;
static GLuint g_cur_texture = 0;

static int g_orig_input_w = 0;
static int g_orig_input_h = 0;
static int g_orig_texture_w = 0;
static int g_orig_texture_h = 0;
static GLuint g_orig_texture = 0;


static int g_final_output_w = 0;
static int g_final_output_h = 0;

static int g_requires_implicit_pass = 0;

static float g_x1, g_x2, g_y1, g_y2;
static float g_alpha;
static int g_render_textured_side = 0;


static void render_quad(
        float s2, float t1, float t2, int first, int last, int flip)
{

    float x1 = -1.0;
    float x2 = 1.0;
    float y1 = -1.0;
    float y2 = 1.0;

    if (last) {
        float t3 = t1;
        t1 = t2;
        t2 = t3;

        x1 = g_x1;
        x2 = g_x2;
        y1 = g_y1;
        y2 = g_y2;

        if (g_render_textured_side) {
            fs_gl_color4f(0.33 * g_alpha, 0.33 * g_alpha, 0.33 * g_alpha,
                    g_alpha);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0, t1); glVertex3f(-1.0, g_y1, -0.1);
            glTexCoord2f(0.0, t1); glVertex3f(-1.0, g_y1,  0.0);
            glTexCoord2f(0.0, t2); glVertex3f(-1.0, g_y2,  0.0);
            glTexCoord2f(0.0, t2); glVertex3f(-1.0, g_y2, -0.1);
            glEnd();
        }
        fs_gl_color4f(g_alpha, g_alpha, g_alpha, g_alpha);
    }

    glBegin(GL_QUADS);
    glTexCoord2f(0.0, t1); glVertex2f(x1, y1);
    glTexCoord2f( s2, t1); glVertex2f(x2, y1);
    glTexCoord2f( s2, t2); glVertex2f(x2, y2);
    glTexCoord2f(0.0, t2); glVertex2f(x1, y2);
    glEnd();
}

static int round_up_pow2(int val)
{
    // simple implementation, but works
    int x = 1;
    while (x < val) {
        x = x << 1;
    }
    return x;
}

static void render_pass(shader_pass *pass, int first, int last)
{
    int output_w = -1;
    int output_h = -1;

    debug_printf("\n");

    // If the horizontal scale method is fixed, set the output width to
    // the horizontal scale value.

    if (pass->hor_scale_method == SCALING_FIXED) {
        output_w = pass->hor_scale_value;
    }

    // If the horizontal scale method is input, set the output width to the
    // horizontal component of the current input size, multiplied by
    // the horizontal scale value.

    else if (pass->hor_scale_method == SCALING_INPUT) {
        output_w = g_cur_input_w * pass->hor_scale_value;
    }

    // If the horizontal scale method is output, set the output width to the
    // horizontal component of the final output size, multiplied by the
    // horizontal scale value.

    else if (pass->hor_scale_method == SCALING_OUTPUT) {
        output_w = g_final_output_w * pass->hor_scale_value;
    }

    // If this is the first shader pass, and the output width is not yet set,
    // the host application may set the output width to an arbitrary value.

    else if (first && !last) {
        output_w = 1024;
        debug_printf("[SHADERS] setting output width to arbitrary value %d\n",
                output_w);
    }

    // If this is the last shader pass:

    if (last) {

        // If the output width is already set, set requires implicit
        // pass to True.

        if (output_w > 0) {
            g_requires_implicit_pass = 1;
        }

        // If the output width is not yet set, set the output width to
        // the horizontal component of the final output size.

        else {
            output_w = g_final_output_w;
        }
    }

    // Otherwise, set the output width to the horizontal component of
    // the current input size.

    if (output_w < 0) {
        output_w = g_cur_input_w;
    }

    // If the vertical scale method is fixed, set the output height to the
    // vertical scale value.

    if (pass->ver_scale_method == SCALING_FIXED) {
        output_h = pass->ver_scale_value;
        debug_printf("[SHADERS] pass->ver_scale_method == SCALING_FIXED, "
                "output_h = %d\n", output_h);
    }

    // If the vertical scale method is input, set the output height to the
    // vertical component of the current input size, multiplied by the
    // vertical scale value.

    else if (pass->ver_scale_method == SCALING_INPUT) {
        output_h = g_cur_input_h * pass->ver_scale_value;
        debug_printf("[SHADERS] pass->ver_scale_method == SCALING_INPUT, "
                "output_h = %d\n", output_h);
    }

    // If the vertical scale method is output, set the output height to the
    // vertical component of the final output size, multiplied by the vertical
    // scale value.

    else if (pass->ver_scale_method == SCALING_OUTPUT) {
        output_h = g_final_output_h * pass->ver_scale_value;
        debug_printf("[SHADERS] pass->ver_scale_method == SCALING_OUTPUT, "
                "output_h = %d\n", output_h);
    }

    // If this is the first shader pass, and the output height is not yet
    // set, the host application may set the output height to an arbitrary
    // value.

    else if (first && !last) {
        output_h = 1024;
        debug_printf("[SHADERS] setting output height to arbitrary value %d\n",
                output_h);
    }

    // If this is the last shader pass:

    if (last) {

        // If the output height is already set, set requires implicit pass
        // to True.

        if (output_h > 0) {
            g_requires_implicit_pass = 1;
        }

        // If the output height is not yet set, set the output height to
        // the vertical component of the final output size.

        else {
            output_h = g_final_output_h;
        }
    }

    // Otherwise, set the output height to the vertical component of the
    // current input size.

    if (output_h < 0) {
        output_h = g_cur_input_h;
    }

    // Calculate the output size by combining the output width and
    // the output height.

    // If this is not the last shader pass, or this is the last shader pass
    // and requires implicit pass is True:

    GLuint frame_buffer = 0;
    GLuint output_texture = 0;
    int output_texture_w = 0;
    int output_texture_h = 0;

    //g_requires_implicit_pass = 1;

    if (!last || g_requires_implicit_pass) {

        // Construct the output texture with a suitable set of dimensions
        // larger than the output size.

        if (!pass->texture) {
            glGenTextures(1, &pass->texture);
            CHECK_GL_ERROR();
        }
        output_texture = pass->texture;

        output_texture_w = round_up_pow2(output_w);
        output_texture_h = round_up_pow2(output_h);

        // The following is a hack to fix compatibility with
        // CRT-interlaced-halation.shader which has a faulty assumption,
        // that rubyTextureSize and rubyOriginalTextureSize will be the
        // same in the final pass. The reason they may not be the same is
        // because of the crop / zoom function, so for example, FS-UAE may
        // allocate a 1024x1024 texture, but because of cropping, a
        // 1024x512 will be big enough for subsequent passes.

        if (output_texture_h < g_orig_texture_h) {
            output_texture_h = g_orig_texture_h;
        }
        if (output_texture_w < g_orig_texture_w) {
            output_texture_w = g_orig_texture_w;
        }

        fs_gl_bind_texture(output_texture);
        // using RGBA ensures that line size in bytes is a multiple of 4
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, output_texture_w,
                output_texture_h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        CHECK_GL_ERROR();

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        fs_gl_bind_texture(0);

        // Construct a framebuffer object, and bind the output texture to it
        // as GL_COLOR_ATTACHMENT0.
        // Tell OpenGL to render to the frame-buffer object.

        if (!pass->frame_buffer) {
            //debug_printf("[SHADERS] generating frame buffer\n");
            glGenFramebuffers(1, &pass->frame_buffer);
            CHECK_GL_ERROR();
        }
        frame_buffer = pass->frame_buffer;

        glPushMatrix();
        glLoadIdentity();
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();

        debug_printf("%d %d\n", output_w, output_h);
        glViewport(0, 0, output_w, output_h);

        glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
        CHECK_GL_ERROR();

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                GL_TEXTURE_2D, output_texture, 0);
        CHECK_GL_ERROR();

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
                GL_FRAMEBUFFER_COMPLETE) {
            debug_printf("[SHADERS] fbo is not complete!\n");
        }

        glClear(GL_COLOR_BUFFER_BIT);
    }

    // Set the GL_TEXTURE_MAG_FILTER and GL_TEXTURE_MIN_FILTER parameters
    // of the current texture according to the filter setting in the current
    // shader pass.
    //glBindTexture(GL_TEXTURE_2D, g_cur_texture);
    fs_gl_bind_texture(g_cur_texture);
    CHECK_GL_ERROR();

    if (pass->filtering == FILTERING_NEAREST) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    CHECK_GL_ERROR();

    // Tell OpenGL to use the shader program handle from the current shader
    // pass as the active shader program.

    glUseProgram(pass->program);
    CHECK_GL_ERROR();

    GLint rubyFrameCount = glGetUniformLocation(pass->program,
            "rubyFrameCount");
    if (rubyFrameCount >= 0) {
        glUniform1i(rubyFrameCount, g_frame_count);
    }
    CHECK_GL_ERROR();

    // Set the uniform rubyOrigTexture to the original texture.

    GLint rubyOrigTexture = glGetUniformLocation(pass->program,
            "rubyOrigTexture");
    if (rubyOrigTexture >= 0) {
        debug_printf("[SHADERS] set rubyOrigTexture to %d\n", 1);
        glUniform1i(rubyOrigTexture, 1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, g_orig_texture);
        glActiveTexture(GL_TEXTURE0);
    }
    CHECK_GL_ERROR();

    // Set the uniform rubyOrigTextureSize to the original texture size.

    GLint rubyOrigTextureSize = glGetUniformLocation(pass->program,
            "rubyOrigTextureSize");
    if (rubyOrigTextureSize >= 0) {
        debug_printf("[SHADERS] set rubyOrigTextureSize to %dx%d\n",
                g_orig_texture_w, g_orig_texture_h);
        glUniform2f(rubyOrigTextureSize, g_orig_texture_w, g_orig_texture_h);
    }
    CHECK_GL_ERROR();

    // Set the uniform rubyOrigInputSize to the original input size.

    GLint rubyOrigInputSize = glGetUniformLocation(pass->program,
            "rubyOrigInputSize");
    if (rubyOrigInputSize >= 0) {
        debug_printf("[SHADERS] set rubyOrigInputSize to %dx%d\n",
                g_orig_input_w, g_orig_input_h);
        glUniform2f(rubyOrigInputSize, g_orig_input_w, g_orig_input_h);
    }
    CHECK_GL_ERROR();

    // Set the uniform rubyTexture to the current texture.

    GLint rubyTexture = glGetUniformLocation(pass->program, "rubyTexture");
    if (rubyTexture >= 0) {
        debug_printf("[SHADERS] set rubyTexture to %d\n", 0);
        glUniform1i(rubyTexture, 0);
    }
    CHECK_GL_ERROR();

    // Set the uniform rubyTextureSize to the current texture size.

    GLint rubyTextureSize = glGetUniformLocation(pass->program,
            "rubyTextureSize");
    if (rubyTextureSize >= 0) {
        debug_printf("[SHADERS] set rubyTextureSize to %dx%d\n",
                g_cur_texture_w, g_cur_texture_h);
        glUniform2f(rubyTextureSize, g_cur_texture_w, g_cur_texture_h);
    }
    CHECK_GL_ERROR();

    // Set the uniform rubyInputSize to the current input size.

    GLint rubyInputSize = glGetUniformLocation(pass->program,
            "rubyInputSize");
    if (rubyInputSize >= 0) {
        debug_printf("[SHADERS] set rubyInputSize to %dx%d\n",
                g_cur_input_w, g_cur_input_h);
        glUniform2f(rubyInputSize, g_cur_input_w, g_cur_input_h);
    }
    CHECK_GL_ERROR();

    // Set the uniform rubyOutputSize to the output size.

    GLint rubyOutputSize = glGetUniformLocation(pass->program,
            "rubyOutputSize");
    if (rubyOutputSize >= 0) {
        debug_printf("[SHADERS] set rubyOutputSize to %dx%d\n",
                     output_w, output_h);
        glUniform2f(rubyOutputSize, output_w, output_h);
    }
    CHECK_GL_ERROR();

    // Render a quad, textured with the current texture, with a vertex at
    // each corner of the output size.

    float s2 = (float) g_cur_input_w / (float) g_cur_texture_w;
    float t2 = (float) g_cur_input_h / (float) g_cur_texture_h;
    fs_gl_bind_texture(g_cur_texture);
    render_quad(s2, 0.0, t2, first, last && !g_requires_implicit_pass, first);
    CHECK_GL_ERROR();

    // Tell OpenGL not to use a shader program (i.e. glUseProgram(0)).

    glUseProgram(0);
    CHECK_GL_ERROR();

    // If this is not the last shader pass, or this is the last shader pass
    // and requires implicit pass is True:

    if (!last || g_requires_implicit_pass) {

        // Tell OpenGL not to use the frame-buffer object.
        debug_printf("[SHADERS] unbind framebuffer\n");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        CHECK_GL_ERROR();

        glViewport(0, 0, fs_ml_video_width(), fs_ml_video_height());
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
    }

    // Set the current input size to the current output size.

    g_cur_input_w = output_w;
    g_cur_input_h = output_h;

    // Set the current texture size to the size of the output texture.

    g_cur_texture_w = output_texture_w;
    g_cur_texture_h = output_texture_h;

    // Set the current texture to the output texture.

    g_cur_texture = output_texture;
}

int fs_emu_xml_shader_is_enabled() {
    return g_active_shader && g_active_shader->ok;
}

int fs_emu_xml_shader_render(int texture, int texture_width,
        int texture_height, int input_width, int input_height,
        int output_width, int output_height, float x1, float y1,
        float x2, float y2, int render_textured_side, float alpha) {

    if (!fs_emu_xml_shader_is_enabled()) {
        return 0;
    }

    g_x1 = x1;
    g_x2 = x2;
    g_y1 = y1;
    g_y2 = y2;
    g_alpha = alpha;
    g_render_textured_side  = render_textured_side;

    fs_gl_blending(0);
    fs_gl_texturing(1);
    fs_gl_color4f(1.0, 1.0, 1.0, 1.0);

    // Let the current input size be the dimensions of the video-frame
    // generated by the host application.
    g_cur_input_w = input_width;
    g_cur_input_h = input_height;

    // Calculate the current texture size as a suitable set of dimensions
    // larger than the current input size.

    g_cur_texture_w = texture_width;
    g_cur_texture_h = texture_height;

    // Calculate the final output size as the dimensions of the region of the
    // back-buffer where the output will be displayed to the user, measured
    // in pixels.

    g_final_output_w = output_width;
    g_final_output_h = output_height;

    // Let requires implicit pass be False.

    g_requires_implicit_pass = 0;

    // Construct the current texture, whose dimensions are the current
    // texture size, and draw the video-frame generated by the host
    // application into it.

    g_cur_texture = texture;
    debug_printf("\n\n\n\n\n");
    debug_printf("[SHADERS] cur tex %d\n", g_cur_texture);

    g_orig_texture = g_cur_texture;
    g_orig_texture_w = g_cur_texture_w;
    g_orig_texture_h = g_cur_texture_h;
    g_orig_input_w = g_cur_input_w;
    g_orig_input_h = g_cur_input_h;

    debug_printf("[SHADERS] final output: %d %d\n",
                 g_final_output_w, g_final_output_h);
    debug_printf("[SHADERS]        input: %d %d\n",
                 g_cur_input_w, g_cur_input_h);
    debug_printf("[SHADERS]      texture: %d %d\n",
                 g_cur_texture_w, g_cur_texture_h);

    // For each shader pass in the list of shader passes...
    GList *link = g_active_shader->passes;
    int first = 1;
    while (link) {
        shader_pass *pass = link->data;
        render_pass(pass, first, link->next == NULL);
        first = 0;
        link = link->next;
    }

    // If requires implicit pass is True
    if (g_requires_implicit_pass) {
        debug_printf("[SHADERS] implicit pass, cur tex %d %d %d\n",
                     g_cur_texture, g_cur_texture_w, g_cur_texture_h);
        // Render a quad, textured with the current texture, with a vertex
        // at each corner of the final output size.

        float s2 = (float) g_cur_input_w / (float) g_cur_texture_w;
        float t2 = (float) g_cur_input_h / (float) g_cur_texture_h;
        fs_gl_bind_texture(g_cur_texture);
        render_quad(s2, 0.0, t2, 0, 1, 0);
    }
    CHECK_GL_ERROR();

    g_frame_count++;

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
    CHECK_GL_ERROR();

    return 1;
}

#ifdef WITH_LUA

static int l_fs_emu_load_shader(lua_State *L) {
    const char *name = luaL_checkstring(L, 1);
    char *path = find_shader(name);
    if (!path) {
        lua_pushnil(L);
        return 1;
    }
    fs_emu_shader *shader = g_new0(fs_emu_shader, 1);
    shader->path = path;
    fs_emu_load_shader(shader);
    fs_gl_add_context_notification(context_notification_handler, shader);
    lua_pushlightuserdata(L, shader);
    luaL_newmetatable(L, "fs_emu_shader");
    lua_setmetatable(L, -2);
    return 1;
}

static int l_fs_emu_set_shader(lua_State *L) {
    if (lua_isnil(L, 1)) {
        g_active_shader = NULL;
        return 0;
    }
    fs_emu_shader *shader = luaL_checkudata(L, 1, "fs_emu_shader");
    g_active_shader = shader;
    return 0;
}

#endif

void fs_emu_xml_shader_init(void)
{
    fs_log("[SHADERS] Initialize\n");

#ifdef USE_GLEE
#elif defined(WITH_GLEW)
#elif defined(WITH_GLAD)
#else
    glGetShaderiv = SDL_GL_GetProcAddress("glGetShaderiv");
    glGetShaderInfoLog = SDL_GL_GetProcAddress("glGetShaderInfoLog");
    glCreateShader = SDL_GL_GetProcAddress("glCreateShader");
    glShaderSource = SDL_GL_GetProcAddress("glShaderSource");
    glCompileShader = SDL_GL_GetProcAddress("glCompileShader");
    glCreateProgram = SDL_GL_GetProcAddress("glCreateProgram");
    glAttachShader = SDL_GL_GetProcAddress("glAttachShader");
    glDeleteShader = SDL_GL_GetProcAddress("glDeleteShader");
    glLinkProgram = SDL_GL_GetProcAddress("glLinkProgram");
    glGetProgramiv = SDL_GL_GetProcAddress("glGetProgramiv");
    glDeleteProgram = SDL_GL_GetProcAddress("glDeleteProgram");
    glGenFramebuffers = SDL_GL_GetProcAddress("glGenFramebuffers");
    glBindFramebuffer = SDL_GL_GetProcAddress("glBindFramebuffer");
    glFramebufferTexture2D = SDL_GL_GetProcAddress("glFramebufferTexture2D");
    glCheckFramebufferStatus = SDL_GL_GetProcAddress("glCheckFramebufferStatus");
    glUseProgram = SDL_GL_GetProcAddress("glUseProgram");
    glGetUniformLocation = SDL_GL_GetProcAddress("glGetUniformLocation");
    glUniform1i = SDL_GL_GetProcAddress("glUniform1i");
    glUniform2f = SDL_GL_GetProcAddress("glUniform2f");
#endif

    fs_emu_load_default_shader();

#ifdef WITH_LUA
    lua_State *L = fs_emu_lua_state;

    lua_register(L, "fs_emu_load_shader", l_fs_emu_load_shader);
    lua_register(L, "fs_emu_set_shader", l_fs_emu_set_shader);

#endif
}

#endif /* WITH_XML_SHADER */
