#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "linked_list.h"
#include "higher_order.h"

void list_test();

void high_order_test();

Node *cli_fill_list();

int main() {
    // list_test();

    high_order_test();

    return 0;
}

Node *cli_fill_list() {
    printf("Enter integers to the new list, then press 'Enter': \n");

    char str[256];
    int x = 0;
    Node *list = NULL;

    fgets(str, 254, stdin);

    char *token = strtok(str, " \n");
    assert(token != NULL);

    while (token != NULL) {
        x = (int) strtol(token, NULL, 10);

        char parsed_as_str[21];
        sprintf(parsed_as_str, "%d", x);

        if (strcmp(parsed_as_str, token) != 0) {
            token = strtok(NULL, " \n");
            continue;
        }

        if (list == NULL) {
            list = list_create(x);
            token = strtok(NULL, " \n");
            continue;
        }

        list = push_front(list, x);
        token = strtok(NULL, " \n");
    }

    assert(list != NULL);

    return list;
}

void print_int_with_space(int x) {
    printf("%d ", x);
}

void print_int_with_newline(int x) {
    printf("%d\n", x);
}

void high_order_test() {
    Node *list = cli_fill_list();

    printf("\n===Print with space character test===\n");
    void (*pSpace_function)(int) = &print_int_with_space;
    foreach(list, pSpace_function);

    printf("\n\n===Print with newline character test===\n");
    void (*pNewline_function)(int) = &print_int_with_newline;
    foreach(list, pNewline_function);
}

void list_test() {
    Node *list = cli_fill_list();

    printf("List items:\n\t");
    list_print(list);
    printf("Length of the list: %lu\n", list_length(list));
    printf("Sum of elements: %d\n", list_sum(list));

    list_free(list);
}
