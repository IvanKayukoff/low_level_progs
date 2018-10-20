#ifndef LAB4_HIGHER_ORDER_H
#define LAB4_HIGHER_ORDER_H

#include "linked_list.h"

/**
 *  Launches the [action] on each element of the list, not changes the list
 *  Requirements: [lst] and [action] must be not null
 **/
void foreach(Node const *lst, void (*action)(int));

/**
 *  Returns the new list containing the result of the [action] applied to all element of the [lst]
 *  The [lst] is not affected
 *  Requirements: [lst] and [action] must be not null
 **/
Node *map(Node const *lst, int (*action)(int));

/**
 *  Applies the [action] to all element of the [lst], so [lst] will be modified
 *  Requirements: [lst] and [action] must be not null
 **/
Node *map_mut(Node *lst, int (*action)(int));

/**
 *  Launches the [action] on each element of the list and the [a]
 *  After every iteration saves returned value by [action] to the [a]
 *  When [lst] is ends - returns [a]
 *  @param a is an accumulator which will be returned after all changes which will be applied by [action]
 *  @param action is the function accepts the current list value and [a] respectively, returns integer
 *  Requirements: [lst] and [action] must be not null
 **/
int foldl(Node const *lst, int (*action)(int, int), int a);

/**
 *  Generates a list of length [length] as follow:
 *  [s, f(s), f(f(s)), f(f(f(s))), ...]
 *  @param lst is a head of the list which value will be used for generation a new list
 *  @param length is desired list size
 *  Requirements: [lst] and [action] must be not null, length of the [lst] must be 1
 **/
void iterate(Node *lst, int (*action)(int), size_t length);

#endif //LAB4_HIGHER_ORDER_H
