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
    struct list_head *queue = malloc(sizeof(struct list_head));

    if (!queue) {
        return NULL;
    }

    INIT_LIST_HEAD(queue);

    return queue;
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

    free(head);
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
    if (!head || list_empty(head))
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
    int count = q_size(head);
    if (count == 0)
        return false;
    if (count == 1)
        return true;

    struct list_head *cur = head->next;
    struct list_head *temp = head->next->next;

    while (temp != head) {
        element_t *node1 = container_of(cur, element_t, list);
        element_t *node2 = container_of(temp, element_t, list);

        bool dup = 0;

        while (strcmp(node1->value, node2->value) == 0) {
            dup = 1;
            temp = temp->next;
            list_del(&node2->list);
            free(node2->value);
            free(node2);
            node2 = container_of(temp, element_t, list);
        }

        if (dup == 1) {
            list_del(&node1->list);
            free(node1->value);
            free(node1);
        }

        cur = temp;
        temp = temp->next;
    }
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    int count = q_size(head);
    if (count == 0 || count == 1)
        return;

    while (count > 1) {
        struct list_head *temp = head;

        element_t *node1 = container_of(temp->next, element_t, list);
        element_t *node2 = container_of(temp->next->next, element_t, list);

        temp = head->next->next->next;

        head->next = &node2->list;
        temp->prev = &node1->list;

        node2->list.next = &node1->list;
        node2->list.prev = head;
        node1->list.next = temp;
        node1->list.prev = &node2->list;

        count -= 2;
        head = head->next->next;
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    int count = q_size(head);
    if (count == 0 || count == 1)
        return;

    struct list_head *cur = head;
    struct list_head *temp = head->next;

    do {
        cur->next = cur->prev;
        cur->prev = temp;
        temp = temp->next;
        cur = cur->prev;
    } while (cur != head);
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    if (list_empty(head) || q_size(head) == 1 || k == 1)
        return;

    int count = 0;

    struct list_head *node, *safe;
    struct list_head *temp = head;

    list_for_each_safe (node, safe, head) {
        count++;
        if (count == k) {
            struct list_head head_to;
            INIT_LIST_HEAD(&head_to);

            list_cut_position(&head_to, temp, node);
            q_reverse(&head_to);
            list_splice_init(&head_to, temp);

            temp = safe->prev;
            count = 0;
        }
    }
}
/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    int count = q_size(head);
    if (count == 0 || count == 1)
        return;

    for (int i = count; i > 0; i--) {
        struct list_head *cur = head;
        struct list_head *temp = head->next->next->next;
        for (int j = 0; j < i - 1; j++) {
            element_t *node1 = container_of(cur->next, element_t, list);
            element_t *node2 = container_of(cur->next->next, element_t, list);

            if (strcmp(node1->value, node2->value) > 0) {
                cur->next = &node2->list;
                temp->prev = &node1->list;

                node2->list.next = &node1->list;
                node2->list.prev = cur;
                node1->list.next = temp;
                node1->list.prev = &node2->list;
            }

            cur = cur->next;
            temp = temp->next;
        }
    }

    if (descend) {
        q_reverse(head);
    }
}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    int count = q_size(head);
    if (count == 0 || count == 1)
        return count;

    q_reverse(head);

    struct list_head *cur = head->next;
    struct list_head *temp = head->next->next;

    while (temp != head) {
        const element_t *node1 = container_of(cur, element_t, list);
        element_t *node2 = container_of(temp, element_t, list);

        if (strcmp(node1->value, node2->value) < 0) {
            list_del(&node2->list);
            free(node2->value);
            free(node2);
            temp = cur->next;
        } else {
            cur = cur->next;
            temp = temp->next;
        }
    }

    q_reverse(head);

    return q_size(head);
}

/* Remove every node which has a node with a strictly greater value anywhere
 * to the right side of it */
int q_descend(struct list_head *head)
{
    int count = q_size(head);
    if (count == 0 || count == 1)
        return count;

    q_reverse(head);

    struct list_head *cur = head->next;
    struct list_head *temp = head->next->next;

    while (temp != head) {
        const element_t *node1 = container_of(cur, element_t, list);
        element_t *node2 = container_of(temp, element_t, list);

        if (strcmp(node1->value, node2->value) > 0) {
            list_del(&node2->list);
            free(node2->value);
            free(node2);
            temp = cur->next;
        } else {
            cur = cur->next;
            temp = temp->next;
        }
    }

    q_reverse(head);

    return q_size(head);
}

/* Merge all the queues into one sorted queue, which is in
 * ascending/descending order */
int q_merge(struct list_head *head, bool descend)
{
    if (!head || list_empty(head))
        return 0;

    queue_contex_t *q_head = container_of(head->next, queue_contex_t, chain);

    if (q_head->chain.next == q_head->chain.prev)
        return q_head->size;

    struct list_head *cur;
    struct list_head *safe;
    queue_contex_t *cur_queue;

    list_for_each_safe (cur, safe, head) {
        if (cur == &q_head->chain) {
            continue;
        }
        cur_queue = container_of(cur, queue_contex_t, chain);
        list_splice_tail_init(cur_queue->q, q_head->q);
        q_head->size += cur_queue->size;
    }

    q_sort(q_head->q, descend);
    return q_head->size;
}
