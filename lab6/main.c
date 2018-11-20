#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "custom_memory.h"

void print_chunk(void *p, char const *name, FILE *f, bool print_bytes) {
    memory_t *chunk = (memory_t *) ((uint8_t *) p - sizeof(memory_t));
    size_t isAllocated = chunk->capacity & 1;
    fprintf(f, "%s:\n", name);
    fprintf(f, "Capacity: %lu, isAllocated: %lu, next: %p", chunk->capacity & -2, isAllocated, chunk->next);

    if (print_bytes) {
        fprintf(f, "\n  Elements: ");
        for (int i = 0; i < (chunk->capacity & -2); ++i) {
            uint8_t elem = *((uint8_t *) chunk + sizeof(memory_t) + i * sizeof(uint8_t));
            fprintf(f, "%d ", elem);
        }
    }

    fprintf(f, "\n\n");
}

int main() {
    uint8_t *first = custom_malloc(32 * sizeof(uint8_t));
    for (int i = 0; i < 32; ++i) {
        first[i] = (uint8_t)i;
    }
    print_chunk(first, "custom_malloc(32 * sizeof(uint8_t))", stdout, true);

    uint8_t *second = custom_malloc(12000 * sizeof(uint8_t));
    print_chunk(second, "custom_malloc(32 * sizeof(uint8_t))", stdout, false);

    uint8_t *third = custom_malloc(1 * sizeof(uint8_t));
    print_chunk(third, "custom_malloc(32 * sizeof(uint8_t))", stdout, true);

    custom_free(first);
    custom_free(second);
    custom_free(third);

    return 0;
}
