#include <stddef.h>
#include <stdint.h>

size_t strnlen_s(const char *str, size_t strsz);
char *strcpy_s(char *restrict dest, size_t destsz, const char *restrict src);
int strncmp(const char *s1, const char *s2, size_t n);
void *memcpy(void *restrict dest, void *restrict src, size_t n);
void memset(void *restrict dest, uint8_t c, size_t n);
void memswap(void *restrict src_a, void *restrict src_b, size_t n);
