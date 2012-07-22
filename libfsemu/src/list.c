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

#include <fs/base.h>
#include <fs/list.h>
#include <stdlib.h>

static inline fs_list* _fs_list_alloc(void) {
    return fs_new(fs_list, 1);
}

static inline fs_list* _fs_list_alloc0(void) {
    return fs_new0(fs_list, 1);
}

fs_list* fs_list_alloc(void) {
#if 0
    return _fs_list_alloc0 ();
#endif
    return fs_new0(fs_list, 1);
}

void fs_list_free(fs_list *list) {
#if 0
    g_slice_free_chain(fs_list, list, next);
#endif
    fs_list *link = list;
    while (link) {
        fs_list *next = link->next;
        free(link);
        link = next;
    }
}

void fs_list_free_1(fs_list *list) {
#if 0
    _fs_list_free_1(list);
#endif
    free(list);
}

void fs_list_free_full(fs_list *list, fs_destroy_notify free_func) {
    fs_list_foreach(list, (fs_func) free_func, NULL);
    fs_list_free(list);
}

fs_list *fs_list_append(fs_list *list, void * data) {
    fs_list *new_list;
    fs_list *last;

    new_list = _fs_list_alloc();
    new_list->data = data;
    new_list->next = NULL;

    if (list) {
        last = fs_list_last(list);
        /* g_assert (last != NULL); */
        last->next = new_list;
        new_list->prev = last;

        return list;
    }
    else {
        new_list->prev = NULL;
        return new_list;
    }
}

fs_list *fs_list_prepend(fs_list *list, void *data) {
    fs_list *new_list;

    new_list = _fs_list_alloc();
    new_list->data = data;
    new_list->next = list;

    if (list) {
        new_list->prev = list->prev;
        if (list->prev) list->prev->next = new_list;
        list->prev = new_list;
    }
    else new_list->prev = NULL;

    return new_list;
}

fs_list *fs_list_insert(fs_list *list, void *data, int position) {
    fs_list *new_list;
    fs_list *tmp_list;

    if (position < 0) return fs_list_append(list, data);
    else if (position == 0) return fs_list_prepend(list, data);

    tmp_list = fs_list_nth(list, position);
    if (!tmp_list) return fs_list_append(list, data);

    new_list = _fs_list_alloc();
    new_list->data = data;
    new_list->prev = tmp_list->prev;
    tmp_list->prev->next = new_list;
    new_list->next = tmp_list;
    tmp_list->prev = new_list;

    return list;
}

fs_list *fs_list_insert_before(fs_list *list, fs_list *sibling, void * data) {
    if (!list) {
        list = fs_list_alloc();
        list->data = data;
        //g_return_val_if_fail(sibling == NULL, list);
        if (sibling != NULL) {
            return list;
        }
        return list;
    }
    else if (sibling) {
        fs_list *node;

        node = _fs_list_alloc();
        node->data = data;
        node->prev = sibling->prev;
        node->next = sibling;
        sibling->prev = node;
        if (node->prev) {
            node->prev->next = node;
            return list;
        }
        else {
            //g_return_val_if_fail(sibling == list, node);
            if (sibling != list) {
                return node;
            }
            return node;
        }
    }
    else {
        fs_list *last;

        last = list;
        while (last->next)
            last = last->next;

        last->next = _fs_list_alloc();
        last->next->data = data;
        last->next->prev = last;
        last->next->next = NULL;

        return list;
    }
}

fs_list *fs_list_concat(fs_list *list1, fs_list *list2) {
    fs_list *tmp_list;

    if (list2) {
        tmp_list = fs_list_last(list1);
        if (tmp_list) tmp_list->next = list2;
        else list1 = list2;
        list2->prev = tmp_list;
    }

    return list1;
}

fs_list *fs_list_remove(fs_list *list, const void * data) {
    fs_list *tmp;

    tmp = list;
    while (tmp) {
        if (tmp->data != data) tmp = tmp->next;
        else {
            if (tmp->prev) tmp->prev->next = tmp->next;
            if (tmp->next) tmp->next->prev = tmp->prev;

            if (list == tmp) list = list->next;

            fs_list_free_1(tmp);

            break;
        }
    }
    return list;
}

fs_list *fs_list_remove_all(fs_list *list, const void * data) {
    fs_list *tmp = list;

    while (tmp) {
        if (tmp->data != data) tmp = tmp->next;
        else {
            fs_list *next = tmp->next;

            if (tmp->prev) tmp->prev->next = next;
            else list = next;
            if (next) next->prev = tmp->prev;

            fs_list_free_1(tmp);
            tmp = next;
        }
    }
    return list;
}

static inline fs_list *_fs_list_remove_link(fs_list *list, fs_list *link) {
    if (link) {
        if (link->prev) link->prev->next = link->next;
        if (link->next) link->next->prev = link->prev;

        if (link == list) list = list->next;

        link->next = NULL;
        link->prev = NULL;
    }

    return list;
}

fs_list *fs_list_remove_link(fs_list *list, fs_list *llink) {
    return _fs_list_remove_link(list, llink);
}

fs_list *fs_list_delete_link(fs_list *list, fs_list *link_) {
    list = _fs_list_remove_link(list, link_);
    fs_list_free_1(link_);

    return list;
}

fs_list *fs_list_copy(fs_list *list) {
    fs_list *new_list = NULL;

    if (list) {
        fs_list *last;

        new_list = _fs_list_alloc0();
        new_list->data = list->data;
        new_list->prev = NULL;
        last = new_list;
        list = list->next;
        while (list) {
            last->next = _fs_list_alloc0();
            last->next->prev = last;
            last = last->next;
            last->data = list->data;
            list = list->next;
        }
        last->next = NULL;
    }

    return new_list;
}

fs_list *fs_list_reverse(fs_list *list) {
    fs_list *last;

    last = NULL;
    while (list) {
        last = list;
        list = last->next;
        last->next = last->prev;
        last->prev = list;
    }

    return last;
}

fs_list *fs_list_nth(fs_list *list, unsigned int n) {
    while ((n-- > 0) && list)
        list = list->next;

    return list;
}

fs_list *fs_list_nth_prev(fs_list *list, unsigned int n) {
    while ((n-- > 0) && list)
        list = list->prev;

    return list;
}

void * fs_list_nth_data(fs_list *list, unsigned int n) {
    while ((n-- > 0) && list)
        list = list->next;

    return list ? list->data : NULL;
}

fs_list *fs_list_find(fs_list *list, const void * data) {
    while (list) {
        if (list->data == data) break;
        list = list->next;
    }

    return list;
}

fs_list *fs_list_find_custom(fs_list *list, const void * data,
        fs_compare_func func) {
    //g_return_val_if_fail(func != NULL, list);
    if (func == NULL) {
        return list;
    }

    while (list) {
        if (!func(list->data, data)) return list;
        list = list->next;
    }

    return NULL;
}

int fs_list_position(fs_list *list, fs_list *llink) {
    int i;

    i = 0;
    while (list) {
        if (list == llink) return i;
        i++;
        list = list->next;
    }

    return -1;
}

int fs_list_index(fs_list *list, const void *data) {
    int i;

    i = 0;
    while (list) {
        if (list->data == data) return i;
        i++;
        list = list->next;
    }

    return -1;
}

fs_list *fs_list_last(fs_list *list) {
    if (list) {
        while (list->next)
            list = list->next;
    }

    return list;
}

fs_list *fs_list_first(fs_list *list) {
    if (list) {
        while (list->prev)
            list = list->prev;
    }

    return list;
}

unsigned int fs_list_length(fs_list *list) {
    unsigned int length;

    length = 0;
    while (list) {
        length++;
        list = list->next;
    }

    return length;
}

void fs_list_foreach(fs_list *list, fs_func func, void *user_data) {
    while (list) {
        fs_list *next = list->next;
        (*func)(list->data, user_data);
        list = next;
    }
}

static fs_list *fs_list_insert_sorted_real(fs_list *list, void * data,
        fs_func func, void * user_data) {
    fs_list *tmp_list = list;
    fs_list *new_list;
    int cmp;

    //g_return_val_if_fail(func != NULL, list);
    if (func == NULL) {
        return list;
    }

    if (!list) {
        new_list = _fs_list_alloc0();
        new_list->data = data;
        return new_list;
    }

    cmp = ((fs_compare_data_func) func)(data, tmp_list->data, user_data);

    while ((tmp_list->next) && (cmp > 0)) {
        tmp_list = tmp_list->next;

        cmp = ((fs_compare_data_func) func)(data, tmp_list->data, user_data);
    }

    new_list = _fs_list_alloc0();
    new_list->data = data;

    if ((!tmp_list->next) && (cmp > 0)) {
        tmp_list->next = new_list;
        new_list->prev = tmp_list;
        return list;
    }

    if (tmp_list->prev) {
        tmp_list->prev->next = new_list;
        new_list->prev = tmp_list->prev;
    }
    new_list->next = tmp_list;
    tmp_list->prev = new_list;

    if (tmp_list == list) return new_list;
    else return list;
}

fs_list *fs_list_insert_sorted(fs_list *list, void * data, fs_compare_func func) {
    return fs_list_insert_sorted_real(list, data, (fs_func) func, NULL);
}

fs_list *fs_list_insert_sorted_with_data(fs_list *list, void * data,
        fs_compare_data_func func, void * user_data) {
    return fs_list_insert_sorted_real(list, data, (fs_func) func, user_data);
}

static fs_list *fs_list_sort_merge(fs_list *l1, fs_list *l2,
        fs_func compare_func, void * user_data) {
    fs_list list, *l, *lprev;
    int cmp;

    l = &list;
    lprev = NULL;

    while (l1 && l2) {
        cmp = ((fs_compare_data_func) compare_func)(l1->data, l2->data,
                user_data);

        if (cmp <= 0) {
            l->next = l1;
            l1 = l1->next;
        }
        else {
            l->next = l2;
            l2 = l2->next;
        }
        l = l->next;
        l->prev = lprev;
        lprev = l;
    }
    l->next = l1 ? l1 : l2;
    l->next->prev = l;

    return list.next;
}

static fs_list *fs_list_sort_real(fs_list *list, fs_func compare_func,
        void * user_data) {
    fs_list *l1, *l2;

    if (!list) return NULL;
    if (!list->next) return list;

    l1 = list;
    l2 = list->next;

    while ((l2 = l2->next) != NULL) {
        if ((l2 = l2->next) == NULL) break;
        l1 = l1->next;
    }
    l2 = l1->next;
    l1->next = NULL;

    return fs_list_sort_merge(fs_list_sort_real(list, compare_func, user_data),
            fs_list_sort_real(l2, compare_func, user_data), compare_func,
            user_data);
}

fs_list *fs_list_sort(fs_list *list, fs_compare_func compare_func) {
    return fs_list_sort_real(list, (fs_func) compare_func, NULL);

}

fs_list *fs_list_sort_with_data(fs_list *list,
        fs_compare_data_func compare_func, void * user_data) {
    return fs_list_sort_real(list, (fs_func) compare_func, user_data);
}
