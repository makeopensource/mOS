#include <stddef.h>

int atoi(const char *str);
void itoa_s(int i, char *buf, int bufsz);

// Implements insertion sort
//
// parameters:
//   void *base:      base pointer for array
//   size_t nel:      array length
//   size_t width:    sizeof array type
//   int (*compar)(const void *a, const void *b): comparison function between
//   two void * values

void isort(void *base, size_t nel, size_t width,
           int (*compar)(const void *a, const void *b));
