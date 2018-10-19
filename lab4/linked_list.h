#include <stdlib.h>

#ifndef LAB4_LINKED_LIST_H
#define LAB4_LINKED_LIST_H

typedef struct Node Node;

/**
 *  Creates Node instance, calls assertion when memory allocation error happens
 *  @param data is a integer value which will be saved in the new node of the list
 *  @return Pointer to the new Node instance
 **/
Node *list_create(int data);

/**
 *  Joins elem to the front of the linked list
 *  @param lst is a current linked list, must be not NULL
 *  @param elem is a node to join, must be not NULL
 *  @return New head's pointer to the linked list
 **/
Node *list_add_front(Node *const lst, Node *const elem);

/**
 *  Joins elem to the back of the linked list
 *  @param lst is a current linked list, must be not NULL
 *  @param elem is a node to join, must be not NULL
 **/
void list_add_back(Node *lst, Node *const elem);

/**
 *  Pulls the value of the linked list at the specific index
 *  @param lst is a current linked list
 *  @param index is number of the element which value we want to pull out
 *  @return Received value or 0 if the index is outside the list bounds
 **/
int list_get(Node const *lst, size_t index);

/**
 *  Frees the memory allocated to all elements of list
 *  @param lst is the head of the linked list, must be not NULL
 **/
void list_free(Node *lst);

/**
 *  Calculates length of the linked list
 *  @param lst is a head of the linked list
 *  @return Length of the linked list
 **/
size_t list_length(Node const *lst);

/**
 *  Pulls a list item at the specific index
 *  @param lst is a head of the linked list
 *  @param index is number of the element which we want to pull out
 *  @return Pointer to the list item with specified index or 0 if the index is outside the list bounds
 **/
Node *list_node_at(Node *lst, size_t index);

/**
 *  Calculates the sum of elements
 *  @param lst is a head of the linked list, must be not NULL
 *  @return The sum of elements
 **/
int list_sum(Node const *lst);

void push_back(Node *lst, int data);

Node *push_front(Node *const lst, int data);

void list_print(Node const *lst);

#endif //LAB4_LINKED_LIST_H