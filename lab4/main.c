#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
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

int sqr(int x) {
    return x * x;
}

int cube(int x) {
    return x * x * x;
}

int min(int left, int right) {
    if (left < right) return left;
    else return right;
}

int max(int left, int right) {
    if (left > right) return left;
    else return right;
}

int sum(int left, int right) {
    return left + right;
}

int double_it(int x) {
    return 2 * x;
}

void high_order_test() {
    Node *list = cli_fill_list();

    printf("\n=== Print list with space character ===\n");
    foreach(list, &print_int_with_space);

    printf("\n\n=== Print list with newline character ===\n");
    foreach(list, &print_int_with_newline);

    Node *sqr_list = map(list, &sqr);
    printf("\n=== Print squares of the list ===\n");
    list_print(sqr_list, " ");

    Node *cube_list = map(list, &cube);
    printf("\n=== Print cubes of the list ===\n");
    list_print(cube_list, " ");

    printf("\n=== Print the minimal value of the list ===\n");
    printf("%d\n", foldl(list, &min, INT_MAX));

    printf("\n=== Print the maximum value of the list ===\n");
    printf("%d\n", foldl(list, &max, INT_MIN));

    printf("\n=== Print the sum of all elements value of the list ===\n");
    printf("%d\n", foldl(list, &sum, 0));

    printf("\n=== Print the module of each value of the list ===\n");
    map_mut(list, &abs);
    list_print(list, " ");

    Node *seq_list = list_create(1);
    iterate(seq_list, &double_it, 10);
    printf("\n=== Print first 10 values of the sequence degrees of 2 ===\n");
    list_print(seq_list, " ");

    list_free(seq_list);
    list_free(list);
    list_free(sqr_list);
    list_free(cube_list);
}

void list_test() {
    Node *list = cli_fill_list();

    printf("List items:\n\t");
    list_print(list, " ");
    printf("Length of the list: %lu\n", list_length(list));
    printf("Sum of elements: %d\n", list_sum(list));

    list_free(list);
}
