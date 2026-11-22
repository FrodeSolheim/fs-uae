/*
 * The test scene (see fsapp-testscene.h).
 *
 * Copyright (c) 2026 Frode Solheim
 */

#include "fsapp-testscene.h"

#include <SDL3/SDL.h>
#include <stdint.h>
#include <stdlib.h>

#include <string>
#include <vector>

#include "fsapp-adornment.h"
#include "fsapp-frame.h"
#include "fsapp-main.h"
#include "fsapp-mainthread.h"
#include "fs/path.h"
#include "fsapp-pythonthread.h"
#include "fsapp-surface.h"
#include "fsapp-time.h"

// The GL grab takes at most this many swaps (fsapp_mainthread_set_gl_grab).
#define MAX_GRABS 32

// The size of an adornment's content, which is stretched to its rectangle (one colour).
#define ADORNMENT_CONTENT_SIZE 4

// How long the stand-in UI thread's first UI frame takes at least (see
// fsapp_testscene_run_ui_frame).
#define FIRST_UI_FRAME_DELAY_NS SDL_MS_TO_NS(50)

// A generated frame is uploaded in this many slices unless the frame line says otherwise.
#define DEFAULT_SLICES 4

// The pixels after each row of a generated frame, so that its rows are further apart than the
// upload region is wide (the slices' stride), and the colour of those pixels, which never shows.
#define ROW_PADDING 3
#define ROW_PADDING_COLOR SDL_Color{255, 0, 255, 255}

// How long the stand-in pacer's wait waits when it has nothing to do: about a host refresh period.
#define PACER_WAIT_NS SDL_US_TO_NS(16667)

namespace
{

struct scene_surface {
    bool emulation;
    SDL_Rect rect;
    SDL_Color color;
    bool translucent;
    // "1 only" (fsapp's default) unless the line says scale any.
    fsapp_pixels_scale_support_t scale_support;
    // Its place in the stacking order: its role, its parent's index (a child or popup), its band,
    // and whether it is raised when created.
    fsapp_stacking_role_t role;
    int parent;
    fsapp_stacking_band_t band;
    bool raised;
    // For the emulation surface: the streamed content's filter (nearest, else linear) and format
    // (RGB565, else BGRA).
    bool nearest;
    bool rgb565;
    // Whether a delete line before the one being read deletes it (while reading the file).
    bool deleted;
};

struct scene_adornment {
    // The index of the surface it is attached to, or -1 for in front of everything.
    int surface;
    SDL_Rect rect;
    SDL_Color color;
    bool translucent;
    // Its content: premultiplied RGBA, kept for as long as the adornment lives.
    std::vector<uint8_t> content;
};

// A change that the stand-in UI thread makes in a later UI frame, to the surface with an index.
struct scene_change {
    enum kind { MOVE, SHOW, HIDE, RAISE, TRANSLUCENT, DRAW, COMMIT, RESIZE, DESTROY } kind;
    int surface;
    // MOVE: the new position.
    int x;
    int y;
    // TRANSLUCENT: on or off.
    bool on;
    // DRAW: the rectangle (surface pixels) and its colour (straight RGBA). RESIZE: the new size
    // (w, h) and the colour to fill the new buffer with.
    SDL_Rect rect;
    SDL_Color color;
    // COMMIT: the damaged rectangles (none: the whole surface).
    std::vector<SDL_Rect> damage;
};

// The change instructions, and the kinds of change they make.
const struct {
    const char *instruction;
    enum scene_change::kind kind;
} g_change_instructions[] = {
    {"move", scene_change::MOVE},
    {"show", scene_change::SHOW},
    {"hide", scene_change::HIDE},
    {"raise", scene_change::RAISE},
    {"translucent", scene_change::TRANSLUCENT},
    {"draw", scene_change::DRAW},
    {"commit", scene_change::COMMIT},
    {"resize", scene_change::RESIZE},
    {"delete", scene_change::DESTROY},
};

// The changes of an at line: made in the stand-in UI thread's first UI frame after the swap.
struct scene_group {
    int swap;
    std::vector<scene_change> changes;
};

// A stream instruction of an at line: carried out in the first frame after the swap, on the drawing
// side by the stand-in pacer's wait (FRAME) or by the scene's drawing hook before drawing below
// (PICTURE, DROP), or on the host window side by the scene's frame hook, which clears and sets the
// stand-in pacer (PAUSE, RESUME).
struct scene_stream {
    enum kind { FRAME, PICTURE, DROP, PAUSE, RESUME } kind;
    int swap;
    // FRAME: the upload region's size, the crop in it (the source rectangle) and the picture
    // rectangle (the emulation surface's coordinates), the colours, the number of slices, and how
    // many of them are uploaded before it is left uncommitted (-1: all, and committed). PICTURE:
    // the picture rectangle.
    int width;
    int height;
    SDL_Rect crop;
    SDL_Rect picture;
    SDL_Color outside;
    SDL_Color top;
    SDL_Color bottom;
    int slices;
    int stop;
};

} // namespace

struct fsapp_testscene {
    std::string path;
    int width = 640;
    int height = 480;
    SDL_Color background = {0, 0, 0, 255};
    SDL_Color fill = {0, 0, 0, 255};
    std::vector<scene_surface> surfaces;
    bool has_emulation = false;
    std::vector<scene_adornment> adornments;
    std::vector<int> grabs;
    // How long the scene keeps running after its last grab (idle), or 0 to quit at once.
    int idle_ms = 0;
    std::vector<scene_group> groups;
    // The stream instructions, in the order of the file.
    std::vector<scene_stream> stream;
    // Whether the first UI frame has made the scene, and the surfaces it made, NULL once deleted
    // (the stand-in UI thread's).
    bool created = false;
    std::vector<fsapp_surface_t *> live_surfaces;
    // The next group of changes to make (an index into groups).
    size_t next_group = 0;
    // The emulation surface once the stand-in UI thread has made it (for the stand-in pacer).
    void *emulation_surface = nullptr;
    // The next stream instruction for the host window side, the stand-in pacer's wait and the
    // drawing hook to look at, whether the stand-in pacer is set and is to be (not paused), and
    // whether the drawing side has set the content's filter.
    size_t next_host_stream = 0;
    size_t next_drawing_stream = 0;
    size_t next_drawing_hook_stream = 0;
    bool pacer_set = false;
    bool paused = false;
    bool filter_set = false;
    // The pause instruction (an index into stream) that the stand-in pacer's wait has woken the
    // main loop for, so it wakes it once per pause (drawing side).
    size_t pause_woken = SIZE_MAX;
};

// -------------------------------------------------------------------------------------------------
// Reading the scene.

namespace
{

// Reads one line's instructions; sets error (what is wrong) and returns false if it is not valid.
class line_parser
{
  public:
    line_parser(fsapp_testscene_t *scene, const std::vector<std::string> &words)
        : scene(scene), words(words)
    {
    }

    bool parse(std::string &error);

  private:
    bool parse_window(std::string &error);
    bool parse_background(std::string &error);
    bool parse_fill(std::string &error);
    bool parse_surface(bool emulation, std::string &error);
    bool parse_adornment(std::string &error);
    bool parse_grab(std::string &error);
    bool parse_at(std::string &error);
    bool parse_idle(std::string &error);
    bool parse_change(enum scene_change::kind kind, std::string &error);
    bool parse_stream(enum scene_stream::kind kind, std::string &error);

    bool surface_index(size_t index, int *value, std::string &error);
    bool integer(size_t index, int *value, std::string &error);
    bool size(size_t index, int *value, std::string &error);
    bool color(size_t index, int count, SDL_Color *color, std::string &error);
    bool rect(size_t index, SDL_Rect *rect, std::string &error);
    bool count(size_t minimum, size_t maximum, std::string &error);

    fsapp_testscene_t *scene;
    const std::vector<std::string> &words;
};

bool line_parser::integer(size_t index, int *value, std::string &error)
{
    const char *text = words[index].c_str();
    char *end = NULL;
    long result = SDL_strtol(text, &end, 10);
    if (end == text || *end != '\0' || result < INT32_MIN || result > INT32_MAX) {
        error = "not a number: " + words[index];
        return false;
    }
    *value = (int)result;
    return true;
}

bool line_parser::size(size_t index, int *value, std::string &error)
{
    if (!integer(index, value, error)) {
        return false;
    }
    if (*value <= 0) {
        error = "not a size (above 0): " + words[index];
        return false;
    }
    return true;
}

bool line_parser::color(size_t index, int count, SDL_Color *color, std::string &error)
{
    Uint8 *components[] = {&color->r, &color->g, &color->b, &color->a};
    for (int i = 0; i < count; i++) {
        int value;
        if (!integer(index + i, &value, error)) {
            return false;
        }
        if (value < 0 || value > 255) {
            error = "not a colour component (0 to 255): " + words[index + i];
            return false;
        }
        *components[i] = (Uint8)value;
    }
    return true;
}

bool line_parser::rect(size_t index, SDL_Rect *rect, std::string &error)
{
    return integer(index, &rect->x, error) && integer(index + 1, &rect->y, error) &&
           size(index + 2, &rect->w, error) && size(index + 3, &rect->h, error);
}

bool line_parser::surface_index(size_t index, int *value, std::string &error)
{
    if (!integer(index, value, error)) {
        return false;
    }
    if (*value < 0 || *value >= (int)scene->surfaces.size()) {
        error = "no surface " + words[index] + " earlier in the file";
        return false;
    }
    return true;
}

bool line_parser::count(size_t minimum, size_t maximum, std::string &error)
{
    if (words.size() < minimum || words.size() > maximum) {
        error = "wrong number of arguments";
        return false;
    }
    return true;
}

bool line_parser::parse_window(std::string &error)
{
    return count(3, 3, error) && size(1, &scene->width, error) && size(2, &scene->height, error);
}

bool line_parser::parse_background(std::string &error)
{
    SDL_Color color = {0, 0, 0, 255};
    if (!count(4, 5, error) || !this->color(1, (int)words.size() - 1, &color, error)) {
        return false;
    }
    scene->background = color;
    return true;
}

bool line_parser::parse_fill(std::string &error)
{
    SDL_Color color = {0, 0, 0, 255};
    if (!count(4, 4, error) || !this->color(1, 3, &color, error)) {
        return false;
    }
    scene->fill = color;
    return true;
}

bool line_parser::parse_surface(bool emulation, std::string &error)
{
    scene_surface surface = {};
    surface.emulation = emulation;
    surface.role = FSAPP_STACKING_ROLE_TOP_LEVEL;
    surface.parent = -1;
    surface.band = FSAPP_STACKING_BAND_NORMAL;
    size_t index = 1;
    if (words.size() < index + 4 || !rect(index, &surface.rect, error)) {
        error = error.empty() ? "wrong number of arguments" : error;
        return false;
    }
    index += 4;
    if (!emulation) {
        if (words.size() < index + 4 || !color(index, 4, &surface.color, error)) {
            error = error.empty() ? "wrong number of arguments" : error;
            return false;
        }
        index += 4;
    } else if (scene->has_emulation) {
        error = "a second emulation surface";
        return false;
    }
    while (index < words.size()) {
        if (!emulation && words[index] == "translucent") {
            surface.translucent = true;
            index += 1;
        } else if (!emulation && words[index] == "scale" && index + 1 < words.size()) {
            const std::string &support = words[index + 1];
            if (support != "one" && support != "any") {
                error = "not a scale support: " + support;
                return false;
            }
            surface.scale_support =
                support == "any" ? FSAPP_PIXELS_SCALE_ANY : FSAPP_PIXELS_SCALE_ONE;
            index += 2;
        } else if (emulation && (words[index] == "nearest" || words[index] == "linear")) {
            surface.nearest = words[index] == "nearest";
            index += 1;
        } else if (emulation && words[index] == "rgb565") {
            surface.rgb565 = true;
            index += 1;
        } else if (words[index] == "band" && index + 1 < words.size()) {
            const std::string &band = words[index + 1];
            if (band == "background") {
                surface.band = FSAPP_STACKING_BAND_BACKGROUND;
            } else if (band == "normal") {
                surface.band = FSAPP_STACKING_BAND_NORMAL;
            } else if (band == "panel") {
                surface.band = FSAPP_STACKING_BAND_PANEL;
            } else {
                error = "not a band: " + band;
                return false;
            }
            index += 2;
        } else if ((words[index] == "child" || words[index] == "popup") &&
                   index + 1 < words.size()) {
            surface.role =
                words[index] == "child" ? FSAPP_STACKING_ROLE_CHILD : FSAPP_STACKING_ROLE_POPUP;
            if (!surface_index(index + 1, &surface.parent, error)) {
                return false;
            }
            index += 2;
        } else if (words[index] == "overlay") {
            surface.role = FSAPP_STACKING_ROLE_OVERLAY;
            index += 1;
        } else if (words[index] == "raise") {
            surface.raised = true;
            index += 1;
        } else {
            error = "unknown option: " + words[index];
            return false;
        }
    }
    scene->surfaces.push_back(surface);
    scene->has_emulation = scene->has_emulation || emulation;
    return true;
}

bool line_parser::parse_adornment(std::string &error)
{
    scene_adornment adornment = {};
    if (!count(10, 11, error)) {
        return false;
    }
    if (words[1] == "all") {
        adornment.surface = -1;
    } else {
        if (!surface_index(1, &adornment.surface, error)) {
            return false;
        }
    }
    if (!rect(2, &adornment.rect, error) || !color(6, 4, &adornment.color, error)) {
        return false;
    }
    if (words.size() == 11) {
        if (words[10] != "translucent") {
            error = "unknown option: " + words[10];
            return false;
        }
        adornment.translucent = true;
    }
    scene->adornments.push_back(adornment);
    return true;
}

bool line_parser::parse_grab(std::string &error)
{
    if (words.size() < 2) {
        error = "no swap to grab";
        return false;
    }
    for (size_t i = 1; i < words.size(); i++) {
        int swap;
        if (!size(i, &swap, error)) {
            return false;
        }
        if (!scene->grabs.empty() && swap <= scene->grabs.back()) {
            error = "swaps not in increasing order: " + words[i];
            return false;
        }
        if (scene->grabs.size() >= MAX_GRABS) {
            error = "more than " + std::to_string(MAX_GRABS) + " swaps to grab";
            return false;
        }
        scene->grabs.push_back(swap);
    }
    return true;
}

bool line_parser::parse_at(std::string &error)
{
    scene_group group = {};
    if (!count(2, 2, error) || !integer(1, &group.swap, error)) {
        return false;
    }
    if (group.swap < 0) {
        error = "not a swap (0 or above): " + words[1];
        return false;
    }
    if (!scene->groups.empty() && group.swap <= scene->groups.back().swap) {
        error = "swap not after the last at line's: " + words[1];
        return false;
    }
    scene->groups.push_back(group);
    return true;
}

bool line_parser::parse_idle(std::string &error)
{
    return count(2, 2, error) && size(1, &scene->idle_ms, error);
}

bool line_parser::parse_change(enum scene_change::kind kind, std::string &error)
{
    scene_change change = {};
    change.kind = kind;
    switch (kind) {
    case scene_change::MOVE:
        if (!count(4, 4, error) || !integer(2, &change.x, error) || !integer(3, &change.y, error)) {
            return false;
        }
        break;
    case scene_change::TRANSLUCENT:
        if (!count(3, 3, error)) {
            return false;
        }
        if (words[2] != "on" && words[2] != "off") {
            error = "not on or off: " + words[2];
            return false;
        }
        change.on = words[2] == "on";
        break;
    case scene_change::DRAW:
        if (!count(10, 10, error) || !rect(2, &change.rect, error) ||
            !color(6, 4, &change.color, error)) {
            return false;
        }
        break;
    case scene_change::COMMIT:
        if (words.size() < 2 || (words.size() - 2) % 4 != 0) {
            error = "wrong number of arguments";
            return false;
        }
        // Any rectangle, also empty or outside the surface (fsapp clips them).
        for (size_t i = 2; i < words.size(); i += 4) {
            SDL_Rect damage;
            if (!integer(i, &damage.x, error) || !integer(i + 1, &damage.y, error) ||
                !integer(i + 2, &damage.w, error) || !integer(i + 3, &damage.h, error)) {
                return false;
            }
            change.damage.push_back(damage);
        }
        break;
    case scene_change::RESIZE:
        if (!count(8, 8, error) || !size(2, &change.rect.w, error) ||
            !size(3, &change.rect.h, error) || !color(4, 4, &change.color, error)) {
            return false;
        }
        break;
    case scene_change::SHOW:
    case scene_change::HIDE:
    case scene_change::RAISE:
    case scene_change::DESTROY:
        if (!count(2, 2, error)) {
            return false;
        }
        break;
    }
    if (!surface_index(1, &change.surface, error)) {
        return false;
    }
    if ((change.kind == scene_change::DRAW || change.kind == scene_change::COMMIT) &&
        scene->surfaces[change.surface].emulation) {
        error = "the emulation surface has no buffer";
        return false;
    }
    scene_surface &item = scene->surfaces[change.surface];
    if (item.deleted) {
        error = "surface " + words[1] + " is deleted earlier in the file";
        return false;
    }
    if (change.kind == scene_change::DESTROY) {
        item.deleted = true;
    }
    if (scene->groups.empty()) {
        error = "a change before the first at line";
        return false;
    }
    scene->groups.back().changes.push_back(change);
    return true;
}

bool line_parser::parse_stream(enum scene_stream::kind kind, std::string &error)
{
    scene_stream stream = {};
    stream.kind = kind;
    stream.slices = DEFAULT_SLICES;
    stream.stop = -1;
    if (kind == scene_stream::FRAME) {
        // frame <w> <h> crop <x> <y> <w> <h> picture <x> <y> <w> <h> <outside r g b>
        // <top r g b> <bottom r g b> [slices <n>] [stop <k>]
        if (words.size() < 22 || words[3] != "crop" || words[8] != "picture") {
            error = "wrong number of arguments";
            return false;
        }
        if (!size(1, &stream.width, error) || !size(2, &stream.height, error) ||
            !rect(4, &stream.crop, error) || !rect(9, &stream.picture, error)) {
            return false;
        }
        stream.outside.a = stream.top.a = stream.bottom.a = 255;
        if (!color(13, 3, &stream.outside, error) || !color(16, 3, &stream.top, error) ||
            !color(19, 3, &stream.bottom, error)) {
            return false;
        }
        const SDL_Rect &crop = stream.crop;
        if (crop.x < 0 || crop.y < 0 || crop.x + crop.w > stream.width ||
            crop.y + crop.h > stream.height) {
            error = "the crop is not inside the frame";
            return false;
        }
        for (size_t index = 22; index < words.size(); index += 2) {
            if (index + 1 >= words.size()) {
                error = "unknown option: " + words[index];
                return false;
            }
            if (words[index] == "slices") {
                if (!size(index + 1, &stream.slices, error)) {
                    return false;
                }
            } else if (words[index] == "stop") {
                if (!integer(index + 1, &stream.stop, error)) {
                    return false;
                }
                if (stream.stop < 0) {
                    error = "not a number of slices (0 or above): " + words[index + 1];
                    return false;
                }
            } else {
                error = "unknown option: " + words[index];
                return false;
            }
        }
        if (stream.slices > stream.height) {
            error = "more slices than rows";
            return false;
        }
    } else if (kind == scene_stream::PICTURE) {
        // picture <x> <y> <w> <h>
        if (!count(5, 5, error) || !rect(1, &stream.picture, error)) {
            return false;
        }
    } else if (!count(1, 1, error)) {
        return false;
    }
    if (!scene->has_emulation) {
        error = "no emulation surface earlier in the file";
        return false;
    }
    if (scene->groups.empty()) {
        error = "a change before the first at line";
        return false;
    }
    stream.swap = scene->groups.back().swap;
    scene->stream.push_back(stream);
    return true;
}

bool line_parser::parse(std::string &error)
{
    const std::string &instruction = words[0];
    if (instruction == "window") {
        return parse_window(error);
    } else if (instruction == "background") {
        return parse_background(error);
    } else if (instruction == "fill") {
        return parse_fill(error);
    } else if (instruction == "surface") {
        return parse_surface(false, error);
    } else if (instruction == "emulation") {
        return parse_surface(true, error);
    } else if (instruction == "adornment") {
        return parse_adornment(error);
    } else if (instruction == "grab") {
        return parse_grab(error);
    } else if (instruction == "at") {
        return parse_at(error);
    } else if (instruction == "idle") {
        return parse_idle(error);
    } else if (instruction == "frame") {
        return parse_stream(scene_stream::FRAME, error);
    } else if (instruction == "picture") {
        return parse_stream(scene_stream::PICTURE, error);
    } else if (instruction == "drop") {
        return parse_stream(scene_stream::DROP, error);
    } else if (instruction == "pause") {
        return parse_stream(scene_stream::PAUSE, error);
    } else if (instruction == "resume") {
        return parse_stream(scene_stream::RESUME, error);
    }
    for (const auto &change : g_change_instructions) {
        if (instruction == change.instruction) {
            return parse_change(change.kind, error);
        }
    }
    error = "unknown instruction";
    return false;
}

// The words of a line, split at spaces and tabs.
std::vector<std::string> split_words(const std::string &line)
{
    std::vector<std::string> words;
    size_t pos = 0;
    while (true) {
        pos = line.find_first_not_of(" \t", pos);
        if (pos == std::string::npos) {
            break;
        }
        size_t end = line.find_first_of(" \t", pos);
        words.push_back(line.substr(pos, end == std::string::npos ? end : end - pos));
        pos = end;
    }
    return words;
}

char *format_error(const std::string &message)
{
    return SDL_strdup(message.c_str());
}

} // namespace

fsapp_testscene_t *fsapp_testscene_load(const char *path, char **error)
{
    size_t length = 0;
    char *data = (char *)SDL_LoadFile(path, &length);
    if (data == NULL) {
        *error = format_error(std::string("The test scene ") + path +
                              " cannot be read: " + SDL_GetError());
        return NULL;
    }
    std::string text(data, length);
    SDL_free(data);

    fsapp_testscene_t *scene = new fsapp_testscene_t();
    scene->path = path;
    size_t start = 0;
    int line_number = 0;
    while (start < text.size()) {
        size_t end = text.find('\n', start);
        if (end == std::string::npos) {
            end = text.size();
        }
        std::string line = text.substr(start, end - start);
        start = end + 1;
        line_number += 1;
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        std::vector<std::string> words = split_words(line);
        if (words.empty() || words[0][0] == '#') {
            continue;
        }
        std::string message;
        line_parser parser(scene, words);
        if (!parser.parse(message)) {
            *error =
                format_error(std::string("The test scene ") + path + ", line " +
                             std::to_string(line_number) + ": " + message + ": \"" + line + "\"");
            delete scene;
            return NULL;
        }
    }
    SDL_Log("Test scene %s: %d surfaces, %d adornments, %d grabs, %d groups of changes, %d stream "
            "instructions",
            path, (int)scene->surfaces.size(), (int)scene->adornments.size(),
            (int)scene->grabs.size(), (int)scene->groups.size(), (int)scene->stream.size());
    return scene;
}

void fsapp_testscene_set_up_gl_grab(const fsapp_testscene_t *scene)
{
    if (scene->grabs.empty()) {
        return;
    }
    std::string swaps;
    for (int swap : scene->grabs) {
        if (!swaps.empty()) {
            swaps += ",";
        }
        swaps += std::to_string(swap);
    }
    char *dir = FS_GetParentPath(scene->path.c_str());
    fsapp_mainthread_set_gl_grab(swaps.c_str(), dir);
    SDL_free(dir);
    // The scene file gives milliseconds.
    fsapp_mainthread_set_quit_after_gl_grab(true, (int64_t)SDL_MS_TO_NS(scene->idle_ms));
}

// -------------------------------------------------------------------------------------------------
// The stand-in UI thread.

// Creates a surface; a child's or popup's parent is among the surfaces created before it.
static fsapp_surface_t *create_surface(const fsapp_testscene_t *scene, const scene_surface *item)
{
    fsapp_surface_t *surface;
    if (item->emulation) {
        surface = fsapp_surface_create(item->rect.w, item->rect.h, true);
        // No content until the stand-in pacer commits a frame.
        fsapp_surface_attach_streamed_content(surface, item->rgb565 ? FSAPP_SURFACE_STREAM_RGB565
                                                                    : FSAPP_SURFACE_STREAM_BGRA);
        fsapp_surface_set_fill_color(surface, scene->fill);
        fsapp_surface_set_scale_support(surface, FSAPP_PIXELS_SCALE_ANY);
    } else {
        surface = fsapp_surface_create(item->rect.w, item->rect.h, false);
        SDL_Rect whole = {0, 0, item->rect.w, item->rect.h};
        fsapp_surface_draw_filled_rectangle(surface, &whole, item->color);
        fsapp_surface_set_translucent(surface, item->translucent);
        fsapp_surface_set_scale_support(surface, item->scale_support);
    }
    fsapp_surface_set_position(surface, item->rect.x, item->rect.y);
    fsapp_surface_set_role(surface, item->role,
                           item->parent >= 0 ? scene->live_surfaces[item->parent] : NULL);
    fsapp_surface_set_band(surface, item->band);
    fsapp_surface_set_visible(surface, true);
    if (item->raised) {
        fsapp_surface_raise(surface);
    }
    return surface;
}

// The colour component premultiplied by alpha, rounded.
static uint8_t premultiply(uint8_t component, uint8_t alpha)
{
    return (uint8_t)((component * alpha + 127) / 255);
}

static void create_adornment(scene_adornment *item, const std::vector<fsapp_surface_t *> &surfaces)
{
    fsapp_adornment_t *adornment =
        item->surface < 0 ? fsapp_adornment_create_in_front()
                          : fsapp_adornment_create_in_front_of(surfaces[item->surface]);
    fsapp_adornment_set_rect(adornment, item->rect.x, item->rect.y, item->rect.w, item->rect.h);
    fsapp_adornment_set_opaque(adornment, !item->translucent);
    const SDL_Color c = item->color;
    const uint8_t pixel[4] = {premultiply(c.r, c.a), premultiply(c.g, c.a), premultiply(c.b, c.a),
                              c.a};
    item->content.clear();
    for (int i = 0; i < ADORNMENT_CONTENT_SIZE * ADORNMENT_CONTENT_SIZE; i++) {
        item->content.insert(item->content.end(), pixel, pixel + 4);
    }
    // The adornment lives until the main loop ends (fsapp_adornment_quit frees it), and its
    // content with the scene.
    fsapp_adornment_commit(adornment, item->content.data(), ADORNMENT_CONTENT_SIZE,
                           ADORNMENT_CONTENT_SIZE, ADORNMENT_CONTENT_SIZE * 4, 0,
                           ADORNMENT_CONTENT_SIZE);
}

void fsapp_testscene_start(fsapp_testscene_t *scene)
{
    SDL_Log("Test scene: the stand-in UI thread starts");
    fsapp_window_t *window =
        fsapp_main_create_window("FS-UAE test scene", scene->width, scene->height, false);
    fsapp_main_set_window_background_color(window, scene->background);
}

// Makes a change with the same calls as fsgui.
static void make_change(fsapp_testscene_t *scene, const scene_change &change)
{
    fsapp_surface_t *surface = scene->live_surfaces[change.surface];
    switch (change.kind) {
    case scene_change::MOVE:
        fsapp_surface_set_position(surface, change.x, change.y);
        break;
    case scene_change::SHOW:
        fsapp_surface_set_visible(surface, true);
        break;
    case scene_change::HIDE:
        fsapp_surface_set_visible(surface, false);
        break;
    case scene_change::RAISE:
        fsapp_surface_raise(surface);
        break;
    case scene_change::TRANSLUCENT:
        fsapp_surface_set_translucent(surface, change.on);
        break;
    case scene_change::DRAW:
        fsapp_surface_draw_filled_rectangle(surface, &change.rect, change.color);
        break;
    case scene_change::COMMIT:
        fsapp_surface_commit(surface, change.damage.data(), (int)change.damage.size());
        break;
    case scene_change::RESIZE:
        fsapp_surface_resize(surface, change.rect.w, change.rect.h);
        if (!scene->surfaces[change.surface].emulation) {
            // The new buffer, filled; it shows once committed.
            SDL_Rect whole = {0, 0, change.rect.w, change.rect.h};
            fsapp_surface_draw_filled_rectangle(surface, &whole, change.color);
        }
        break;
    case scene_change::DESTROY:
        // The scene's reference goes with it.
        fsapp_surface_destroy(surface);
        scene->live_surfaces[change.surface] = NULL;
        break;
    }
}

// Makes the changes of the groups whose swap has been made.
static void make_due_changes(fsapp_testscene_t *scene)
{
    int swaps = fsapp_mainthread_get_swap_count();
    while (scene->next_group < scene->groups.size() &&
           scene->groups[scene->next_group].swap <= swaps) {
        const scene_group &group = scene->groups[scene->next_group];
        SDL_Log("Test scene: the changes at swap %d, after swap %d", group.swap, swaps);
        for (const scene_change &change : group.changes) {
            make_change(scene, change);
        }
        scene->next_group += 1;
    }
}

void fsapp_testscene_run_ui_frame(fsapp_testscene_t *scene)
{
    if (!scene->created) {
        // The whole scene is made in the first UI frame, so the first swap shows all of it. The
        // first UI frame takes a while, as the Python UI's start-up does, so that a frame drawn
        // before the first record is taken in would show (the compositor test's first swap case).
        SDL_DelayNS(FIRST_UI_FRAME_DELAY_NS);
        scene->created = true;
        for (const scene_surface &item : scene->surfaces) {
            fsapp_surface_t *surface = create_surface(scene, &item);
            // Committed whole: what the first UI frame drew.
            fsapp_surface_commit(surface, NULL, 0);
            scene->live_surfaces.push_back(surface);
            if (item.emulation) {
                // Before the record that brings it: the stand-in pacer can name it from then on.
                SDL_SetAtomicPointer(&scene->emulation_surface, surface);
            }
        }
        for (scene_adornment &item : scene->adornments) {
            create_adornment(&item, scene->live_surfaces);
        }
    }
    // Later UI frames commit only what the scene's commit lines say.
    make_due_changes(scene);
    // The stand-in UI thread sleeps between UI frames: it requests a UI frame for each host window
    // frame until it has made the last group of changes, so that it sees each group's swap.
    if (scene->next_group < scene->groups.size()) {
        fsapp_pythonthread_request_ui_frame();
    }
}

// -------------------------------------------------------------------------------------------------
// The stand-in pacer.

// The scene whose stand-in pacer is set up (the hooks take no data).
static fsapp_testscene_t *g_pacer_scene;

// The next stream instruction due (its at line's swap made, by the swaps made) that a hook has not
// carried out yet, by the hook's own position in the scene's stream, which it moves past it; or
// NULL when none is due.
static const scene_stream *take_due_step(fsapp_testscene_t *scene, size_t *next, int swaps)
{
    if (*next >= scene->stream.size() || scene->stream[*next].swap > swaps) {
        return nullptr;
    }
    return &scene->stream[(*next)++];
}

// Writes a pixel of a straight colour in the content's format (BGRA: bytes B, G, R, A; RGB565: a
// native 16-bit value, the components cut to 5, 6 and 5 bits) at the pixel's bytes.
static void write_pixel(uint8_t *pixel, fsapp_surface_stream_format_t format, SDL_Color color)
{
    if (format == FSAPP_SURFACE_STREAM_RGB565) {
        uint16_t value =
            (uint16_t)(((color.r >> 3) << 11) | ((color.g >> 2) << 5) | (color.b >> 3));
        SDL_memcpy(pixel, &value, sizeof(value));
    } else {
        uint32_t value =
            0xff000000u | ((uint32_t)color.r << 16) | ((uint32_t)color.g << 8) | color.b;
        SDL_memcpy(pixel, &value, sizeof(value));
    }
}

// Carries out a frame instruction with fsapp's calls for streamed content: generates the frame,
// begins it, uploads its slices (all, or the first stop of them) and commits it (unless stopped).
// Returns whether it committed.
static bool stream_frame(fsapp_surface_t *surface, const scene_stream &frame)
{
    fsapp_surface_stream_format_t format = surface->stream_format;
    int pixel_size = fsapp_surface_get_stream_pixel_size(format);
    int stride = (frame.width + ROW_PADDING) * pixel_size;
    std::vector<uint8_t> pixels((size_t)stride * frame.height);
    const SDL_Rect &crop = frame.crop;
    for (int y = 0; y < frame.height; y++) {
        uint8_t *row = pixels.data() + (size_t)y * stride;
        for (int x = 0; x < frame.width + ROW_PADDING; x++) {
            SDL_Color color = frame.outside;
            if (x >= frame.width) {
                color = ROW_PADDING_COLOR;
            } else if (y >= crop.y && y < crop.y + crop.h && x >= crop.x && x < crop.x + crop.w) {
                // The crop's top half (its first h / 2 rows) and bottom half.
                color = y < crop.y + crop.h / 2 ? frame.top : frame.bottom;
            }
            write_pixel(row + (size_t)x * pixel_size, format, color);
        }
    }
    fsapp_surface_begin_streamed_frame(surface, frame.width, frame.height);
    for (int i = 0; i < frame.slices; i++) {
        if (frame.stop >= 0 && i >= frame.stop) {
            SDL_Log("Test scene: a frame left after %d of %d slices", i, frame.slices);
            return false;
        }
        int first = i * frame.height / frame.slices;
        int end = (i + 1) * frame.height / frame.slices;
        fsapp_surface_upload_streamed_slice(surface, first, end - first,
                                            pixels.data() + (size_t)first * stride, stride);
    }
    fsapp_surface_commit_streamed_frame(surface, &frame.crop, &frame.picture);
    return true;
}

// Whether the resume after a pause instruction is due by swaps (any thread: the stream does not
// change once the scene is read).
static bool is_resume_due(const fsapp_testscene_t *scene, const scene_stream *pause, int swaps)
{
    for (size_t i = (size_t)(pause - scene->stream.data()) + 1; i < scene->stream.size(); i++) {
        if (scene->stream[i].kind == scene_stream::RESUME) {
            return scene->stream[i].swap <= swaps;
        }
    }
    return false;
}

// The stand-in pacer's wait (drawing side): carries out the frame instructions due, and returns
// new content after a commit; otherwise waits about a host refresh period in the frame wait. A
// frame after a pause instruction that is due waits until the resume is due.
static fsapp_pacer_wait_result_t pacer_wait(void)
{
    fsapp_testscene_t *scene = g_pacer_scene;
    fsapp_surface_t *surface = (fsapp_surface_t *)SDL_GetAtomicPointer(&scene->emulation_surface);
    if (!scene->filter_set) {
        scene->filter_set = true;
        bool nearest = false;
        for (const scene_surface &item : scene->surfaces) {
            nearest = nearest || (item.emulation && item.nearest);
        }
        fsapp_surface_set_streamed_filter(surface, nearest ? FSAPP_SURFACE_STREAM_FILTER_NEAREST
                                                           : FSAPP_SURFACE_STREAM_FILTER_LINEAR);
    }
    int swaps = fsapp_mainthread_get_swap_count();
    bool committed = false;
    while (const scene_stream *step = take_due_step(scene, &scene->next_drawing_stream, swaps)) {
        if (step->kind == scene_stream::PAUSE && !is_resume_due(scene, step, swaps)) {
            // Paused, as fsemu's wait sees at once that its pacer is no longer wanted: no frame
            // after the pause is taken in until the resume is due, also when the frame hook (host
            // window side) has not cleared the pacer yet (in render thread mode the main loop
            // runs no pass per swap). The main loop is woken once, so that it clears the pacer.
            scene->next_drawing_stream--;
            if (scene->pause_woken != scene->next_drawing_stream) {
                scene->pause_woken = scene->next_drawing_stream;
                fsapp_frame_wake();
            }
            break;
        }
        if (step->kind == scene_stream::FRAME) {
            SDL_Log("Test scene: a frame at swap %d, after swap %d", step->swap, swaps);
            committed = stream_frame(surface, *step) || committed;
        }
    }
    // Frames committed before a pause are new content, as fsemu takes in what was posted before
    // it pauses.
    if (committed) {
        return FSAPP_PACER_NEW_CONTENT;
    }
    if (fsapp_frame_wait((int64_t)SDL_GetTicksNS() + (int64_t)PACER_WAIT_NS) ==
        FSAPP_FRAME_WAIT_QUIT) {
        return FSAPP_PACER_QUIT;
    }
    return FSAPP_PACER_NO_NEW_CONTENT;
}

// The scene's frame hook (host window side): carries out the pause and resume instructions due,
// and sets the stand-in pacer for the emulation surface once it exists, unless paused, or clears
// it while paused, as fsemu sets and clears its pacer.
static void pacer_frame_hook(void)
{
    fsapp_testscene_t *scene = g_pacer_scene;
    fsapp_surface_t *surface = (fsapp_surface_t *)SDL_GetAtomicPointer(&scene->emulation_surface);
    if (surface == NULL) {
        return;
    }
    int swaps = fsapp_mainthread_get_swap_count();
    while (const scene_stream *step = take_due_step(scene, &scene->next_host_stream, swaps)) {
        if (step->kind == scene_stream::PAUSE || step->kind == scene_stream::RESUME) {
            SDL_Log("Test scene: %s at swap %d, after swap %d",
                    step->kind == scene_stream::PAUSE ? "pause" : "resume", step->swap, swaps);
            scene->paused = step->kind == scene_stream::PAUSE;
        }
    }
    if (scene->paused && scene->pacer_set) {
        fsapp_mainthread_set_pacer(NULL);
        scene->pacer_set = false;
    } else if (!scene->paused && !scene->pacer_set) {
        fsapp_pacer_t pacer = {};
        pacer.wait = pacer_wait;
        pacer.surface = surface;
        // The stand-in pacer paces the loop in its wait.
        pacer.swap_interval = 0;
        fsapp_mainthread_set_pacer(&pacer);
        scene->pacer_set = true;
    }
}

// The scene's drawing hook (drawing side, before drawing below, also while paused): carries out the
// picture and drop instructions due, as fsemu refits and drops its content in its drawing hook.
static void pacer_drawing_hook(const struct fsapp_snapshot_window *window)
{
    (void)window;
    fsapp_testscene_t *scene = g_pacer_scene;
    fsapp_surface_t *surface = (fsapp_surface_t *)SDL_GetAtomicPointer(&scene->emulation_surface);
    if (surface == NULL) {
        return;
    }
    int swaps = fsapp_mainthread_get_swap_count();
    while (const scene_stream *step =
               take_due_step(scene, &scene->next_drawing_hook_stream, swaps)) {
        if (step->kind == scene_stream::PICTURE) {
            // The surface's rectangle in this frame, as fsemu fits its picture to it.
            SDL_Rect rect = {};
            if (fsapp_surface_get_frame_rectangle(surface, &rect)) {
                SDL_Log("Test scene: picture at swap %d, after swap %d, surface at %d, %d, %d x %d",
                        step->swap, swaps, rect.x, rect.y, rect.w, rect.h);
            } else {
                SDL_Log("Test scene: picture at swap %d, after swap %d, surface not in the frame",
                        step->swap, swaps);
            }
            fsapp_surface_set_streamed_picture(surface, &step->picture);
        } else if (step->kind == scene_stream::DROP) {
            SDL_Log("Test scene: drop at swap %d, after swap %d", step->swap, swaps);
            fsapp_surface_drop_streamed_content(surface);
        }
    }
}

void fsapp_testscene_set_up_pacer(fsapp_testscene_t *scene)
{
    if (scene->stream.empty()) {
        return;
    }
    g_pacer_scene = scene;
    fsapp_mainthread_add_frame_hook(pacer_frame_hook);
    fsapp_mainthread_add_drawing_hook(pacer_drawing_hook);
}
