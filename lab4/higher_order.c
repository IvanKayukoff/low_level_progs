#include <stdlib.h>
#include <assert.h>
#include "linked_list.h"
#include "higher_order.h"

void foreach(Node const *lst, void (*action)(int)) {
    assert(lst != NULL);

    while (lst != NULL) {
        (*action)(list_get(lst, 0));
        lst = list_node_at((Node *)lst, 1);
    }
}
