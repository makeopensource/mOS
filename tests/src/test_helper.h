#ifndef TEST_HELPER_H
#define TEST_HELPER_H

#include "device/serial.h"

#define AS_STR(x) #x

// __LINE__ loves to expand as "__LINE__" instead of the linenum
#define _STR_LINE(line) AS_STR(line)
#define STR_LINE _STR_LINE(__LINE__)

#define ASSERT(condition)\
if (!(condition)) {\
    while (!serialWriteReady(COM1));\
    char errmsg[] = AS_STR(condition) " failed in " __FILE__ ":" STR_LINE "\n";\
    serialWrite(COM1, (uint8_t*)(errmsg), sizeof(errmsg) - 1);\
}


#endif