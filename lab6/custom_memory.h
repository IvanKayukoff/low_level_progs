#ifndef LAB6_CUSTOM_MEMORY_H
#define LAB6_CUSTOM_MEMORY_H

#include <stddef.h>
#include <unistd.h>

#define HEAP_HEAD ((void*)0x04040000)
#define CHUNK_ALIGN 8
#define CHUNK_INIT_SIZE ((size_t)(1 * sysconf(_SC_PAGESIZE)))

/**
 *  Structure represents a one chunk of memory in the heap, 8 bytes aligned.
 *  Since [capacity] always divisible by 8, the last bit is [isAllocated] flag:
 *  0 - free chunk, 1 - allocated
 **/
#pragma pack(push, 1)
typedef struct {
    struct memory_t *next;
    size_t capacity;
} memory_t;
#pragma pack(pop)

void *custom_malloc(size_t query_sz);

void custom_free(void *memory);

void *heap_init();

#endif //LAB6_CUSTOM_MEMORY_H
