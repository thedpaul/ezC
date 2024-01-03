#include "ezC/util/iterator.h"
#include "unity.h"

void setUp(void) {

}

void tearDown(void) {

}

#include <stdio.h>
#include <stdlib.h>

#include "ezC/util/vector.h"

#define NELEMS 4

static void die(const char msg[]) {
  perror(msg);
  exit(EXIT_FAILURE);
}

static void vectorTestCreateAndDestroy(void) {
  ezC_vector *vector = ezC_create_vector(0, sizeof(int));
  TEST_ASSERT_TRUE(vector);
  ezC_destroy_vector(vector);
}

static void vectorTestAppend(void) {
  ezC_vector *vector = ezC_create_vector(0, sizeof(int));
  for (int i = 0; i < NELEMS; ++i)
    TEST_ASSERT_FALSE(ezC_vector_append(vector, &i));
  ezC_destroy_vector(vector);
}

static void vectorTestPopBack(void) {
  ezC_vector *vector = ezC_create_vector(0, sizeof(int));
  int i;
  for (i = 0; i < NELEMS; ++i)
    TEST_ASSERT_FALSE(ezC_vector_append(vector, &i));
  for (; i; --i) {
    int e;
    TEST_ASSERT_FALSE(ezC_vector_pop_back(vector, &e));
    if (printf("%d\n", e) < 0)
      die("printf");
  }
  ezC_destroy_vector(vector);
}

static void vectorTestIterate(void) {
  ezC_vector *vector = ezC_create_vector(0, sizeof(int));
  int i;
  for (i = 0; i < NELEMS; ++i)
    TEST_ASSERT_FALSE(ezC_vector_append(vector, &i));
  ezC_iterator *it = (ezC_iterator *)ezC_vector_create_iterator(vector, 0);
  while (ezC_iterator_has_next(it)) {
    if (printf("%d\n", *(int *)ezC_iterator_next(it)) < 0)
      die("printf");
  }
  free(it);
  ezC_destroy_vector(vector);
}

static void vectorTestIterateReverse(void) {
  ezC_vector *vector = ezC_create_vector(0, sizeof(int));
  int i;
  for (i = 0; i < NELEMS; ++i)
    TEST_ASSERT_FALSE(ezC_vector_append(vector, &i));
  ezC_iterator *it = (ezC_iterator *)ezC_vector_create_reversed_iterator(vector, -1);
  while (ezC_iterator_has_next(it)) {
    if (printf("%d\n", *(int *)ezC_iterator_next(it)) < 0)
      die("printf");
  }
  free(it);
  ezC_destroy_vector(vector);
}

int main(void) {
  UNITY_BEGIN();

  RUN_TEST(vectorTestCreateAndDestroy);
  RUN_TEST(vectorTestAppend);
  RUN_TEST(vectorTestPopBack);
  RUN_TEST(vectorTestIterate);
  RUN_TEST(vectorTestIterateReverse);
  
  if (fflush(stdout))
    die("fflush");

  return UNITY_END();
}

