#include "test.h"

#include "device/serial.h"

typedef void(*vfunc_t)(void);

#define MEMORY_BEGIN 0x100000
vfunc_t program = (vfunc_t)(MEMORY_BEGIN);
uint8_t* current = (uint8_t*)(MEMORY_BEGIN);

void enterTesting(void) {

    serialWrite(COM1, (uint8_t*)("begin test"), 10);

    current = (uint8_t*)(MEMORY_BEGIN);

    uint32_t size;
    serialRead(COM1, (uint8_t*)(&size), sizeof(size));

    while ((size_t)(current) - MEMORY_BEGIN < size) {
        size_t bytes = serialReadReady(COM1);

        serialRead(COM1, current, bytes);

        current += bytes;
    }

    program();
}