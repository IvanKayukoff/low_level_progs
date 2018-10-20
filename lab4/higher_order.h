#ifndef LAB4_HIGHER_ORDER_H
#define LAB4_HIGHER_ORDER_H

#include "linked_list.h"

void foreach(Node const *lst, void (*action)(int));

Node *map(Node const *lst, int (*action)(int));

Node *map_mut(Node *lst, int (*action)(int));

#endif //LAB4_HIGHER_ORDER_H
