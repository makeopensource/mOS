#ifndef TEST_HELPER_H
#define TEST_HELPER_H

#include "device/serial.h"
#include "stdio.h"

#include <stdarg.h>

void _assert_fail_m(char *fmt, const char *assertion, const char *file,
                    unsigned line, const char *function, ...) {
    va_list args;
    va_start(args, function);
    char buff[1024];
    char errfmt[] = "%s:%i Assertion '%s' failed.\n";
    int len = snprintf(buff, 1024, errfmt, file, line, assertion);
    while (!serialWriteReady(COM1))
        ;
    serialWrite(COM1, (uint8_t *)buff, len);

    len = vsnprintf(buff, 1024, fmt, args);
    while (!serialWriteReady(COM1))
        ;
    serialWrite(COM1, (uint8_t *)buff, len);

    va_end(args);
}

#define AS_STR(x) #x

// __LINE__ loves to expand as "__LINE__" instead of the linenum
#define _STR_LINE(line) AS_STR(line)
#define STR_LINE _STR_LINE(__LINE__)

#define ASSERT_M(condition, fmt, ...)                                          \
    if (!(condition)) {                                                        \
        _assert_fail_m(fmt "\n", #condition, __FILE__, __LINE__,               \
                       __func__ __VA_OPT__(, ) __VA_ARGS__);                   \
    }

#define ASSERT(condition) ASSERT_M(condition, "")

#endif