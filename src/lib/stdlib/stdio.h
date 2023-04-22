#include "string.h"

#include <stdarg.h>
#include <stddef.h>

int snprintf(char *restrict buffer, size_t bufsz, char *format, ...);

int vsnprintf(char *restrict buffer, size_t bufsz, char *format, va_list ap);
