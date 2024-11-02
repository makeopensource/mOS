#ifndef TEST_HELPER_H
#define TEST_HELPER_H

#include "device/serial.h"
#include "stdio.h"

#include <stdarg.h>

#define ERR_BUFF_SIZE 256

void _assert_fail_m(char *fmt, const char *assertion, unsigned line,
                    const char *function, ...) {
    va_list args;
    va_start(args, function);

    // buffer to write the messages to
    char buff[256];
    // "{function name}:{line number} Assertion '{assertion as a string}'
    // failed."
    char errfmt[] = "%s:%i Assertion '%s' failed.\n";

    int len = snprintf(buff, sizeof(buff), errfmt, function, line, assertion);
    while (!serialWriteReady(COM1))

        ;
    serialWrite(COM1, (uint8_t *)buff, len);

    // avoid an empty line
    if (fmt[0] != 0) {
        len = vsnprintf(buff, sizeof(buff), fmt, args);

        while (!serialWriteReady(COM1))
            ;
        serialWrite(COM1, (uint8_t *)buff, len);
    }

    va_end(args);
}

void _fail_m(char *fmt, unsigned line, const char *function, ...) {
    va_list args;
    va_start(args, function);

    // buffer to write the messages to
    char buff[256];
    // "{function name}:{line number} Fail assertion reached."
    char errfmt[] = "%s:%i Fail assertion reached.\n";

    int len = snprintf(buff, sizeof(buff), errfmt, function, line);

    while (!serialWriteReady(COM1))
        ;
    serialWrite(COM1, (uint8_t *)buff, len);

    // avoid an empty line
    if (fmt[0] != 0) {
        len = vsnprintf(buff, sizeof(buff), fmt, args);

        while (!serialWriteReady(COM1))
            ;
        serialWrite(COM1, (uint8_t *)buff, len);
    }

    va_end(args);
}

#define AS_STR(x) #x

// __LINE__ loves to expand as "__LINE__" instead of the linenum
#define _STR_LINE(line) AS_STR(line)
#define STR_LINE _STR_LINE(__LINE__)

#define ASSERT_M(condition, fmt, ...)                                          \
    if (!(condition)) {                                                        \
        _assert_fail_m(fmt "\n", #condition, __LINE__,                         \
                       __func__ __VA_OPT__(, ) __VA_ARGS__);                   \
    }

#define FAIL_M(fmt, ...)                                                       \
    _fail_m(fmt "\n", __LINE__, __func__ __VA_OPT__(, ) __VA_ARGS__)

#define ASSERT(condition) ASSERT_M(condition, "")

#endif