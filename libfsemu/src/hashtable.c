/* GLIB - Library of useful routines for C programming
 * Copyright (C) 1995-1997  Peter Mattis, Spencer Kimball and Josh MacDonald
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/*
 * Modified by the GLib Team and others 1997-2000.  See the AUTHORS
 * file for a list of people on the GLib Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GLib at ftp://ftp.gtk.org/pub/gtk/.
 */

/*
 * MT safe
 */

//#include "config.h"
#include <string.h>  /* memset */
#include <stdlib.h>

#include <fs/base.h>
#include <fs/hashtable.h>
#include <fs/list.h>
#include <fs/string.h>

unsigned int fs_direct_hash (const void *v) {
    return FS_POINTER_TO_UINT(v);
}

#define HASH_TABLE_MIN_SHIFT 3  /* 1 << 3 == 8 buckets */

#define UNUSED_HASH_VALUE 0
#define TOMBSTONE_HASH_VALUE 1
#define HASH_IS_UNUSED(h_) ((h_) == UNUSED_HASH_VALUE)
#define HASH_IS_TOMBSTONE(h_) ((h_) == TOMBSTONE_HASH_VALUE)
#define HASH_IS_REAL(h_) ((h_) >= 2)

#define g_return_if_fail(a) if (!(a)) return;
#define g_return_val_if_fail(a, b) if (!(a)) return b;

struct _fs_hash_table {
    int size;
    int mod;
    unsigned int mask;
    int nnodes;
    int noccupied; /* nnodes + tombstones */

    void * *keys;
    unsigned int *hashes;
    void * *values;

    fs_hash_func hash_func;
    fs_equal_func key_equal_func;
    int ref_count;
#ifndef G_DISABLE_ASSERT
    /*
     * Tracks the structure of the hash table, not its contents: is only
     * incremented when a node is added or removed (is not incremented
     * when the key or data of a node is modified).
     */
    int version;
#endif
    fs_destroy_notify key_destroy_func;
    fs_destroy_notify value_destroy_func;
};

typedef struct {
    fs_hash_table *hash_table;
    void * dummy1;
    void * dummy2;
    int position;
    int dummy3;
    int version;
} RealIter;

static const int prime_mod[] = { 1, /* For 1 << 0 */
2, 3, 7, 13, 31, 61, 127, 251, 509, 1021, 2039, 4093, 8191, 16381, 32749, 65521, /* For 1 << 16 */
131071, 262139, 524287, 1048573, 2097143, 4194301, 8388593, 16777213, 33554393,
        67108859, 134217689, 268435399, 536870909, 1073741789, 2147483647 /* For 1 << 31 */
};

static void fs_hash_table_set_shift(fs_hash_table *hash_table, int shift) {
    int i;
    unsigned int mask = 0;

    hash_table->size = 1 << shift;
    hash_table->mod = prime_mod[shift];

    for (i = 0; i < shift; i++) {
        mask <<= 1;
        mask |= 1;
    }

    hash_table->mask = mask;
}

static int fs_hash_table_find_closest_shift(int n) {
    int i;

    for (i = 0; n; i++)
        n >>= 1;

    return i;
}

static void fs_hash_table_set_shift_from_size(fs_hash_table *hash_table,
        int size) {
    int shift;

    shift = fs_hash_table_find_closest_shift(size);
    shift = MAX (shift, HASH_TABLE_MIN_SHIFT);

    fs_hash_table_set_shift(hash_table, shift);
}

static inline unsigned int fs_hash_table_lookup_node(fs_hash_table *hash_table,
        const void * key, unsigned int *hash_return) {
    unsigned int node_index;
    unsigned int node_hash;
    unsigned int hash_value;
    unsigned int first_tombstone = 0;
    int have_tombstone = FALSE;
    unsigned int step = 0;

    hash_value = hash_table->hash_func(key);
    if (FS_UNLIKELY(!HASH_IS_REAL (hash_value)))
        hash_value = 2;

    *hash_return = hash_value;

    node_index = hash_value % hash_table->mod;
    node_hash = hash_table->hashes[node_index];

    while (!HASH_IS_UNUSED (node_hash)) {
        /* We first check if our full hash values
         * are equal so we can avoid calling the full-blown
         * key equality function in most cases.
         */
        if (node_hash == hash_value) {
            void * node_key = hash_table->keys[node_index];

            if (hash_table->key_equal_func) {
                if (hash_table->key_equal_func(node_key, key))
                    return node_index;
            }
            else if (node_key == key) {
                return node_index;
            }
        }
        else if (HASH_IS_TOMBSTONE (node_hash) && !have_tombstone) {
            first_tombstone = node_index;
            have_tombstone = TRUE;
        }

        step++;
        node_index += step;
        node_index &= hash_table->mask;
        node_hash = hash_table->hashes[node_index];
    }

    if (have_tombstone)
        return first_tombstone;

    return node_index;
}

static void fs_hash_table_remove_node(fs_hash_table *hash_table, int i,
        int notify) {
    void * key;
    void * value;

    key = hash_table->keys[i];
    value = hash_table->values[i];

    /* Erect tombstone */
    hash_table->hashes[i] = TOMBSTONE_HASH_VALUE;

    /* Be GC friendly */
    hash_table->keys[i] = NULL;
    hash_table->values[i] = NULL;

    hash_table->nnodes--;

    if (notify && hash_table->key_destroy_func)
        hash_table->key_destroy_func(key);

    if (notify && hash_table->value_destroy_func)
        hash_table->value_destroy_func(value);

}

static void fs_hash_table_remove_all_nodes(fs_hash_table *hash_table,
        int notify) {
    int i;
    void * key;
    void * value;

    hash_table->nnodes = 0;
    hash_table->noccupied = 0;

    if (!notify
            || (hash_table->key_destroy_func == NULL
                    && hash_table->value_destroy_func == NULL )) {
        memset(hash_table->hashes, 0, hash_table->size * sizeof(unsigned int));
        memset(hash_table->keys, 0, hash_table->size * sizeof(void *));
        memset(hash_table->values, 0, hash_table->size * sizeof(void *));

        return;
    }

    for (i = 0; i < hash_table->size; i++) {
        if (HASH_IS_REAL (hash_table->hashes[i])) {
            key = hash_table->keys[i];
            value = hash_table->values[i];

            hash_table->hashes[i] = UNUSED_HASH_VALUE;
            hash_table->keys[i] = NULL;
            hash_table->values[i] = NULL;

            if (hash_table->key_destroy_func != NULL )
                hash_table->key_destroy_func(key);

            if (hash_table->value_destroy_func != NULL )
                hash_table->value_destroy_func(value);
        }
        else if (HASH_IS_TOMBSTONE (hash_table->hashes[i])) {
            hash_table->hashes[i] = UNUSED_HASH_VALUE;
        }
    }
}

static void fs_hash_table_resize(fs_hash_table *hash_table) {
    void * *new_keys;
    void * *new_values;
    unsigned int *new_hashes;
    int old_size;
    int i;

    old_size = hash_table->size;
    fs_hash_table_set_shift_from_size(hash_table, hash_table->nnodes * 2);

    new_keys = fs_new0 (void *, hash_table->size);
    if (hash_table->keys == hash_table->values)
        new_values = new_keys;
    else
        new_values = fs_new0 (void *, hash_table->size);
    new_hashes = fs_new0 (unsigned int, hash_table->size);

    for (i = 0; i < old_size; i++) {
        unsigned int node_hash = hash_table->hashes[i];
        unsigned int hash_val;
        unsigned int step = 0;

        if (!HASH_IS_REAL (node_hash))
            continue;

        hash_val = node_hash % hash_table->mod;

        while (!HASH_IS_UNUSED (new_hashes[hash_val])) {
            step++;
            hash_val += step;
            hash_val &= hash_table->mask;
        }

        new_hashes[hash_val] = hash_table->hashes[i];
        new_keys[hash_val] = hash_table->keys[i];
        new_values[hash_val] = hash_table->values[i];
    }

    if (hash_table->keys != hash_table->values)
        free(hash_table->values);

    free(hash_table->keys);
    free(hash_table->hashes);

    hash_table->keys = new_keys;
    hash_table->values = new_values;
    hash_table->hashes = new_hashes;

    hash_table->noccupied = hash_table->nnodes;
}

static inline void fs_hash_table_maybe_resize(fs_hash_table *hash_table) {
    int noccupied = hash_table->noccupied;
    int size = hash_table->size;

    if ((size > hash_table->nnodes * 4 && size > 1 << HASH_TABLE_MIN_SHIFT)
            || (size <= noccupied + (noccupied / 16)))
        fs_hash_table_resize(hash_table);
}

fs_hash_table*
fs_hash_table_new(fs_hash_func hash_func, fs_equal_func key_equal_func) {
    return fs_hash_table_new_full(hash_func, key_equal_func, NULL, NULL );
}

fs_hash_table*
fs_hash_table_new_full(fs_hash_func hash_func, fs_equal_func key_equal_func,
        fs_destroy_notify key_destroy_func,
        fs_destroy_notify value_destroy_func) {
    fs_hash_table *hash_table;

    hash_table = fs_new(fs_hash_table, 1);
    fs_hash_table_set_shift(hash_table, HASH_TABLE_MIN_SHIFT);
    hash_table->nnodes = 0;
    hash_table->noccupied = 0;
    hash_table->hash_func = hash_func ? hash_func : fs_direct_hash;
    hash_table->key_equal_func = key_equal_func;
    hash_table->ref_count = 1;
#ifndef G_DISABLE_ASSERT
    hash_table->version = 0;
#endif
    hash_table->key_destroy_func = key_destroy_func;
    hash_table->value_destroy_func = value_destroy_func;
    hash_table->keys = fs_new0 (void *, hash_table->size);
    hash_table->values = hash_table->keys;
    hash_table->hashes = fs_new0 (unsigned int, hash_table->size);

    return hash_table;
}

void fs_hash_table_iter_init(fs_hash_table_iter *iter,
        fs_hash_table *hash_table) {
    RealIter *ri = (RealIter *) iter;

    g_return_if_fail(iter != NULL);
    g_return_if_fail(hash_table != NULL);

    ri->hash_table = hash_table;
    ri->position = -1;
#ifndef G_DISABLE_ASSERT
    ri->version = hash_table->version;
#endif
}

int fs_hash_table_iter_next(fs_hash_table_iter *iter, void * *key,
        void * *value) {
    RealIter *ri = (RealIter *) iter;
    int position;

    g_return_val_if_fail(iter != NULL, FALSE);
#ifndef G_DISABLE_ASSERT
    g_return_val_if_fail(ri->version == ri->hash_table->version, FALSE);
#endif
    g_return_val_if_fail(ri->position < ri->hash_table->size, FALSE);

    position = ri->position;

    do {
        position++;
        if (position >= ri->hash_table->size) {
            ri->position = position;
            return FALSE;
        }
    }
    while (!HASH_IS_REAL (ri->hash_table->hashes[position]));

    if (key != NULL )
        *key = ri->hash_table->keys[position];
    if (value != NULL )
        *value = ri->hash_table->values[position];

    ri->position = position;
    return TRUE;
}

fs_hash_table *
fs_hash_table_iter_get_hash_table(fs_hash_table_iter *iter) {
    g_return_val_if_fail(iter != NULL, NULL);

    return ((RealIter *) iter)->hash_table;
}

static void iter_remove_or_steal(RealIter *ri, int notify) {
    g_return_if_fail(ri != NULL);
#ifndef G_DISABLE_ASSERT
    g_return_if_fail(ri->version == ri->hash_table->version);
#endif
    g_return_if_fail(ri->position >= 0);
    g_return_if_fail(ri->position < ri->hash_table->size);

    fs_hash_table_remove_node(ri->hash_table, ri->position, notify);

#ifndef G_DISABLE_ASSERT
    ri->version++;
    ri->hash_table->version++;
#endif
}

void fs_hash_table_iter_remove(fs_hash_table_iter *iter) {
    iter_remove_or_steal((RealIter *) iter, TRUE);
}

static void fs_hash_table_insert_node(fs_hash_table *hash_table,
        unsigned int node_index, unsigned int key_hash, void * key,
        void * value, int keep_new_key, int reusing_key) {
    unsigned int old_hash;
    void * old_key;
    void * old_value;

    if (FS_UNLIKELY(hash_table->keys == hash_table->values && key != value))
        hash_table->values = fs_memdup(hash_table->keys,
                sizeof(void *) * hash_table->size);

    old_hash = hash_table->hashes[node_index];
    old_key = hash_table->keys[node_index];
    old_value = hash_table->values[node_index];

    if (HASH_IS_REAL (old_hash)) {
        if (keep_new_key)
            hash_table->keys[node_index] = key;
        hash_table->values[node_index] = value;
    }
    else {
        hash_table->keys[node_index] = key;
        hash_table->values[node_index] = value;
        hash_table->hashes[node_index] = key_hash;

        hash_table->nnodes++;

        if (HASH_IS_UNUSED (old_hash)) {
            /* We replaced an empty node, and not a tombstone */
            hash_table->noccupied++;
            fs_hash_table_maybe_resize(hash_table);
        }

#ifndef G_DISABLE_ASSERT
        hash_table->version++;
#endif
    }

    if (HASH_IS_REAL (old_hash)) {
        if (hash_table->key_destroy_func && !reusing_key)
            hash_table->key_destroy_func(keep_new_key ? old_key : key);
        if (hash_table->value_destroy_func)
            hash_table->value_destroy_func(old_value);
    }
}

void fs_hash_table_iter_replace(fs_hash_table_iter *iter, void * value) {
    RealIter *ri;
    unsigned int node_hash;
    void * key;

    ri = (RealIter *) iter;

    g_return_if_fail(ri != NULL);
#ifndef G_DISABLE_ASSERT
    g_return_if_fail(ri->version == ri->hash_table->version);
#endif
    g_return_if_fail(ri->position >= 0);
    g_return_if_fail(ri->position < ri->hash_table->size);

    node_hash = ri->hash_table->hashes[ri->position];
    key = ri->hash_table->keys[ri->position];

    fs_hash_table_insert_node(ri->hash_table, ri->position, node_hash, key,
            value, TRUE, TRUE);

#ifndef G_DISABLE_ASSERT
    ri->version++;
    ri->hash_table->version++;
#endif
}

void fs_hash_table_iter_steal(fs_hash_table_iter *iter) {
    iter_remove_or_steal((RealIter *) iter, FALSE);
}

#if 0
fs_hash_table*
fs_hash_table_ref(fs_hash_table *hash_table) {
    g_return_val_if_fail(hash_table != NULL, NULL);

    g_atomic_int_inc(&hash_table->ref_count);

    return hash_table;
}

void fs_hash_table_unref(fs_hash_table *hash_table) {
    g_return_if_fail(hash_table != NULL);

    if (g_atomic_int_dec_and_test(&hash_table->ref_count)) {
        fs_hash_table_remove_all_nodes(hash_table, TRUE);
        if (hash_table->keys != hash_table->values)
            free(hash_table->values);
        free(hash_table->keys);
        free(hash_table->hashes);
        g_slice_free(fs_hash_table, hash_table);
    }
}

void fs_hash_table_destroy(fs_hash_table *hash_table) {
    g_return_if_fail(hash_table != NULL);

    fs_hash_table_remove_all(hash_table);
    fs_hash_table_unref(hash_table);
}
#endif

void fs_hash_table_destroy(fs_hash_table *hash_table) {
    g_return_if_fail(hash_table != NULL);

    fs_hash_table_remove_all(hash_table);

    fs_hash_table_remove_all_nodes(hash_table, TRUE);
    if (hash_table->keys != hash_table->values)
        free(hash_table->values);
    free(hash_table->keys);
    free(hash_table->hashes);
    free(hash_table);
}


void *fs_hash_table_lookup(fs_hash_table *hash_table, const void *key) {
    unsigned int node_index;
    unsigned int node_hash;

    g_return_val_if_fail(hash_table != NULL, NULL);

    node_index = fs_hash_table_lookup_node(hash_table, key, &node_hash);

    return HASH_IS_REAL (hash_table->hashes[node_index]) ?
            hash_table->values[node_index] : NULL ;
}

int fs_hash_table_lookup_extended(fs_hash_table *hash_table,
        const void * lookup_key, void * *orig_key, void * *value) {
    unsigned int node_index;
    unsigned int node_hash;

    g_return_val_if_fail(hash_table != NULL, FALSE);

    node_index = fs_hash_table_lookup_node(hash_table, lookup_key, &node_hash);

    if (!HASH_IS_REAL (hash_table->hashes[node_index]))
        return FALSE;

    if (orig_key)
        *orig_key = hash_table->keys[node_index];

    if (value)
        *value = hash_table->values[node_index];

    return TRUE;
}

static void fs_hash_table_insert_internal(fs_hash_table *hash_table, void * key,
        void * value, int keep_new_key) {
    unsigned int key_hash;
    unsigned int node_index;

    g_return_if_fail(hash_table != NULL);

    node_index = fs_hash_table_lookup_node(hash_table, key, &key_hash);

    fs_hash_table_insert_node(hash_table, node_index, key_hash, key, value,
            keep_new_key, FALSE);
}

void fs_hash_table_insert(fs_hash_table *hash_table, void * key, void * value) {
    fs_hash_table_insert_internal(hash_table, key, value, FALSE);
}

void fs_hash_table_replace(fs_hash_table *hash_table, void * key, void * value) {
    fs_hash_table_insert_internal(hash_table, key, value, TRUE);
}

static int fs_hash_table_remove_internal(fs_hash_table *hash_table,
        const void * key, int notify) {
    unsigned int node_index;
    unsigned int node_hash;

    g_return_val_if_fail(hash_table != NULL, FALSE);

    node_index = fs_hash_table_lookup_node(hash_table, key, &node_hash);

    if (!HASH_IS_REAL (hash_table->hashes[node_index]))
        return FALSE;

    fs_hash_table_remove_node(hash_table, node_index, notify);
    fs_hash_table_maybe_resize(hash_table);

#ifndef G_DISABLE_ASSERT
    hash_table->version++;
#endif

    return TRUE;
}

int fs_hash_table_remove(fs_hash_table *hash_table, const void * key) {
    return fs_hash_table_remove_internal(hash_table, key, TRUE);
}

int fs_hash_table_steal(fs_hash_table *hash_table, const void * key) {
    return fs_hash_table_remove_internal(hash_table, key, FALSE);
}

void fs_hash_table_remove_all(fs_hash_table *hash_table) {
    g_return_if_fail(hash_table != NULL);

#ifndef G_DISABLE_ASSERT
    if (hash_table->nnodes != 0)
        hash_table->version++;
#endif

    fs_hash_table_remove_all_nodes(hash_table, TRUE);
    fs_hash_table_maybe_resize(hash_table);
}

void fs_hash_table_steal_all(fs_hash_table *hash_table) {
    g_return_if_fail(hash_table != NULL);

#ifndef G_DISABLE_ASSERT
    if (hash_table->nnodes != 0)
        hash_table->version++;
#endif

    fs_hash_table_remove_all_nodes(hash_table, FALSE);
    fs_hash_table_maybe_resize(hash_table);
}

static unsigned int fs_hash_table_foreach_remove_or_steal(
        fs_hash_table *hash_table, fs_hr_func func, void * user_data, int notify) {
    unsigned int deleted = 0;
    int i;
#ifndef G_DISABLE_ASSERT
    int version = hash_table->version;
#endif

    for (i = 0; i < hash_table->size; i++) {
        unsigned int node_hash = hash_table->hashes[i];
        void * node_key = hash_table->keys[i];
        void * node_value = hash_table->values[i];

        if (HASH_IS_REAL (node_hash)
                && (*func)(node_key, node_value, user_data)) {
            fs_hash_table_remove_node(hash_table, i, notify);
            deleted++;
        }

#ifndef G_DISABLE_ASSERT
        g_return_val_if_fail(version == hash_table->version, 0);
#endif
    }

    fs_hash_table_maybe_resize(hash_table);

#ifndef G_DISABLE_ASSERT
    if (deleted > 0)
        hash_table->version++;
#endif

    return deleted;
}

unsigned int fs_hash_table_foreach_remove(fs_hash_table *hash_table,
        fs_hr_func func, void * user_data) {
    g_return_val_if_fail(hash_table != NULL, 0);
    g_return_val_if_fail(func != NULL, 0);

    return fs_hash_table_foreach_remove_or_steal(hash_table, func, user_data,
            TRUE);
}

unsigned int fs_hash_table_foreach_steal(fs_hash_table *hash_table,
        fs_hr_func func, void *user_data) {
    g_return_val_if_fail(hash_table != NULL, 0);
    g_return_val_if_fail(func != NULL, 0);

    return fs_hash_table_foreach_remove_or_steal(hash_table, func, user_data,
            FALSE);
}

void fs_hash_table_foreach(fs_hash_table *hash_table, fs_h_func func,
        void *user_data) {
    int i;
#ifndef G_DISABLE_ASSERT
    int version;
#endif

    g_return_if_fail(hash_table != NULL);
    g_return_if_fail(func != NULL);

#ifndef G_DISABLE_ASSERT
    version = hash_table->version;
#endif

    for (i = 0; i < hash_table->size; i++) {
        unsigned int node_hash = hash_table->hashes[i];
        void * node_key = hash_table->keys[i];
        void * node_value = hash_table->values[i];

        if (HASH_IS_REAL (node_hash))
            (*func)(node_key, node_value, user_data);

#ifndef G_DISABLE_ASSERT
        g_return_if_fail(version == hash_table->version);
#endif
    }
}

void *fs_hash_table_find(fs_hash_table *hash_table, fs_hr_func predicate,
        void * user_data) {
    int i;
#ifndef G_DISABLE_ASSERT
    int version;
#endif
    int match;

    g_return_val_if_fail(hash_table != NULL, NULL);
    g_return_val_if_fail(predicate != NULL, NULL);

#ifndef G_DISABLE_ASSERT
    version = hash_table->version;
#endif

    match = FALSE;

    for (i = 0; i < hash_table->size; i++) {
        unsigned int node_hash = hash_table->hashes[i];
        void * node_key = hash_table->keys[i];
        void * node_value = hash_table->values[i];

        if (HASH_IS_REAL (node_hash))
            match = predicate(node_key, node_value, user_data);

#ifndef G_DISABLE_ASSERT
        g_return_val_if_fail(version == hash_table->version, NULL);
#endif

        if (match)
            return node_value;
    }

    return NULL ;
}

unsigned int fs_hash_table_size(fs_hash_table *hash_table) {
    g_return_val_if_fail(hash_table != NULL, 0);

    return hash_table->nnodes;
}

fs_list *fs_hash_table_get_keys(fs_hash_table *hash_table) {
    int i;
    fs_list *retval;

    g_return_val_if_fail(hash_table != NULL, NULL);

    retval = NULL;
    for (i = 0; i < hash_table->size; i++) {
        if (HASH_IS_REAL (hash_table->hashes[i]))
            retval = fs_list_prepend(retval, hash_table->keys[i]);
    }

    return retval;
}

fs_list *fs_hash_table_get_values(fs_hash_table *hash_table) {
    int i;
    fs_list *retval;

    g_return_val_if_fail(hash_table != NULL, NULL);

    retval = NULL;
    for (i = 0; i < hash_table->size; i++) {
        if (HASH_IS_REAL (hash_table->hashes[i]))
            retval = fs_list_prepend(retval, hash_table->values[i]);
    }

    return retval;
}
