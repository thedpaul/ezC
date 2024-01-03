#include "ezC/util/arena.h"

#include <errno.h>
#include <stddef.h>
#include <stdlib.h>

#include "ezC/util/types.h"

struct ezC_arena {
  u8 *start;
  u8 *end;
  u8 *ptr;
};


ezC_arena *ezC_create_arena(size_t size) {
  ezC_arena *arena = malloc(sizeof(*arena));
  if (arena) {
    arena->start = malloc(size * sizeof(*arena->start));
    if (arena->start) {
      arena->end = arena->start + size;
      arena->ptr = arena->start;

      return arena;
    }
    free(arena);
  }

  return NULL;
}

void ezC_destroy_arena(ezC_arena *arena) {
  if (arena) {
    free(arena->start);
    free(arena);
  }
}

void *ezC_arena_start(ezC_arena *arena) {
  return arena->start;
}

void *ezC_arena_end(ezC_arena *arena) {
  return arena->end;
}

size_t ezC_arena_size(ezC_arena *arena) {
  return arena->end - arena->start;
}

int ezC_arena_is_full(ezC_arena *arena) {
  return arena->ptr == arena->end;
}

int ezC_arena_resize(ezC_arena *arena, size_t size) {
  if (arena->start + size >= arena->ptr) {
    u8 *start = realloc(arena->start, size);
    if (start) {
      ptrdiff_t off = arena->ptr - arena->start;

      arena->start = start;
      arena->end = start + size;
      arena->ptr = start + off;

      return 0;
    }

    return -1;
  }

  errno = EINVAL;

  return -1;
}

void *ezC_arena_alloc(ezC_arena *arena, size_t size) {
  vp_t ptr = NULL;
  if (arena->ptr + size <= arena->end) {
    ptr = arena->ptr;
    arena->ptr += size;
  }
  else
    errno = ENOMEM;

  return ptr;
}

int ezC_arena_dealloc(ezC_arena *arena, size_t size) {
  if (arena->ptr - size >= arena->start) {
    arena->ptr -= size;

    return 0;
  }

  errno = EINVAL;

  return -1;
}

#define IS_POW2(val) \
  ((val) && !((val - 1) & val))

void *ezC_arena_aligned_alloc(ezC_arena *arena, size_t size, size_t alignment) {
  if (!IS_POW2(alignment / sizeof(vp_t))) {
    errno = EINVAL;

    return NULL;
  }

  size_t off = alignment - 1;

  vp_t ptr = ezC_arena_alloc(arena, size + off);
  if (ptr) {
    vp_t aligned_ptr = (vp_t)(((uptr_t)ptr + alignment - 1) & ~(alignment - 1));

    return aligned_ptr;
  }

  return ptr;
}

