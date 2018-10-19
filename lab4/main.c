#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

int main() {
    int x = 0;
    Node *list = NULL;

    while (scanf("%d", &x) > 0) {
        if (list == NULL) {
            list = list_create(x);
            continue;
        }
        list = push_front(list, x);
    }

    if (list == NULL) return 0;

    printf("List items:\n\t");
    list_print(list);
    printf("Length of the list: %lu\n", list_length(list));
    printf("Sum of elements: %d\n", list_sum(list));
    return 0;
}
