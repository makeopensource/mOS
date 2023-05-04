#include "container/ring_buffer.h"

#include "../../test_helper.h"
#include "device/serial.h"

#define RB_SIZE 32
typedef ring_buffer(RB_SIZE) ring_buffer_byte_t;

void test_n(ring_buffer_byte_t *buf, uint8_t n) {
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

void test_overflow(ring_buffer_byte_t *buf) {
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

struct TestTriple {
    uint32_t a;
    char b;
    const struct TestTriple *c;
};

static const struct TestTriple TestTripleInitVal = {0, '\0', NULL};

typedef ring_buffer_g(RB_SIZE, struct TestTriple) ring_buffer_triple_t;

void test_type(void) {
    ring_buffer_triple_t buf;
    ring_buffer_init(&buf, TestTripleInitVal);

    ASSERT(ring_buffer_empty(&buf));

    struct TestTriple tst = TestTripleInitVal;
    tst.a = 0xc0decafe;
    tst.b = '\n';
    tst.c = &TestTripleInitVal;

    ring_buffer_push_g(&buf, tst);

    ASSERT(!ring_buffer_empty(&buf));
    ASSERT(ring_buffer_top_g(&buf).a == tst.a);
    ASSERT(ring_buffer_top_g(&buf).b == tst.b);
    ASSERT(ring_buffer_top_g(&buf).c == tst.c);

    struct TestTriple tst2;
    tst2.a = 0xdeadbeef;
    tst2.b = 'W';
    tst2.c = &tst;

    ring_buffer_push_g(&buf, tst2);
    ASSERT(!ring_buffer_empty(&buf));
    ASSERT(tst.a == ring_buffer_top_g(&buf).a);
    ASSERT(tst.b == ring_buffer_top_g(&buf).b);
    ASSERT(tst.c == ring_buffer_top_g(&buf).c);

    struct TestTriple dest;
    ring_buffer_pop_g(&buf, dest);

    ASSERT(dest.a == tst.a);
    ASSERT(dest.b == tst.b);
    ASSERT(dest.c == tst.c);

    ring_buffer_pop_g(&buf, dest);

    ASSERT(dest.a == tst2.a);
    ASSERT(dest.b == tst2.b);
    ASSERT(dest.c == tst2.c);
    ASSERT(ring_buffer_empty(&buf));

    char done[] = "test_type done\n";
    serialWrite(COM1, (uint8_t *)(done), sizeof(done) - 1);
}

void test_main() {
    ring_buffer_byte_t buffer;
    ring_buffer_init(&buffer, 0);

    test_n(&buffer, 1);
    test_n(&buffer, 4);
    test_n(&buffer, 9);
    test_n(&buffer, 16);
    test_n(&buffer, 25);

    test_type();

    test_overflow(&buffer);
}