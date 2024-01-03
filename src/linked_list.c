#include "ezC/util/linked_list.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
  void *data;
  struct node *prev;
  struct node *next;
} node;

struct ezC_linked_list {
  size_t dsize;
  size_t size;
  node *sentinel;
};

node *create_node(void *data, size_t dsize, node *prev, node *next) {
  node *node = malloc(sizeof(*node));
  if (node) {
    node->data = malloc(dsize);
    if (node->data) {
      memcpy(node->data, data, dsize);
      node->prev = prev;
      node->next = next;
    }
  }

  return node;
}

void destroy_node(node *node) {
  free(node->data);
  free(node);
}

ezC_linked_list *ezC_create_linked_list(size_t dsize) {
  ezC_linked_list *list = malloc(sizeof(*list));
  if (list) {
    list->dsize = dsize;
    list->size = 0;

    list->sentinel = malloc(sizeof(*list->sentinel));
    if (list->sentinel) {
      list->sentinel->data = NULL;
      list->sentinel->prev = list->sentinel;
      list->sentinel->next = list->sentinel;

      return list;
    }
    free(list);
  }

  return NULL;
}

void ezC_destroy_linked_list(ezC_linked_list *list) {
  if (list) {
    for (node **itp = &list->sentinel->next; !ezC_linked_list_is_empty(list);) {
      node *curr = *itp;
      *itp = curr->next;
      destroy_node(curr);
    }
    destroy_node(list->sentinel);
    free(list);
  }
}

int ezC_linked_list_is_empty(ezC_linked_list *list) {
  return list->sentinel == list->sentinel->next;
}

int ezC_linked_list_append(ezC_linked_list *list, void *data) {
  node *node = create_node(data, list->dsize, list->sentinel->prev, list->sentinel);
  if (node) {
    list->sentinel->prev = list->sentinel->prev->next = node;
    ++list->size;

    return 0;
  }

  return -1;
}

int ezC_linked_list_pop_back(ezC_linked_list *list, void *dret) {
  if (!ezC_linked_list_is_empty(list)) {
    node *node = list->sentinel->prev;

    list->sentinel->prev = node->prev;
    node->prev->next = list->sentinel;

    memcpy(dret, node->data, list->dsize);

    destroy_node(node);

    --list->size;

    return 0;
  }

  return -1;
}

int ezC_linked_list_insert(ezC_linked_list *list, size_t pos, void *data) {
  node *inode = list->sentinel->next;
  for (; pos; --pos) {
    if (inode == list->sentinel) {
      errno = EINVAL;

      return -1;
    }

    inode = inode->next;
  }

  node *node = create_node(data, list->dsize, inode->prev, inode);
  if (node) {
    inode->prev->next = node;
    inode->prev = node;

    ++list->size;

    return 0;
  }

  return -1;
}

int ezC_linked_list_remove(ezC_linked_list *list, size_t pos) {
  node *inode = list->sentinel->next;
  for (; pos; --pos) {
    if (inode == list->sentinel) {
      errno = EINVAL;

      return -1;
    }

    inode = inode->next;
  }

  inode->prev->next = inode->next;
  inode->next->prev = inode->prev;

  destroy_node(inode);

  --list->size;

  return 0;
}

void *ezC_linked_list_get(ezC_linked_list *list, size_t pos) {
  node *inode = list->sentinel->next;
  for (; pos; --pos) {
    if (inode == list->sentinel) {
      errno = EINVAL;

      return NULL;
    }

    inode = inode->next;
  }

  return inode->data;
}

// TODO
// typedef struct {
//   ezC_list_iterator _super;
//   ezC_linked_list *list;
// } linked_list_itr;

