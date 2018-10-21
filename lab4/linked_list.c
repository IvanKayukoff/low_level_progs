#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "linked_list.h"

Node *list_create(int data) {
    Node *head = (Node *) calloc(1, sizeof(Node));

    assert(head != NULL);

    head->data = data;
    head->next = NULL;
    return head;
}

Node *list_add_front(Node *const lst, Node *const elem) {
    assert(elem != NULL);
    assert(lst != NULL);

    elem->next = lst;
    return elem;
}

void list_add_back(Node *lst, Node *const elem) {
    assert(elem != NULL);
    assert(lst != NULL);

    while (lst->next != NULL) {
        lst = lst->next;
    }
    lst->next = elem;
}

int list_get(Node const *lst, size_t index) {
    if (lst == NULL) return 0;

    for (int i = 0; i < index; ++i) {
        if (lst->next == NULL) return 0;
        lst = lst->next;
    }
    return lst->data;
}

void list_free(Node *lst) {
    assert(lst != NULL);

    while (lst != NULL) {
        Node *next = lst->next;
        free(lst);
        lst = next;
    }
}

size_t list_length(Node const *lst) {
    for (size_t i = 0;; ++i) {
        if (lst == NULL) return i;
        lst = lst->next;
    }
}

Node *list_node_at(Node *lst, size_t index) {
    if (lst == NULL) return NULL;

    for (int i = 0; i < index; ++i) {
        if (lst->next == NULL) return NULL;
        lst = lst->next;
    }
    return lst;
}

int list_sum(Node const *lst) {
    assert(lst != NULL);

    int sum = 0;
    for (int i = 0; lst != NULL; ++i) {
        sum += lst->data;
        lst = lst->next;
    }
    return sum;
}

void push_back(Node *lst, int data) {
    assert(lst != NULL);

    Node *item = list_create(data);
    list_add_back(lst, item);
}

Node *push_front(Node *const lst, int data) {
    assert(lst != NULL);

    Node *item = list_create(data);
    Node *head = list_add_front(lst, item);
    return head;
}

void list_print(Node const *lst, char const *const delim) {
    assert(lst != NULL);
    assert(delim != NULL);

    while (lst != NULL) {
        printf("%d%s", lst->data, delim);
        lst = lst->next;
    }
    printf("\n");
}

bool save(Node *lst, char const *filename) {
    
}
