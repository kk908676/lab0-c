#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */

/* Create an empty queue */
struct list_head *q_new()
{
    queue_contex_t *queue = (queue_contex_t *) malloc(sizeof(queue_contex_t));

    if (!queue) {
        return NULL;
    }
    queue->q = &queue->chain;
    queue->chain.prev = &queue->chain;
    queue->chain.next = &queue->chain;
    queue->size = 0;
    queue->id = 0;

    return queue->q;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (!head)
        return;

    struct list_head *cur = head->next;

    while (cur != head) {
        struct list_head *temp = cur;
        cur = cur->next;

        element_t *item = container_of(temp, element_t, list);
        free(item->value);
        free(item);
    }

    queue_contex_t *queue = container_of(head, queue_contex_t, chain);
    free(queue);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    element_t *item = (element_t *) malloc(sizeof(element_t));
    if (!item) {
        return false;
    }
    item->value = strdup(s);

    if (head->next == head) {
        head->next = &item->list;
        head->prev = &item->list;
        item->list.next = head;
        item->list.prev = head;
    } else {
        struct list_head *old = head->next;
        head->next = &item->list;
        old->prev = &item->list;
        item->list.next = old;
        item->list.prev = head;
    }

    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    element_t *item = (element_t *) malloc(sizeof(element_t));
    if (!item) {
        return false;
    }
    item->value = strdup(s);

    if (head->prev == head) {
        head->next = &item->list;
        head->prev = &item->list;
        item->list.next = head;
        item->list.prev = head;
    } else {
        struct list_head *old = head->prev;
        head->prev = &item->list;
        old->next = &item->list;
        item->list.next = head;
        item->list.prev = old;
    }

    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head)
        return NULL;

    struct list_head *remove = head->next;
    element_t *re_item = container_of(remove, element_t, list);

    head->next = head->next->next;
    if (head->next == head)
        head->prev = head;
    else {
        head->next->prev = head;
    }

    if (sp != NULL) {
        strncpy(sp, re_item->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }

    return re_item;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head)
        return NULL;

    struct list_head *remove = head->prev;
    element_t *re_item = container_of(remove, element_t, list);

    head->prev = head->prev->prev;
    if (head->prev == head)
        head->next = head;
    else
        head->prev->next = head;

    if (sp != NULL) {
        strncpy(sp, re_item->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }

    return re_item;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head || head->next == head)
        return 0;

    int count = 0;
    struct list_head *temp = head->next;
    while (temp != head) {
        count++;
        temp = temp->next;
    }

    return count;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    struct list_head *temp = head;

    int count = q_size(head);
    if (count == 0)
        return false;
    else if (count == 1) {
        element_t *re_item = q_remove_head(temp, NULL, 0);
        free(re_item->value);
        free(re_item);
    } else if (count == 2) {
        element_t *re_item = q_remove_tail(temp, NULL, 0);
        free(re_item->value);
        free(re_item);
    } else {
        count = count / 2 + 1;
        while (count != 1) {
            temp = temp->next;
            count -= 1;
        }
        element_t *re_item = container_of(temp->next, element_t, list);

        temp->next->next->prev = temp;
        temp->next = temp->next->next;

        free(re_item->value);
        free(re_item);
    }

    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head) {}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere
 * to the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in
 * ascending/descending order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
