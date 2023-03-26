#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#define MAX_SNPRINTF_STRING 100


// Inspired by chapter 7.3 of The C Programming Language
int sn_printf( char *restrict buffer, size_t bufsz,
        char *format, ... ) {
    va_list ap;
    char *p;

    // valid types
    unsigned char c;     // printf("%c\n", 'c');
    char *s;    // printf("%s\n", "hello, world!");
    // int i;      // printf("%i\n", 42);
    // double f;   // printf("%f\n", 42.0);

    va_start(ap, format);
    int n = 0;
    for (p = format; *p != '\0' && n < bufsz - 1; n++, p++) {
        if (*p != '%') {
            *buffer = *p;
            buffer++;
        }
        else {
            p++;
            char fmtchar = *p;
            switch(fmtchar) {
                // string formatting
                case 's':
                    s = va_arg(ap, char *);
                    size_t strlen = strnlen_s(s, MAX_SNPRINTF_STRING);

                    // checks if there is space for the entire string
                    // plus the null-terminating byte
                    if (strlen + n + 1 < bufsz) {
                        memcpy(buffer, s, strlen);
                        buffer += strlen;
                    } else {
                        return n;
                    }

                    break;

                // character formatting
                case 'c':
                    c = (char) va_arg(ap, unsigned int);
                    memcpy(buffer, &c, sizeof(char));
                    buffer++;
                    break;

                // integer formatting
                // case 'i':
                //     i = va_arg(ap, int);
                //     s = atoi(i);
                //     int strlen = strnlen_s(s, MAX_SNPRINTF_STRING);
                //     memcpy(buffer, s, strlen);
                //     buffer += sizeof(int);
                // case 'f':
                //     f = va_arg(ap, double);
                //     memcpy(buffer, s, sizeof(double));
                //     buffer += sizeof(double);
            }
        }
    }
    va_end(ap);

    if (bufsz != 0) {
        *buffer = '\0';
    }
    return n;
}


