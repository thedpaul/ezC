#ifndef EZC_UTIL_ARENA_H
#define EZC_UTIL_ARENA_H

#include <stddef.h>

typedef struct ezC_arena ezC_arena;

ezC_arena *ezC_create_arena(size_t size);

void ezC_destroy_arena(ezC_arena *arena);

void *ezC_arena_start(ezC_arena *arena);

void *ezC_arena_end(ezC_arena *arena);

size_t ezC_arena_size(ezC_arena *arena);

int ezC_arena_is_full(ezC_arena *arena);

int ezC_arena_resize(ezC_arena *arena, size_t size);

void *ezC_arena_alloc(ezC_arena *arena, size_t size);

int ezC_arena_dealloc(ezC_arena *arena, size_t size);

void *ezC_arena_aligned_alloc(ezC_arena *arena, size_t size, size_t alignment);

#endif // !EZC_UTIL_ARENA_H

