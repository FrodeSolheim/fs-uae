#include <stdlib.h>
#include <string.h>
#include <uae/uae.h>
#include <fs/fs.h>
#include "fs-uae.h"

#define MAX_LEN 1024

static void parse_option(char *key, char *value) {
    if (key[0] == 'u' && key[1] == 'a' && key[2] == 'e' && key[3] == '_') {
        static int first = 1;
        if (first) {
            fs_log("WARNING: custom uae_* options used! Your warranty is "
                    "now void! ;)\n");
            fs_log("(not that there was any warranty before...)\n");
            first = 0;
        }
        fs_strchomp(value);
        //amiga_set_hardware_option(key + 4, value);
        amiga_set_option(key + 4, value);
    }
}

static void read_custom_uae_options_from_file(FILE* f) {
    char *key = malloc(MAX_LEN);
    char *value = malloc(MAX_LEN);
    char c;
    int mode = 0;
    int index = 0;
    int bytes_read = 1;
    while (bytes_read) {
        bytes_read = fread(&c, 1, 1, f);
        if (bytes_read != 1) {
            c = '\n';
        }
        if (mode == 0) {
            // reading key
            if (c == '=') {
                key[index] = '\0';
                index = 0;
                mode = 1;
            }
            else if (c == '\n') {
                // invalid state, really
                index = 0;
            }
            else if (c != ' ' && index < MAX_LEN - 1) {
                key[index++] = c;
            }
        }
        else {
            // reading value
            if (c == '\n') {
                value[index] = '\0';
                index = 0;
                mode = 0;
                parse_option(key, value);
            }
            else if (mode == 1 && c != ' ') {
                mode = 2;
            }
            if (mode == 2 && index < MAX_LEN - 1) {
                value[index++] = c;
            }
        }
    }
    free(key);
    free(value);
}

void fs_uae_read_custom_uae_options(int argc, char **argv) {
    fs_log("read_custom_uae_options\n");
    if (g_fs_uae_config_file_path) {
        FILE *f = fs_fopen(g_fs_uae_config_file_path, "rb");
        read_custom_uae_options_from_file(f);
        fclose(f);
    }

    for (int i = 0; i < argc; i++) {
        char *arg = argv[i];
        if (!fs_str_has_prefix(arg, "--")) {
            continue;
        }
        char *key = arg + 2;
        char *value = strchr(arg, '=');
        if (value) {
            char *k = fs_strndup(key, value - key);
            fs_strdelimit (k, "-", '_');
            char *v = fs_strdup(value + 1);
            char *key_lower = fs_ascii_strdown(k, -1);
            free(k);
            parse_option(key_lower, v);
            free(key_lower);
            free(v);
        }
    }
}
