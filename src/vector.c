#include "ezC/util/vector.h"

#include <stdlib.h>
#include <string.h>

#include "ezC/util/arena.h"
#include "ezC/util/iterator.h"
#include "ezC/util/list_iterator.h"
#include "ezC/util/types.h"

struct ezC_vector {
  size_t size;
  size_t dsize;
  ezC_arena *arena;
};

ezC_vector *ezC_create_vector(size_t cap, size_t dsize) {
  ezC_vector *vector = malloc(sizeof(*vector));
  if (vector) {
    vector->size = 0;
    vector->dsize = dsize;
    vector->arena = ezC_create_arena(((cap) ? cap : 16) * dsize);
    if (vector->arena)
      return vector;
    free(vector);
  }

  return NULL;
}

void ezC_destroy_vector(ezC_vector *vector) {
  if (vector) {
    ezC_destroy_arena(vector->arena);
    free(vector);
  }
}

void *ezC_vector_at(ezC_vector *vector, size_t pos) {
  if (pos >= vector->size)
    return NULL;
  return (vp_t)((uptr_t)ezC_arena_start(vector->arena) + pos * vector->dsize);
}

size_t ezC_vector_size(ezC_vector *vector) {
  return vector->size;
}

int ezC_vector_resize(ezC_vector *vector, size_t new_cap) {
  return ezC_arena_resize(vector->arena, new_cap * vector->dsize);
}

int ezC_vector_prepend(ezC_vector *vector, void *p) {
  return ezC_vector_insert(vector, 0, p);
}

int ezC_vector_append(ezC_vector *vector, void *p) {
  if (ezC_arena_is_full(vector->arena)) {
    size_t cap = ezC_arena_size(vector->arena) / vector->dsize;
    if (ezC_vector_resize(vector, cap * 2)) {
      size_t c;
      for (c = 16; c && ezC_vector_resize(vector, cap + c); --c);

      if (!c)
        return -1;
    }
  }

  vp_t data = ezC_arena_alloc(vector->arena, vector->dsize);
  if (data) {
    memcpy(data, p, vector->dsize);
    ++vector->size;

    return 0;
  }

  return -1;
}

int ezC_vector_pop_back(ezC_vector *vector, void *p) {
  return ezC_vector_pop(vector, vector->size - 1, p);
}

int ezC_vector_remove_back(ezC_vector *vector) {
  --vector->dsize;
  return ezC_arena_dealloc(vector->arena, vector->dsize);
}

int ezC_vector_insert(ezC_vector *vector, size_t pos, void *p) {
  if (pos > vector->size)
    return -1;

  if (ezC_vector_append(vector, p))
    return -1;

  void *data = ezC_vector_at(vector, vector->size - 1);
  void *tmp = malloc(vector->dsize);
  memcpy(tmp, data, vector->dsize);
  uptr_t loc = (uptr_t)ezC_arena_start(vector->arena) + pos * vector->dsize;
  data = (vp_t)loc;
  memmove((vp_t)(loc + vector->dsize), data, (vector->size - pos - 1) * vector->dsize);
  memcpy(data, tmp, vector->dsize);
  free(tmp);

  return 0;
}

int ezC_vector_pop(ezC_vector *vector, size_t pos, void *p) {
  size_t cap = ezC_arena_size(vector->arena) / vector->dsize;
  if (2 * vector->size < cap)
    ezC_vector_resize(vector, cap / 2);

  vp_t data = ezC_vector_at(vector, pos);
  if (data) {
    memcpy(p, data, vector->dsize);
    --vector->size;
    memmove(data, (vp_t)((uptr_t)data + vector->dsize), (vector->size - pos) * vector->dsize);

    return 0;
  }

  return -1;
}

int ezC_vector_remove(ezC_vector *vector, size_t pos) {
  size_t cap = ezC_arena_size(vector->arena) / vector->dsize;
  if (2 * vector->size < cap)
    ezC_vector_resize(vector, cap / 2);

  void *data = ezC_vector_at(vector, pos);
  if (data) {
    --vector->size;
    memmove(data, (vp_t)((uptr_t)data + vector->dsize), (vector->size - pos) * vector->dsize);

    return 0;
  }

  return -1;
}

typedef struct {
  ezC_list_iterator _super;
  ezC_vector *vector;
  ssize_t cursor;
  ssize_t last_ret;
} vector_itr;

static int vector_itr_has_next(ezC_iterator *it) {
  vector_itr *itr = (vector_itr *)it;
  return itr->cursor != itr->vector->size;
}

static void *vector_itr_next(ezC_iterator *it) {
  vector_itr *itr = (vector_itr *)it;
  ssize_t i = itr->cursor;
  
  void *ret = ezC_vector_at(itr->vector, i);
  if (!ret)
    abort();
  
  itr->last_ret = i;
  ++itr->cursor;

  return ret;
}

static int vector_itr_has_prev(ezC_list_iterator *it) {
  vector_itr *itr = (vector_itr *)it;
  return itr->cursor != 0;
}

static void *vector_itr_prev(ezC_list_iterator *it) {
  vector_itr *itr = (vector_itr *)it;
  ssize_t i = itr->cursor - 1;
  if (i < 0)
    abort();
  itr->cursor = i;
  itr->last_ret = i;
  return ezC_vector_at(itr->vector, (size_t)i);
}

static size_t vector_itr_next_index(ezC_list_iterator *it) {
  vector_itr *itr = (vector_itr *)it;
  return itr->cursor;
}

static size_t vector_itr_prev_index(ezC_list_iterator *it) {
  vector_itr *itr = (vector_itr *)it;
  return itr->cursor - 1;
}

static void vector_itr_remove(ezC_list_iterator *it) {
  vector_itr *itr = (vector_itr *)it;
  if (itr->cursor == -1)
    abort();
  ezC_vector_remove(itr->vector, (size_t)itr->cursor);
  itr->cursor = itr->last_ret;
  itr->last_ret = -1;
}

static void vector_itr_set(ezC_list_iterator *it, void *e) {
  vector_itr *itr = (vector_itr *)it;
  if (itr->last_ret == -1)
    abort();
  memcpy(ezC_vector_at(itr->vector, itr->cursor), e, itr->vector->dsize);
}

static void vector_itr_add(ezC_list_iterator *it, void *e) {
  vector_itr *itr = (vector_itr *)it;
  if (ezC_vector_insert(itr->vector, itr->cursor, e))
    abort();
  ++itr->cursor;
  itr->last_ret = -1;
}

ezC_list_iterator *ezC_vector_create_iterator(ezC_vector *vector, ssize_t pos) {
  vector_itr *it = malloc(sizeof(*it));
  if (it) {
    it->_super._super.has_next = vector_itr_has_next;
    it->_super._super.next = vector_itr_next;
    it->_super.has_prev = vector_itr_has_prev;
    it->_super.prev = vector_itr_prev;
    it->_super.next_index = vector_itr_next_index;
    it->_super.prev_index = vector_itr_prev_index;
    it->_super.remove = vector_itr_remove;
    it->_super.set = vector_itr_set;
    it->_super.add = vector_itr_add;
    it->vector = vector;
    it->cursor = (pos < 0) ? it->vector->size + 1 + pos : pos;
    it->last_ret = -1;
  }

  return (ezC_list_iterator *)it;
}

ezC_list_iterator *ezC_vector_create_reversed_iterator(ezC_vector *vector, ssize_t pos) {
  vector_itr *it = (vector_itr *)ezC_vector_create_iterator(vector, pos);
  if (it) {
    it->_super._super.has_next = (int (*)(ezC_iterator *))vector_itr_has_prev;
    it->_super._super.next = (void *(*)(ezC_iterator *))vector_itr_prev;
  }

  return (ezC_list_iterator *)it;
}

