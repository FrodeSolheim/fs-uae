#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* GLIB - Library of useful routines for C programming
 * Copyright (C) 1995-1997  Peter Mattis, Spencer Kimball and Josh MacDonald
 *
 * fs_queue: Double ended queue implementation, piggy backed on fs_list.
 * Copyright (C) 1998 Tim Janik
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
 * MT safe
 */

#include <stdlib.h>
#include <fs/base.h>
#include <fs/queue.h>
#include <fs/list.h>

fs_queue *fs_queue_new(void) {
    return fs_new0(fs_queue, 1);
}

static void fs_queue_free(fs_queue *queue) {
    if (queue == NULL) {
        return;
    }
    fs_list_free(queue->head);
    //fs_slice_free(fs_queue, queue);
    free(queue);
}

static void fs_queue_init(fs_queue *queue) {
    if (queue == NULL) {
        return;
    }
    queue->head = queue->tail = NULL;
    queue->length = 0;
}

static void fs_queue_clear(fs_queue *queue) {
    if (queue == NULL) {
        return;
    }
    fs_list_free(queue->head);
    fs_queue_init(queue);
}

static int fs_queue_is_empty(fs_queue *queue) {
    if (queue == NULL) {
        return 1;
    }
    return queue->head == NULL ;
}

unsigned int fs_queue_get_length(fs_queue *queue) {
    if (queue == NULL) {
        return 0;
    }
    return queue->length;
}

static void fs_queue_reverse(fs_queue *queue) {
    if (queue == NULL) {
        return;
    }
    queue->tail = queue->head;
    queue->head = fs_list_reverse(queue->head);
}

static fs_queue *fs_queue_copy(fs_queue *queue) {
    fs_queue *result;
    fs_list *list;
    if (queue == NULL ) {
        return NULL ;
    }
    result = fs_queue_new();
    for (list = queue->head; list != NULL ; list = list->next) {
        fs_queue_push_tail(result, list->data);
    }
    return result;
}

#if 0
void fs_queue_foreach (fs_queue *queue,
        GFunc func,
        void * user_data)
{
    fs_list *list;

    g_return_if_fail (queue != NULL);
    g_return_if_fail (func != NULL);

    list = queue->head;
    while (list)
    {
        fs_list *next = list->next;
        func (list->data, user_data);
        list = next;
    }
}

fs_list *fs_queue_find(fs_queue *queue, const void * data) {
    g_return_val_if_fail(queue != NULL, NULL );

    return g_list_find(queue->head, data);
}

fs_list *
fs_queue_find_custom(fs_queue *queue, const void * data, GCompareFunc func) {
    g_return_val_if_fail(queue != NULL, NULL );
    g_return_val_if_fail(func != NULL, NULL );

    return g_list_find_custom(queue->head, data, func);
}

void fs_queue_sort(fs_queue *queue, GCompareDataFunc compare_func,
        void * user_data) {
    g_return_if_fail(queue != NULL );
    g_return_if_fail(compare_func != NULL );

    queue->head = g_list_sort_with_data(queue->head, compare_func, user_data);
    queue->tail = g_list_last(queue->head);
}
#endif

void fs_queue_push_head(fs_queue *queue, void * data) {
    if (queue == NULL) {
        return;
    }

    queue->head = fs_list_prepend(queue->head, data);
    if (!queue->tail)
        queue->tail = queue->head;
    queue->length++;
}

/**
 * fs_queue_push_nth:
 * @queue: a #fs_queue
 * @data: the data for the new element
 * @n: the position to insert the new element. If @n is negative or
 *     larger than the number of elements in the @queue, the element is
 *     added to the end of the queue.
 * 
 * Inserts a new element into @queue at the given position
 * 
 * Since: 2.4
 **/
static void fs_queue_push_nth(fs_queue *queue, void * data, int n) {
    if (queue == NULL) {
        return;
    }

    if (n < 0 || n >= queue->length) {
        fs_queue_push_tail(queue, data);
        return;
    }

    fs_queue_insert_before(queue, fs_queue_peek_nth_link(queue, n), data);
}

/**
 * fs_queue_push_head_link:
 * @queue: a #fs_queue.
 * @link_: a single #fs_list element, <emphasis>not</emphasis> a list with
 *     more than one element.
 *
 * Adds a new element at the head of the queue.
 **/
static void fs_queue_push_head_link(fs_queue *queue, fs_list *link) {
    if (queue == NULL || link == NULL) {
        return;
    }
    if (link->prev != NULL || link->next != NULL) {
        return;
    }
    link->next = queue->head;
    if (queue->head)
        queue->head->prev = link;
    else
        queue->tail = link;
    queue->head = link;
    queue->length++;
}

void fs_queue_push_tail(fs_queue *queue, void * data) {
    if (queue == NULL) {
        return;
    }

    queue->tail = fs_list_append(queue->tail, data);
    if (queue->tail->next)
        queue->tail = queue->tail->next;
    else
        queue->head = queue->tail;
    queue->length++;
}

static void fs_queue_push_tail_link(fs_queue *queue, fs_list *link) {
    if (queue == NULL || link == NULL) {
        return;
    }
    if (link->prev != NULL || link->next != NULL) {
        return;
    }

    link->prev = queue->tail;
    if (queue->tail)
        queue->tail->next = link;
    else
        queue->head = link;
    queue->tail = link;
    queue->length++;
}

#if 0
void fs_queue_push_nth_link(fs_queue *queue, int n, fs_list *link_) {
    fs_list *next;
    fs_list *prev;

    g_return_if_fail(queue != NULL );
    g_return_if_fail(link_ != NULL );

    if (n < 0 || n >= queue->length) {
        fs_queue_push_tail_link(queue, link_);
        return;
    }

    g_assert(queue->head);
    g_assert(queue->tail);

    next = fs_queue_peek_nth_link(queue, n);
    prev = next->prev;

    if (prev)
        prev->next = link_;
    next->prev = link_;

    link_->next = next;
    link_->prev = prev;

    if (queue->head->prev)
        queue->head = queue->head->prev;

    if (queue->tail->next)
        queue->tail = queue->tail->next;

    queue->length++;
}
#endif

void *fs_queue_pop_head(fs_queue *queue) {
    if (queue == NULL) {
        return NULL;
    }

    if (queue->head) {
        fs_list *node = queue->head;
        void * data = node->data;

        queue->head = node->next;
        if (queue->head)
            queue->head->prev = NULL;
        else
            queue->tail = NULL;
        fs_list_free_1(node);
        queue->length--;

        return data;
    }

    return NULL ;
}

static fs_list *fs_queue_pop_head_link(fs_queue *queue) {
    if (queue == NULL) {
        return NULL;
    }

    if (queue->head) {
        fs_list *node = queue->head;

        queue->head = node->next;
        if (queue->head) {
            queue->head->prev = NULL;
            node->next = NULL;
        } else
            queue->tail = NULL;
        queue->length--;

        return node;
    }

    return NULL ;
}

fs_list *fs_queue_peek_head_link(fs_queue *queue) {
    if (queue == NULL) {
        return NULL;
    }
    return queue->head;
}

static fs_list *fs_queue_peek_tail_link(fs_queue *queue) {
    if (queue == NULL) {
        return NULL;
    }
    return queue->tail;
}

void *fs_queue_pop_tail(fs_queue *queue) {
    if (queue == NULL) {
        return NULL;
    }

    if (queue->tail) {
        fs_list *node = queue->tail;
        void * data = node->data;

        queue->tail = node->prev;
        if (queue->tail)
            queue->tail->next = NULL;
        else
            queue->head = NULL;
        queue->length--;
        fs_list_free_1(node);

        return data;
    }

    return NULL ;
}

static void *fs_queue_pop_nth(fs_queue *queue, unsigned int n) {
    fs_list *nth_link;
    void * result;

    if (queue == NULL) {
        return NULL;
    }

    if (n >= queue->length)
        return NULL ;

    nth_link = fs_queue_peek_nth_link(queue, n);
    result = nth_link->data;

    fs_queue_delete_link(queue, nth_link);

    return result;
}

static fs_list *fs_queue_pop_tail_link(fs_queue *queue) {
    if (queue == NULL) {
        return NULL;
    }

    if (queue->tail) {
        fs_list *node = queue->tail;

        queue->tail = node->prev;
        if (queue->tail) {
            queue->tail->next = NULL;
            node->prev = NULL;
        } else
            queue->head = NULL;
        queue->length--;

        return node;
    }

    return NULL ;
}

static fs_list* fs_queue_pop_nth_link(fs_queue *queue, unsigned int n) {
    fs_list *link;

    if (queue == NULL) {
        return NULL;
    }

    if (n >= queue->length)
        return NULL ;

    link = fs_queue_peek_nth_link(queue, n);
    fs_queue_unlink(queue, link);

    return link;
}

fs_list *fs_queue_peek_nth_link(fs_queue *queue, unsigned int n) {
    fs_list *link;
    int i;

    if (queue == NULL) {
        return NULL;
    }

    if (n >= queue->length)
        return NULL ;

    if (n > queue->length / 2) {
        n = queue->length - n - 1;

        link = queue->tail;
        for (i = 0; i < n; ++i)
            link = link->prev;
    } else {
        link = queue->head;
        for (i = 0; i < n; ++i)
            link = link->next;
    }

    return link;
}

static int fs_queue_link_index(fs_queue *queue, fs_list *link_) {
    if (queue == NULL) {
        return -1;
    }
    return fs_list_position(queue->head, link_);
}

void fs_queue_unlink(fs_queue *queue, fs_list *link_) {
    if (queue == NULL || link_ == NULL) {
        return;
    }

    if (link_ == queue->tail)
        queue->tail = queue->tail->prev;

    queue->head = fs_list_remove_link(queue->head, link_);
    queue->length--;
}

void fs_queue_delete_link(fs_queue *queue, fs_list *link_) {
    if (queue == NULL || link_ == NULL) {
        return;
    }
    fs_queue_unlink(queue, link_);
    fs_list_free(link_);
}

void *fs_queue_peek_head(fs_queue *queue) {
    if (queue == NULL) {
        return NULL;
    }
    return queue->head ? queue->head->data : NULL;
}

void *fs_queue_peek_tail(fs_queue *queue) {
    if (queue == NULL) {
        return NULL;
    }
    return queue->tail ? queue->tail->data : NULL;
}

static void *fs_queue_peek_nth(fs_queue *queue, unsigned int n) {
    fs_list *link;

    if (queue == NULL) {
        return NULL;
    }

    link = fs_queue_peek_nth_link(queue, n);

    if (link)
        return link->data;

    return NULL ;
}

static int fs_queue_index(fs_queue *queue, const void * data) {
    if (queue == NULL) {
        return -1;
    }
    return fs_list_index(queue->head, data);
}

#if 0
int fs_queue_remove(fs_queue *queue, const void * data) {
    fs_list *link;

    if (queue == NULL) {
        return 0;
    }

    link = fs_list_find(queue->head, data);

    if (link)
        fs_queue_delete_link(queue, link);

    return (link != NULL);
}
#endif

static unsigned int fs_queue_remove_all(fs_queue *queue, const void * data) {
    fs_list *list;
    unsigned int old_length;

    if (queue == NULL) {
        return 0;
    }

    old_length = queue->length;

    list = queue->head;
    while (list) {
        fs_list *next = list->next;

        if (list->data == data)
            fs_queue_delete_link(queue, list);

        list = next;
    }

    return (old_length - queue->length);
}

void fs_queue_insert_before(fs_queue *queue, fs_list *sibling, void * data) {
    if (queue == NULL || sibling == NULL) {
        return;
    }
    queue->head = fs_list_insert_before(queue->head, sibling, data);
    queue->length++;
}

static void fs_queue_insert_after(fs_queue *queue, fs_list *sibling, void * data) {
    if (queue == NULL || sibling == NULL) {
        return;
    }
    if (sibling == queue->tail) {
        fs_queue_push_tail(queue, data);
    }
    else {
        fs_queue_insert_before(queue, sibling->next, data);
    }
}

#if 0
void fs_queue_insert_sorted(fs_queue *queue, void * data, GCompareDataFunc func,
        void * user_data) {
    fs_list *list;

    if (queue == NULL) {
        return;
    }

    list = queue->head;
    while (list && func(list->data, data, user_data) < 0)
        list = list->next;

    if (list)
        fs_queue_insert_before(queue, list, data);
    else
        fs_queue_push_tail(queue, data);
}
#endif
