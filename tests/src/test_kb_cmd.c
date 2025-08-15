#include "kb_cmd.h"
#include "string.h"

#define testStart(name, print)                                                 \
    int name##Start(int) {                                                     \
        char text[] = print " starting\n";                                     \
        serialWrite(COM1, (uint8_t *)(text), sizeof(text) - 1);                \
        return 0;                                                              \
    }

#define testEnd(name, print)                                                   \
    int name##End(int) {                                                       \
        char text[] = print " completed\n";                                    \
        serialWrite(COM1, (uint8_t *)(text), sizeof(text) - 1);                \
        return 0;                                                              \
    }

char buff[64];
int buffIdx = 0;

int resetBuff(int) {
    memset(buff, 0, sizeof(buff));
    buffIdx = 0;
    return 0;
}

void testKeyHandler(struct PS2Buf_t buf) {
    buff[buffIdx++] = keyPressToASCII(buf.keyEvent);
}

// clang-format off

// Loop prints
testStart(loopTest, "loop test")
testEnd(loopTest, "loop test")

// Nesting prints
testStart(nestingTest, "nesting test")
testEnd(nestingTest, "nesting test")

// Single Iteration prints
testStart(singleIterationTest, "single iteration test")
testEnd(singleIterationTest, "single iteration test")

// Char table prints
testStart(charTableTest, "char table test")
testEnd(charTableTest, "char table test")

// Word prints
testStart(wordTest, "word test")
testEnd(wordTest, "word test")

// Complex word prints
testStart(complexWordTest, "complex word test")
testEnd(complexWordTest, "complex word test")

// Loop testing
int v = 0;

// clang-format on

int vReset(int) {
    v = 0;
    return 0;
}

int vInc(int) {
    v++;
    return 0;
}

int testSingleLoop(int) {
    ASSERT_M(v == 4, "Expected to loop 4 times, looped %i time(s) instead", v);
    return 0;
}

int testNestedLoop(int) {
    ASSERT_M(v == 6, "Expected to loop 6 times, looped %i time(s) instead", v);
    return 0;
}

int testSingleIteration(int) {
    ASSERT_M(v == 1, "Expected to loop 1 time, looped %i time(s) instead", v);
    return 0;
}

// Char table testing
int capitalShiftTest(int) {
    ASSERT(Key_a == charToPressCMD['a'].c);
    ASSERT(0 == charToPressCMD['a'].mods);
    ASSERT(Key_a == charToPressCMD['A'].c);
    ASSERT(KEY_MOD_SHIFT == charToPressCMD['A'].mods)
    return 0;
}

int shiftTest(int) {
    ASSERT(Key_grave == charToPressCMD['`'].c)
    ASSERT(0 == charToPressCMD['`'].mods)
    ASSERT(Key_grave == charToPressCMD['~'].c)
    ASSERT(KEY_MOD_SHIFT == charToPressCMD['~'].mods)
    return 0;
}

// Word type testing
int wordTest(int) {
    ASSERT(strncmp("test", buff, 5) == 0)
    return 0;
}

int complexWordTest(int) {
    ASSERT(strncmp("This is a very Loong word$%@^@\\", buff, 32) == 0)
    return 0;
}

void test_main() {
    struct KbCmd list[] = {
        // Single loop test
        funcCMD(loopTestStart),
        loopStartCMD(4),
        funcCMD(vInc),
        loopEndCMD(),
        funcCMD(testSingleLoop),
        funcCMD(loopTestEnd),

        funcCMD(vReset),
        // Nested loop test
        funcCMD(nestingTestStart),
        loopStartCMD(2),
        loopStartCMD(3),
        funcCMD(vInc),
        loopEndCMD(),
        loopEndCMD(),
        funcCMD(testNestedLoop),
        funcCMD(nestingTestEnd),

        funcCMD(vReset),
        // Single iteration test
        funcCMD(singleIterationTestStart),
        loopStartCMD(1),
        funcCMD(vInc),
        loopEndCMD(),
        funcCMD(testSingleIteration),
        funcCMD(singleIterationTestEnd),

        // Char table values test
        funcCMD(charTableTestStart),
        funcCMD(capitalShiftTest),
        funcCMD(shiftTest),
        funcCMD(charTableTestEnd),

        // Word type test
        funcCMD(wordTestStart),
        typeWordCMD("test"),
        funcCMD(wordTest),
        funcCMD(wordTestEnd),

        funcCMD(resetBuff),

        funcCMD(complexWordTestStart),
        typeWordCMD("This is a very Loong word$%@^@\\"),
        funcCMD(complexWordTest),
        funcCMD(complexWordTestEnd),

        endCMD(),
    };
    execList(list, baseExec, testKeyHandler);
    char done[] = "test_kb_cmd done\n";
    serialWrite(COM1, (uint8_t *)(done), sizeof(done) - 1);
}