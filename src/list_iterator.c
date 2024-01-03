#include "ezC/util/list_iterator.h"

int ezC_list_iterator_has_prev(ezC_list_iterator *it) {
  return it->has_prev(it);
}

void *ezC_list_iterator_prev(ezC_list_iterator *it) {
  return it->prev(it);
}

size_t ezC_list_iterator_next_index(ezC_list_iterator *it) {
  return it->next_index(it);
}

size_t ezC_list_iterator_prev_index(ezC_list_iterator *it) {
  return it->prev_index(it);
}

void ezC_list_iterator_remove(ezC_list_iterator *it) {
  it->remove(it);
}

void ezC_list_iterator_set(ezC_list_iterator *it, void *e) {
  it->set(it, e);
}

void ezC_list_iterator_add(ezC_list_iterator *it, void *e) {
  it->add(it, e);
}

