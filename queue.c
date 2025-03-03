#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */

void merge_two_list(struct list_head *L1, struct list_head *L2);

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
    if (!head)
        return false;

    element_t *item = malloc(sizeof(element_t));
    if (!item)
        return false;

    item->value = strdup(s);
    if (!item->value) {
        free(item);
        return false;
    }

    list_add(&item->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head)
        return false;

    element_t *item = malloc(sizeof(element_t));
    if (!item) {
        return false;
    }

    item->value = strdup(s);
    if (!item->value) {
        free(item);
        return false;
    }

    list_add_tail(&item->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    struct list_head *remove = head->next;
    element_t *re_item = container_of(remove, element_t, list);

    list_del(remove);

    if (sp != NULL) {
        strncpy(sp, re_item->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }

    return re_item;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    struct list_head *remove = head->prev;
    element_t *re_item = container_of(remove, element_t, list);

    list_del(remove);

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
    if (!head)
        return false;
    if (list_empty(head))
        return true;

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

    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    if (!head || list_empty(head))
        return false;

    struct list_head *cur = head->next;

    while (cur != head) {
        element_t *node1 = container_of(cur, element_t, list);
        struct list_head *temp = cur->next;
        bool dup = false;

        while (temp != head) {
            element_t *node2 = container_of(temp, element_t, list);

            if (strcmp(node1->value, node2->value) == 0) {
                dup = true;
                temp = temp->next;
                list_del(&node2->list);
                free(node2->value);
                free(node2);
            } else {
                break;
            }
        }

        struct list_head *next = cur->next;
        if (dup) {
            list_del(cur);
            free(node1->value);
            free(node1);
        }
        cur = next;
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
    if (!head || list_empty(head) || list_is_singular(head))
        return;

    struct list_head *fast = head->next;
    struct list_head *slow = head;

    while (fast != head && fast->next != head) {
        slow = slow->next;
        fast = fast->next->next;
    }

    struct list_head half;
    INIT_LIST_HEAD(&half);
    list_cut_position(&half, head, slow);

    q_sort(&half, descend);
    q_sort(head, descend);
    merge_two_list(head, &half);
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

void merge_two_list(struct list_head *L1, struct list_head *L2)
{
    struct list_head temp;
    INIT_LIST_HEAD(&temp);

    while (!list_empty(L1) && !list_empty(L2)) {
        element_t *node1 = container_of(L1->next, element_t, list);
        element_t *node2 = container_of(L2->next, element_t, list);
        if (strcmp(node1->value, node2->value) < 0) {
            list_move_tail(&node1->list, &temp);
        } else {
            list_move_tail(&node2->list, &temp);
        }
    }

    if (!list_empty(L1))
        list_splice_tail_init(L1, &temp);
    else
        list_splice_tail_init(L2, &temp);

    list_splice(&temp, L1);
}
