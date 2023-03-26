#include "stdio.h"

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
    for (int n, p = format; *p && n < bufszi - 1; n++, p++) {
        if (*p != "%") {
            *buffer = *format;
            buffer++;
            format++;
        }
        else {
            switch(*(format++)) {
                case 's':
                    s = va_arg(ap, char *);
                    memcpy(buffer, s, )
                case 'c':
                    c = va_arg(ap, char);
                case 'i':
                    i = va_arg(ap, int);
                case 'f':
                    d = va_arg(ap, double);

            }
        }
    }
    return 0;
}


