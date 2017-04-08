#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef FSE_DRIVERS

#define FSE_INTERNAL_API
#include <fs/emu/input.h>
#include <fs/log.h>
#include <stdio.h>

#define MAX_INPUT_ACTIONS 1024

struct fs_emu_action {
    char *name;
    fs_emu_input_action_function function;
    void *data;
};

static int g_actions_count;
static struct fs_emu_action g_actions_list[MAX_INPUT_ACTIONS];
static GHashTable *g_actions_table;

fs_emu_action_id fs_emu_input_action_register(
    const char *name, fs_emu_input_action_function function, void *data)
{
    fs_emu_action_id result;
    if (g_actions_count == MAX_INPUT_ACTIONS) {
        fs_log("Max number of input actions already registered\n");
        result.value = 0;
        return result;
    }
    char *name_copy = g_strdup(name);
    g_hash_table_insert(g_actions_table, name_copy,
                        GINT_TO_POINTER(g_actions_count));
    g_actions_list[g_actions_count].name = name_copy;
    g_actions_list[g_actions_count].function = function;
    g_actions_list[g_actions_count].data = data;

    result.value = g_actions_count;

    g_actions_count += 1;
    return result;
}

fs_emu_action_id fs_emu_input_action_lookup(const char *name)
{
    fs_emu_action_id result;
    result.value = GPOINTER_TO_INT(g_hash_table_lookup(g_actions_table, name));
    return result;
}

void fs_emu_input_action_execute(fs_emu_action_id action, int state)
{
    g_assert(action.value < g_actions_count);
    g_actions_list[action.value].function(
                state, g_actions_list[action.value].data);
}

static void action_none_handler(int state, void *data)
{
    printf("action_none state=%d data=%p\n", state, data);
}

static void register_actions(void)
{
    fs_emu_input_action_register("action_none", action_none_handler, NULL);
}

void fs_emu_input_action_init(void)
{
    fs_log("fs_emu_input_action_init\n");
    g_actions_table = g_hash_table_new(g_str_hash, g_str_equal);
    register_actions();
}

#endif /* FSE_DRIVERS */
