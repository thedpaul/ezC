#ifndef EZC_VECTOR_H
#define EZC_VECTOR_H

#include <stddef.h>
#include <sys/types.h>

#include "ezC/util/list_iterator.h"

typedef struct ezC_vector ezC_vector;

ezC_vector *ezC_create_vector(size_t cap, size_t dsize);

void ezC_destroy_vector(ezC_vector *vector);

void *ezC_vector_at(ezC_vector *vector, size_t pos);

size_t ezC_vector_size(ezC_vector *vector);

int ezC_vector_resize(ezC_vector *vector, size_t new_cap);

int ezC_vector_prepend(ezC_vector *vector, void *p);

int ezC_vector_append(ezC_vector *vector, void *p);

int ezC_vector_pop_back(ezC_vector *vector, void *p);

int ezC_vector_remove_back(ezC_vector *vector);

int ezC_vector_insert(ezC_vector *vector, size_t pos, void *p);

int ezC_vector_pop(ezC_vector *vector, size_t pos, void *p);

int ezC_vector_remove(ezC_vector *vector, size_t pos);

ezC_list_iterator *ezC_vector_create_iterator(ezC_vector *vector, ssize_t pos);

ezC_list_iterator *ezC_vector_create_reversed_iterator(ezC_vector *vector, ssize_t pos);

#endif // !EZC_VECTOR_H

