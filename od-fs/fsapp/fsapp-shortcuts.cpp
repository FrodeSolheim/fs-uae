/*
 * The shortcut registry (fsapp-shortcuts.h, which describes the rules).
 *
 * Copyright (c) 2026 Frode Solheim
 */

#include "fsapp-shortcuts.h"

#include <bitset>
#include <vector>

namespace
{

struct registration {
    int id;
    uint32_t key;
    uint16_t modifiers;
    void *data;
};

// A key down as a shortcut, and the registration its press hit.
struct held_key {
    uint32_t key;
    int id;
};

} // namespace

struct fsapp_shortcuts {
    std::vector<registration> registrations;
    std::vector<held_key> held;
    int last_id = 0;
};

fsapp_shortcuts_t *fsapp_shortcuts_create(void)
{
    return new fsapp_shortcuts;
}

void fsapp_shortcuts_destroy(fsapp_shortcuts_t *shortcuts)
{
    delete shortcuts;
}

int fsapp_shortcuts_add(fsapp_shortcuts_t *shortcuts, uint32_t key, uint16_t modifiers, void *data)
{
    for (const registration &r : shortcuts->registrations) {
        if (r.key == key && r.modifiers == modifiers) {
            return 0;
        }
    }
    int id = ++shortcuts->last_id;
    shortcuts->registrations.push_back({id, key, modifiers, data});
    return id;
}

bool fsapp_shortcuts_remove(fsapp_shortcuts_t *shortcuts, int id)
{
    for (size_t i = 0; i < shortcuts->registrations.size(); i++) {
        if (shortcuts->registrations[i].id == id) {
            shortcuts->registrations.erase(shortcuts->registrations.begin() + i);
            return true;
        }
    }
    return false;
}

static const registration *find_registration(const fsapp_shortcuts_t *shortcuts, int id)
{
    for (const registration &r : shortcuts->registrations) {
        if (r.id == id) {
            return &r;
        }
    }
    return nullptr;
}

// The registration a press hits, or NULL.
static const registration *find_match(const fsapp_shortcuts_t *shortcuts, uint32_t key,
                                      uint16_t modifiers)
{
    const registration *best = nullptr;
    size_t best_count = 0;
    for (const registration &r : shortcuts->registrations) {
        if (r.key != key || (modifiers & r.modifiers) != r.modifiers) {
            continue;
        }
        size_t count = std::bitset<16>(r.modifiers).count();
        if (best == nullptr || count > best_count) {
            best = &r;
            best_count = count;
        }
    }
    return best;
}

static void set_hit(const fsapp_shortcuts_t *shortcuts, int id, bool activated,
                    fsapp_shortcuts_hit_t *hit)
{
    const registration *r = find_registration(shortcuts, id);
    hit->consumed = true;
    hit->activated = activated;
    hit->id = r != nullptr ? r->id : 0;
    hit->data = r != nullptr ? r->data : nullptr;
}

void fsapp_shortcuts_handle_key(fsapp_shortcuts_t *shortcuts, uint32_t key, uint16_t modifiers,
                                bool down, bool repeat, fsapp_shortcuts_hit_t *hit)
{
    *hit = {};
    for (size_t i = 0; i < shortcuts->held.size(); i++) {
        if (shortcuts->held[i].key == key) {
            // REVIEW: A guess: the release (and repeats) of a shortcut's key are consumed too, so
            // that neither the Amiga nor a surface sees a release without its press.
            set_hit(shortcuts, shortcuts->held[i].id, false, hit);
            if (!down) {
                shortcuts->held.erase(shortcuts->held.begin() + i);
            }
            return;
        }
    }
    if (!down || repeat) {
        return;
    }
    const registration *r = find_match(shortcuts, key, modifiers);
    if (r != nullptr) {
        shortcuts->held.push_back({key, r->id});
        set_hit(shortcuts, r->id, true, hit);
    }
}
