#include "kb_cmd.h"
#include "string.h"

#define testStart(name, print)                                                 \
    int name##Start() {                                                        \
        char text[] = print " starting\n";                                     \
        serialWrite(COM1, (uint8_t *)(text), sizeof(text) - 1);                \
        return 0;                                                              \
    }

#define testEnd(name, print)                                                   \
    int name##End() {                                                          \
        char text[] = print " completed\n";                                    \
        serialWrite(COM1, (uint8_t *)(text), sizeof(text) - 1);                \
        return 0;                                                              \
    }

char buff[64];
int buffIdx = 0;

int resetBuff() {
    memset(buff, 0, 64);
    buffIdx = 0;
    return 0;
}

int testExec(struct KbCmd cmd, int *idx) {
    switch (cmd.cmd) {
    case CMD_KEY_PRESS:
        buff[buffIdx++] = keyPressToASCII(cmd.data.kb.keyEvent);
        break;
    case CMD_TYPE_WORD:
        for(char *c = cmd.data.w; *c != 0; c++) {
            buff[buffIdx++] = keyPressToASCII(keyPressCMDFromData(charToPressCMD[(int)*c]).data.kb.keyEvent);
        }
        break;
    case CMD_LOOP_START:
        if (cmd.data.loops < 1) {
            FAIL_M("Loop count must be greater than 0, got %i", cmd.data.loops);
            return 1;
        }
        if (current == -1 || loopStack[current].idx != *idx) {
            current++;
            loopStack[current] = (struct LoopNode){*idx, cmd.data.loops};
        }
        loopStack[current].remaining--;
        break;
    case CMD_LOOP_END:
        if (loopStack[current].remaining) {
            // subtract 1 because idx is incremented after command completes
            // this was a surprisingly annoying issue to track down
            *idx = loopStack[current].idx - 1;
        } else {
            current--;
        }
        break;
    case CMD_FUNC:
        return cmd.data.func();
    case CMD_END:
        break;
    default:
        FAIL_M("Unexpected command type: %i", cmd.cmd);
        return 1;
    }
    return 0;
}

// clang-format off

// Loop prints
testStart(loopTest, "loop test")
testEnd(loopTest, "loop test")

// Nesting prints
testStart(nestingTest, "nesting test")
testEnd(nestingTest, "nesting test")

// Char table prints
testStart(charTableTest, "char table test")
testEnd(charTableTest, "char table test")

// Word prints
testStart(wordTest, "word test")
testEnd(wordTest, "word test")

// Loop testing
int v = 0;

// clang-format on
// stop fucking this up

int vReset() {
    v = 0;
    return 0;
}

int vInc() {
    v++;
    return 0;
}

int testSingleLoop() {
    ASSERT_M(v == 4, "Expected to loop 4 times, looped %i time(s) instead", v);
    return 0;
}

int testNestedLoop() {
    ASSERT_M(v == 6, "Expected to loop 6 times, looped %i time(s) instead", v);
    return 0;
}

// Char table testing
int capitalShiftTest() {
    ASSERT(Key_a == charToPressCMD['a'].c);
    ASSERT(0 == charToPressCMD['a'].mods);
    ASSERT(Key_a == charToPressCMD['A'].c);
    ASSERT(KEY_MOD_SHIFT == charToPressCMD['A'].mods)
    return 0;
}

int shiftTest() {
    ASSERT(Key_grave == charToPressCMD['`'].c)
    ASSERT(0 == charToPressCMD['`'].mods)
    ASSERT(Key_grave == charToPressCMD['~'].c)
    ASSERT(KEY_MOD_SHIFT == charToPressCMD['~'].mods)
    return 0;
}

// Word type testing
int wordTest() {
    ASSERT(strncmp("test", buff, 5))
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

        // Char table values test
        funcCMD(charTableTestStart),
        funcCMD(capitalShiftTest),
        funcCMD(shiftTest),
        funcCMD(charTableTestEnd),

        // Word type test
        funcCMD(wordTestStart),
        typeWordCMD("test"),
        funcCMD(wordTestEnd),

        endCMD(),
    };
    execList(list, testExec);
    char done[] = "test_kb_cmd done\n";
    serialWrite(COM1, (uint8_t *)(done), sizeof(done) - 1);
}