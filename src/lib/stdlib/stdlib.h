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
//
// compar function:
//   3-way comparison:
//   - if a comes before b, return -1
//   - if a is equivilent to b, return 0
//   - if a comes after b, return 1

void isort(void *base, size_t nel, size_t width,
           int (*compar)(const void *a, const void *b));
