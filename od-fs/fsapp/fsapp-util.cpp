/*
 * Small helpers: environment variables and moving averages.
 *
 * Copyright (c) 2025-2026 Frode Solheim
 */

#include "fsapp-util.h"

const char *fsapp_util_get_env(const char *name)
{
    const char *value = getenv(name);
    if (value != NULL) {
        return value;
    }
    return "";
}
