#include "ezC/util/iterator.h"

int ezC_iterator_has_next(ezC_iterator *it) {
  return it->has_next(it);
}

void *ezC_iterator_next(ezC_iterator *it) {
  return it->next(it);
}

