#ifndef EZC_UTIL_LINKED_LIST_H
#define EZC_UTIL_LINKED_LIST_H

#include <stddef.h>

#include "ezC/util/list_iterator.h"

typedef struct ezC_linked_list ezC_linked_list;

ezC_linked_list *ezC_create_linked_list(size_t dsize);

void ezC_destroy_linked_list(ezC_linked_list *list);

int ezC_linked_list_is_empty(ezC_linked_list *list);

int ezC_linked_list_append(ezC_linked_list *list, void *data);

int ezC_linked_list_pop_back(ezC_linked_list *list, void *dret);

int ezC_linked_list_insert(ezC_linked_list *list, size_t pos, void *data);

int ezC_linked_list_remove(ezC_linked_list *list, size_t pos);

void *ezC_linked_list_get(ezC_linked_list *list, size_t pos);

// ezC_list_iterator *ezC_linked_list_create_iterator(ezC_linked_list *list);
//
// ezC_list_iterator *ezC_linked_list_create_reversed_iterator(ezC_linked_list *list);

#endif // !EZC_UTIL_LINKED_LIST_H

