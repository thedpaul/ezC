#ifndef EZC_UTIL_ITERATOR_H
#define EZC_UTIL_ITERATOR_H

typedef struct ezC_iterator {
  int (*has_next)(struct ezC_iterator *);
  void *(*next)(struct ezC_iterator *);
} ezC_iterator;

int ezC_iterator_has_next(ezC_iterator *it);

void *ezC_iterator_next(ezC_iterator *it);

#endif // !EZC_UTIL_ITERATOR_H

