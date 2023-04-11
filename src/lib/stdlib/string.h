#include <stddef.h>

size_t strnlen_s( const char *str, size_t strsz );
char *strcpy_s( char *restrict dest, size_t destsz, const char *restrict src );
int strncmp( const char *s1, const char *s2, size_t n );
void *memcpy( void *dest, void *src, size_t n );
