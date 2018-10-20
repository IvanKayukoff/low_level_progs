#include <stdlib.h>
#include <assert.h>
#include "linked_list.h"
#include "higher_order.h"

void foreach(Node const *lst, void (*action)(int)) {
    assert(lst != NULL);
    assert(action != NULL);

    while (lst != NULL) {
        (*action)(lst->data);
        lst = lst->next;
    }
}

Node *map(Node const *lst, int (*action)(int)) {
    assert(lst != NULL);
    assert(action != NULL);

    Node *head = NULL;
    size_t length = list_length(lst);
    for (size_t i = 0; i < length; ++i) {
        if (head == NULL) {
            head = list_create((*action)(list_get(lst, i)));
        }
        push_back(head, (*action)(list_get(lst, i)));
    }
    return head;
}

Node *map_mut(Node *lst, int (*action)(int)) {
    assert(lst != NULL);
    assert(action != NULL);

    size_t length = list_length(lst);
    for (size_t i = 0; i < length; ++i) {
        Node *node = list_node_at(lst, i);
        node->data = (*action)(node->data);
    }
    return lst;
}

int foldl(Node const *lst, int (*action)(int, int), int a) {
    assert(lst != NULL);
    assert(action != NULL);

    size_t length = list_length(lst);
    for (size_t i = 0; i < length; ++i) {
        a = (*action)(lst->data, a);
        lst = lst->next;
    }
    return a;
}

void iterate(Node *lst, int (*action)(int), size_t length) {
    assert(lst != NULL);
    assert(action != NULL);
    /** To avoid memory leaks */
    assert(list_length(lst) == 1);

    for (size_t i = 0; i < length - 1; ++i) {
        lst->next = list_create((*action)(lst->data));
        lst = lst->next;
    }
}
