#include <stdint.h>
#include <sys/mman.h>
#include <assert.h>
#include "custom_memory.h"

static uint8_t *heap_head = NULL;

static void *heap_init() {
    assert(heap_head == NULL);

    heap_head = mmap(HEAP_HEAD, MMAP_MIN_SIZE, PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, 0, 0);
    assert(heap_head != MAP_FAILED); // FIXME what if failed ?

    memory_t *p_heap = (memory_t *) heap_head;
    p_heap->next = NULL;
    p_heap->capacity = MMAP_MIN_SIZE - sizeof(memory_t);

    return (void *) heap_head;
}

static void coalesce_free_chunks(memory_t *p) {
    assert(p != NULL);
    assert((p->capacity & 1) == 0);

    while (p->next != NULL && (p->next->capacity & 1) == 0) {
        if (p->next != (memory_t *) ((uint8_t *) p + sizeof(memory_t) + p->capacity)) return;

        p->capacity += p->next->capacity + sizeof(memory_t);
        p->next = p->next->next;
    }
}

static void alloc_chunk(memory_t *last, size_t size) {
    assert(last != NULL);
    assert(size != 0);

    void *heap_end = (uint8_t *) last + last->capacity + sizeof(memory_t);

    size_t add_size = size + 2 * MMAP_MIN_SIZE - (size % MMAP_MIN_SIZE);
    void *add_memory = mmap(heap_end, add_size, PROT_READ | PROT_WRITE,
                            MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, 0, 0);

    if (add_memory != MAP_FAILED && (last->capacity & 1) == 0) {
        last->capacity += add_size;
        return;
    }

    if (add_memory == MAP_FAILED) {
        add_memory = mmap(heap_end, add_size, PROT_READ | PROT_WRITE,
                          MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
    }

    memory_t *add_head = (memory_t *) add_memory;
    add_head->next = NULL;
    add_head->capacity = add_size;
    last->next = add_head;
}

static void split_chunk(memory_t *chunk, size_t size) {
    assert(chunk != NULL);
    assert((chunk->capacity & 1) == 0);
    assert(size != 0);
    assert(chunk->capacity >= size);

    size_t remainder = chunk->capacity - size;
    if (remainder >= CHUNK_MIN_SIZE) {
        memory_t *second_chunk = (memory_t *) (((uint8_t *) chunk) + sizeof(memory_t) + size);
        second_chunk->capacity = remainder - sizeof(memory_t);
        second_chunk->next = chunk->next;
        chunk->next = second_chunk;
        chunk->capacity -= remainder;
    }
}

void *custom_malloc(size_t query_sz) {
    if (query_sz == 0) return NULL;
    if (query_sz % CHUNK_ALIGN != 0) query_sz += CHUNK_ALIGN - (query_sz % CHUNK_ALIGN);

    if (heap_head == NULL) heap_init();
    memory_t *chunk = (memory_t *) heap_head;

    while (1) {
        if ((chunk->capacity & 1) == 0) {
            coalesce_free_chunks(chunk);
            if (chunk->capacity >= query_sz) break;
        }

        if (chunk->next == NULL) alloc_chunk(chunk, query_sz);
        else chunk = chunk->next;
    }

    split_chunk(chunk, query_sz);
    chunk->capacity = chunk->capacity | 1;
    return (void *) (chunk + 1);
}

void custom_free(void *p) {
    memory_t *chunk = (memory_t *) ((uint8_t *) p - sizeof(memory_t));
    assert((chunk->capacity & 1) == 1 && "The memory is already freed");

    chunk->capacity = chunk->capacity & -2;
    coalesce_free_chunks(chunk);
}
