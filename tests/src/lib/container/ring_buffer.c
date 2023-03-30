
#include "device/serial.h"

void test_main() {
    serialWrite(COM1, (uint8_t*)("test"), 4);
}