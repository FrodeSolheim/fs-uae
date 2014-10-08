#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <fs/conf.h>
#include <fs/glib.h>
#include <stdlib.h>
#include <string.h>
#include "options.h"
#include "config-common.h"
#include "fs-uae.h"

#define LEN_MAX 1024

static void strip(char *dstr, const char *sstr)
{
    char *d = dstr;
    int k = 0;
    for (const char *s = sstr; *s && k < LEN_MAX; s++, k++) {
        if (*s >= 'a' && *s <= 'z') {
        } else if (*s >= 'A' && *s <= 'Z') {
        } else if (*s >= '0' && *s <= '9') {
        } else if (*s == '+') {
        } else {
            continue;
        }
        *d++ = *s;
    }
    *d = '\0';
}

bool fs_uae_values_matches(const char *str1, const char *str2)
{
    if (str1 == NULL) {
        return str2 == NULL;
    } else if (str2 == NULL) {
        return str1 == NULL;
    }
    char nrm1[LEN_MAX + 1];
    char nrm2[LEN_MAX + 1];
    strip(nrm1, str1);
    strip(nrm2, str2);
    return strcasecmp(nrm1, nrm2) == 0;
}

int fs_uae_read_memory_option(const char *name)
{
    const char *s = fs_config_get_const_string(name);
    if (!s) {
        return FS_CONFIG_NONE;
    }
    return fs_uae_memory_kb_from_string(s, 0);
}

int fs_uae_read_memory_option_small(const char *name)
{
    const char *s = fs_config_get_const_string(name);
    if (!s) {
        return FS_CONFIG_NONE;
    }
    return fs_uae_memory_kb_from_string(s, 1);
}

int fs_uae_memory_kb_from_string(const char *str, bool smallmem)
{
    int len = strlen(str);
    if (len == 0) {
        return FS_CONFIG_NONE;
    }
    char u = str[len - 1];
    int factor = 1;
    bool factor_specified = false;
    int result = FS_CONFIG_NONE;
    if (u == 'K' || u == 'k' || u == 'M' || u == 'm') {
        factor_specified = true;
        if (u == 'M' || u == 'm') {
            factor = 1024;
        }
#if 0
        char *s2 = strdup(str);
        s2[len - 1] = '\0';
        result = atoi(s2);
        result = strtoll(s, NULL, 0);
    } else {
        result = atoi(s);
#endif
    }
    result = strtoll(str, NULL, 0);
    if (!factor_specified) {
        if (smallmem && result >= 32) {
            return result;
        }
        if (!smallmem && result < 1024) {
            return result * 1024;
        }
    }
    return result * factor;
}
