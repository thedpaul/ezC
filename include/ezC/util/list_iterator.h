#ifndef EZC_UTIL_LIST_ITERATOR_H
#define EZC_UTIL_LIST_ITERATOR_H

#include <stddef.h>

#include "iterator.h"

typedef struct ezC_list_iterator {
  ezC_iterator _super;
  int (*has_prev)(struct ezC_list_iterator *);
  void *(*prev)(struct ezC_list_iterator *);
  size_t (*next_index)(struct ezC_list_iterator *);
  size_t (*prev_index)(struct ezC_list_iterator *);
  void (*remove)(struct ezC_list_iterator *);
  void (*set)(struct ezC_list_iterator *, void *);
  void (*add)(struct ezC_list_iterator *, void *);
} ezC_list_iterator;

int ezC_list_iterator_has_prev(ezC_list_iterator *it);

void *ezC_list_iterator_prev(ezC_list_iterator *it);

size_t ezC_list_iterator_next_index(ezC_list_iterator *it);

size_t ezC_list_iterator_prev_index(ezC_list_iterator *it);

void ezC_list_iterator_remove(ezC_list_iterator *it);

void ezC_list_iterator_set(ezC_list_iterator *it, void *e);

void ezC_list_iterator_add(ezC_list_iterator *it, void *e);

#endif // !EZC_UTIL_LIST_ITERATOR_H

