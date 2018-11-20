#ifndef LAB6_CUSTOM_MEMORY_H
#define LAB6_CUSTOM_MEMORY_H

#include <stddef.h>
#include <unistd.h>

#define HEAP_HEAD ((void *)0x04040000) // FIXME not sure about this value
#define MMAP_MIN_SIZE ((size_t)(sysconf(_SC_PAGESIZE)))
#define CHUNK_ALIGN 8

/**
 *  Structure represents a one chunk of memory in the heap, 8 bytes aligned.
 *  Since [capacity] always divisible by 8, the last bit is [isAllocated] flag:
 *  0 - free chunk, 1 - allocated
 **/
#pragma pack(push, 1)
typedef struct memory_t {
    struct memory_t *next;
    size_t capacity;
} memory_t;
#pragma pack(pop)

#define CHUNK_MIN_SIZE (sizeof(memory_t) + CHUNK_ALIGN)

void *custom_malloc(size_t query_sz);

void custom_free(void *memory);

#endif //LAB6_CUSTOM_MEMORY_H
