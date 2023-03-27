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
    unsigned char c;    // printf("%c\n", 'c');
    char *s;            // printf("%s\n", "hello, world!");
    int i;              // printf("%i\n", 42);
    // double f;        // printf("%f\n", 42.0);

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
            size_t strlen;
            switch(fmtchar) {
                // print "%"
                case '%':
                    *buffer = *p;
                    buffer++;

                // string formatting
                case 's':
                    s = va_arg(ap, char *);
                    strlen = strnlen_s(s, MAX_SNPRINTF_STRING);

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
                    c = (char) va_arg(ap, int);
                    memcpy(buffer, &c, sizeof(char));
                    buffer++;
                    break;

                case 'i':
                    i = va_arg(ap, int);
                    int digits = 0;
                    for (int temp = i; temp >= 1; temp *= 0.1, digits++)
		                ;
                    char temp_buffer[digits];
                    itoa(i, temp_buffer);
                    strlen = strnlen_s(temp_buffer, MAX_SNPRINTF_STRING);

                    // checks if there is space for the entire string
                    // plus the null-terminating byte
                    if (strlen + n + 1 < bufsz) {
                        memcpy(buffer, temp_buffer, strlen);
                        buffer += strlen;
                    } else {
                        return n;
                    }
                // case 'f':
                //     f = va_arg(ap, double);
                // default:
                //     *buffer = *p;
                //     buffer++;
            }
        }
    }
    va_end(ap);

    if (bufsz != 0) {
        *buffer = '\0';
    }
    return n;
}


