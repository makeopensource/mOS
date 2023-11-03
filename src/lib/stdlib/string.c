#include "string.h"

#include <stddef.h>
#include <stdint.h>

// returns the length of a string
// return 0 if str is NULL
// return strsz if null byte does not appear in strsz chars
// otherwise return strlen(str)
size_t strnlen_s(const char *str, size_t strsz) {
    if (str == NULL)
        return 0;

    size_t i;
    for (i = 0; i < strsz && str[i] != '\0'; i++)
        ;
    return i;
}

// compares
int strncmp(const char *s1, const char *s2, size_t n) {
    for (int i = 0; i < n; i++, s1++, s2++) {
        if (*s1 != *s2)
            return *s1 - *s2;
        else if (*s1 == '\0' && *s2 == '\0')
            return 0;
    }
    return 0;
}

// We cannot have the exact definition as in C11, since we do not
// have access to <errno.h>. Return value defaults to dest ptr,
// used in C99 strcpy.
char *strcpy_s(char *restrict dest, size_t destsz, const char *restrict src) {

    if (src == NULL || dest == NULL)
        return NULL;

    if (destsz <= 0)
        return dest;

    int i;
    for (i = 0; i < destsz - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }

    dest[i] = '\0';

    return dest;
}

void *memcpy(void *restrict dest, void *restrict src, size_t n) {
    for (int i = 0; i < n; i++) {

        ((uint8_t *)dest)[i] = ((uint8_t *)src)[i];
    }
    return dest;
}

void memset(void *restrict dest, uint8_t c, size_t n) {
    for (int i = 0; i < n; i++) {
        ((uint8_t *)dest)[i] = c;
    }
}

void memswap(void *restrict src_a, void *restrict src_b, size_t n) {
    char *a = (char *)src_a;
    char *b = (char *)src_b;
    for (int i = 0; i < n; i++) {
        char swap_a = a[i];
        char swap_b = b[i];

        a[i] = swap_b;
        b[i] = swap_a;
    }
}
