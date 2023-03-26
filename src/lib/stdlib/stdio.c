#include "stdio.h"
#define MAX_SNPRINTF_STRING 1000

int snprintf( char *restrict buffer, size_t bufsz,
        const char *restrict format, ... ) {
    va_list ap;
    char *p;

    // valid types
    char c;     // printf("%c\n", 'c');
    char *s;    // printf("%s\n", "hello, world!");
    int i;      // printf("%i\n", 42);
    double f;   // printf("%f\n", 42.0);

    va_start(ap, format);
    int n = 0;
    for (p = format; *p && n < bufsz - 1; n++, p++) {
        if (*p != "%") {
            *buffer = *format;
            buffer++;
            format++;
        }
        else {
            switch(*(format++)) {
                case 's':
                    s = va_arg(ap, char *);
                    size_t strlen = strnlen_s(s, MAX_SNPRINTF_STRING);
                    memcpy(buffer, s, MAX_SNPRINTF_STRING);
                    buffer += strlen;
                case 'c':
                    c = va_arg(ap, char);
                    memcpy(buffer, s, sizeof(char));
                    buffer++;
                case 'i':
                    i = va_arg(ap, int);
                    memcpy(buffer, s, sizeof(int));
                    buffer += sizeof(int);
                case 'f':
                    f = va_arg(ap, double);
                    memcpy(buffer, s, sizeof(double));
                    buffer += sizeof(double);
            }
        }
    }
    
    if (bufsz != 0) {
        *buffer = '\0';
    }
    return n;
}


