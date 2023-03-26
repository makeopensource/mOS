#include <stddef.h>
#include "string.h"

// returns the length of a string
// return 0 if str is NULL
// return strsz if null byte does not appear in strsz chars
// otherwise return strlen(str)
size_t strnlen_s( const char *str, size_t strsz ) {
    if (str == NULL)
        return 0;

    size_t i;
    for (i = 0; i < strsz && str[i] != '\0'; i++)
        ;
    return i;
}

// compares
int strncmp( const char *s1, const char *s2, size_t n ) {
    for (int i = 0; i < n; i++, s1++, s2++) {
        if (*s1 < *s2)
            return -1;
        else if (*s1 > *s2)
            return 1;
        else if (*s1 == '\0' && *s2 == '\0')
            return 0;
    }
    return 0;
}

// We cannot have the exact definition as in C11, since we do not
// have access to <errno.h>. Return value defaults to dest ptr,
// used in C99 strcpy.
char *strcpy_s( char *restrict dest, size_t destsz, const char *restrict src ) {
    for (int i = 0; i < destsz - 1; i++) {
        dest[i] = src[i];
    }
    dest[destsz-1] = '\0';
    return dest;
}

void *memcpy( void *restrict dest, void *restrict src, size_t n ) {
    for (int i = 0; i < n; i++) {
        ((char *)dest)[i] = ((char *)src)[i];
    }
    return dest;
}
