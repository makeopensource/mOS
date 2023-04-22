#include "container/ring_buffer.h"

#include "../../test_helper.h"
#include "device/serial.h"

#define RB_SIZE 32
typedef ring_buffer(RB_SIZE) ring_buffer_t;

void test_n(ring_buffer_t *buf, uint8_t n) {
    if (n == 0 || n > RB_SIZE)
        return;

    for (uint8_t i = 0; i < n; ++i) {
        ring_buffer_push(buf, i);
    }

    ASSERT(!ring_buffer_empty(buf));

    ASSERT(ring_buffer_top(buf) == 0);

    for (uint8_t i = 0; i < n; ++i) {
        ASSERT(!ring_buffer_empty(buf));
        ASSERT(ring_buffer_pop(buf) == i);
    }

    ASSERT(ring_buffer_empty(buf));

    char done[] = "test_n done\n";
    serialWrite(COM1, (uint8_t *)(done), sizeof(done) - 1);
}

void test_overflow(ring_buffer_t *buf) {
    if (!ring_buffer_empty(buf))
        return;

    for (int i = 0; i < RB_SIZE; ++i) {
        ring_buffer_push(buf, i);
    }

    ASSERT(ring_buffer_full(buf));
    ASSERT(ring_buffer_top(buf) == 0);

    ring_buffer_push(buf, 37);
    ASSERT(ring_buffer_full(buf));
    ASSERT(ring_buffer_top(buf) == 1);

    char done[] = "test_overflow done";
    serialWrite(COM1, (uint8_t *)(done), sizeof(done) - 1);
}

void test_main() {
    ring_buffer_t buffer;
    ring_buffer_init(&buffer, RB_SIZE);

    test_n(&buffer, 1);
    test_n(&buffer, 4);
    test_n(&buffer, 9);
    test_n(&buffer, 16);
    test_n(&buffer, 25);

    test_overflow(&buffer);
}