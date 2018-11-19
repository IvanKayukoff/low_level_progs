#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>
#include "custom_memory.h"

uint8_t *heap_head;

void *heap_init() {
    heap_head = mmap(HEAP_HEAD, CHUNK_INIT_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, 0, 0);
    memory_t *p_heap = (memory_t *) heap_head;
    p_heap->next = NULL;
    p_heap->capacity = CHUNK_INIT_SIZE - sizeof(memory_t);
    return (void *) heap_head;
}


