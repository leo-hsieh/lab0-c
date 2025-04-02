#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *new_node;
    new_node = malloc(sizeof(struct list_head));
    if (!new_node)
        return NULL;
    new_node->next = new_node;
    new_node->prev = new_node;
    return new_node;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    struct list_head *tail = head->prev;
    struct list_head *tmp = tail->prev;
    while (tail != head) {
        element_t *element = container_of(tail, element_t, list);
        free(element->value);
        free(element);
        tail = tmp;
        tmp = tmp->prev;
    }
    free(head);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    char *s_cpy;
    s_cpy = malloc(sizeof(char) * (strlen(s) + 1));
    strlcpy(s_cpy, s, sizeof(s_cpy));
    element_t *element;
    element = malloc(sizeof(element_t));
    if (!element) {
        return false;
    }
    element->value = s_cpy;
    list_add(&element->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    char *s_cry;
    s_cry = strdup(s);
    element_t *element = malloc(sizeof(element_t));
    if (!element) {
        return false;
    }
    element->value = s_cry;
    list_add_tail(&element->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (list_empty(head)) {
        return NULL;
    }
    element_t *entry;
    entry = container_of(head->next, element_t, list);
    strlcpy(sp, entry->value, sizeof(entry->value));
    list_del(head->next);
    return entry;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (list_empty(head)) {
        return NULL;
    }
    element_t *entry;
    entry = container_of(head->prev, element_t, list);
    strlcpy(sp, entry->value, sizeof(entry->value));
    list_del(head->prev);
    return entry;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    struct list_head *node;
    int size = 0;
    list_for_each(node, head) {
        size++;
    }
    return size;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    int size = q_size(head);
    int middle = size / 2 + 1;
    struct list_head *mid_node = head;
    element_t *entry;
    for (int i = 0; i < middle; i++) {
        mid_node = mid_node->next;
    }
    entry = container_of(mid_node, element_t, list);
    list_del(mid_node);
    free(entry->value);
    free(entry);
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    struct list_head *node;
    element_t *entry, *n_entry;
    list_for_each(node, head) {
        entry = container_of(node, element_t, list);
        char *tmp_str = entry->value;
        if (node->next == head) {
            break;
        }
        n_entry = container_of(node->next, element_t, list);
        char *n_str = n_entry->value;
        if (strcmp(tmp_str, n_str) == 0) {
            list_del(node->next);
            free(n_str);
            free(n_entry);
            while (node->next != head) {
                n_entry = container_of(node->next, element_t, list);
                n_str = n_entry->value;
                if (strcmp(tmp_str, n_str) != 0) {
                    break;
                } else {
                    list_del(node->next);
                    free(n_str);
                    free(n_entry);
                }
            }
            node = node->prev;
            list_del(node->next);
            free(tmp_str);
            free(entry);
        }
    }
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    struct list_head *node, *prev, *next;
    list_for_each(node, head) {
        if (node->next != head) {
            prev = node->prev;
            next = node->next->next;
            prev->next = node->next;
            prev->next->next = node;
            node->prev = prev->next;
            node->prev->prev = prev;
            node->next = next;
            next->prev = node;
        }
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    struct list_head *node;
    struct list_head *tmp;
    for (node = head->next; node != head; node = node->prev) {
        tmp = node->prev;
        node->prev = node->next;
        node->next = tmp;
    }
    tmp = head->prev;
    head->prev = head->next;
    head->next = tmp;
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
    struct list_head *start, *end;
    struct list_head *node;
    struct list_head *tmp_n, *tmp_p;
    bool last = false;
    for (node = head->next; node != head;) {
        start = node->prev;
        tmp_p = start->prev;
        end = node;
        for (int i = 0; i < k - 1; i++) {
            end = end->next;
            if (end == head) {
                last = true;
            }
        }
        if (last) {
            break;
        }
        tmp_n = end->next;
        end->next = start;
        start->prev = end;
        q_reverse(start);
        start->prev->next = tmp_n;
        tmp_n->prev = start->prev;
        start->prev = tmp_p;
        node = tmp_n;
    }
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    struct list_head *node;
    element_t *entry;
    char *str;
    const char *less = container_of(head->prev, element_t, list)->value;
    for (node = head->prev->prev; node != head; node = node->prev) {
        entry = container_of(node, element_t, list);
        str = entry->value;
        if (atoi(less) - atoi(str) < 0) {
            list_del(node);
        } else if (atoi(less) - atoi(str) > 0) {
            less = str;
        }
    }

    return q_size(head);
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    struct list_head *node;
    element_t *entry;
    char *str;
    const char *greater = container_of(head->prev, element_t, list)->value;
    for (node = head->prev->prev; node != head; node = node->prev) {
        entry = container_of(node, element_t, list);
        str = entry->value;
        if (atoi(greater) - atoi(str) > 0) {
            list_del(node);
        } else if (atoi(greater) - atoi(str) < 0) {
            greater = str;
        }
    }

    return q_size(head);
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
