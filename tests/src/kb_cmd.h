#ifndef KB_CMD_H
#define KB_CMD_H

#include "device/keyboard.h"
#include "device/ps2.h"
#include "test_helper.h"

// Returns 0 if executed successfully, non-zero otherwise
typedef int (*CmdFunc)(void);

struct LoopNode {
    int idx;
    int remaining;
};

// if you have more than 8 nested loops then what the fuck
static struct LoopNode loopStack[8];
static int current = -1;

typedef enum {
    // A singular key press
    CMD_KEY_PRESS,

    // Types the entire provided word
    CMD_TYPE_WORD,

    // Signifies the start of a command loop
    CMD_LOOP_START,

    // Signifies the end of the most recent loop command
    CMD_LOOP_END,

    // Runs a function
    CMD_FUNC,

    // End of command list
    CMD_END
} CMD;

struct KbCmd {
    CMD cmd;
    union {
        // key to press
        struct PS2Buf_t kb;

        // null terminated string
        char *w;

        // loop count
        int loops;

        // function to execute
        CmdFunc func;
    } data;
};

struct KeyData {
    enum KeyCode c;
    uint8_t mods;
};

#define keyDataStruct(code, modifiers)                                         \
    (struct KeyData) {                                                         \
        code, modifiers                                                        \
    }

#define keyAndShifted(unshiftedChar, shiftedChar, key)                         \
    [unshiftedChar] = keyDataStruct(key, 0), [shiftedChar] = keyDataStruct(    \
                                                 key, KEY_MOD_SHIFT)

// clang-format off
static const struct KeyData charToPressCMD[] = {
    // Row 1
    keyAndShifted('`', '~', Key_grave),
    keyAndShifted('1', '!', Key_1),
    keyAndShifted('2', '@', Key_2),
    keyAndShifted('3', '#', Key_3),
    keyAndShifted('4', '$', Key_4),
    keyAndShifted('5', '%', Key_5),
    keyAndShifted('6', '^', Key_6),
    keyAndShifted('7', '&', Key_7),
    keyAndShifted('8', '*', Key_8),
    keyAndShifted('9', '(', Key_9),
    keyAndShifted('0', ')', Key_0),
    keyAndShifted('-', '_', Key_minus),
    keyAndShifted('=', '+', Key_equal),

    // Row 2
    keyAndShifted('q', 'Q', Key_q),
    keyAndShifted('w', 'W', Key_w),
    keyAndShifted('e', 'E', Key_e),
    keyAndShifted('r', 'R', Key_r),
    keyAndShifted('t', 'T', Key_t),
    keyAndShifted('y', 'Y', Key_y),
    keyAndShifted('u', 'U', Key_u),
    keyAndShifted('i', 'I', Key_i),
    keyAndShifted('o', 'O', Key_o),
    keyAndShifted('p', 'P', Key_p),
    keyAndShifted('[', '{', Key_openSquare),
    keyAndShifted(']', '}', Key_closeSquare),
    keyAndShifted('\\', '|', Key_backSlash),

    // Row 3
    keyAndShifted('a', 'A', Key_a),
    keyAndShifted('s', 'S', Key_s),
    keyAndShifted('d', 'D', Key_d),
    keyAndShifted('f', 'F', Key_f),
    keyAndShifted('g', 'G', Key_g),
    keyAndShifted('h', 'H', Key_h),
    keyAndShifted('j', 'J', Key_j),
    keyAndShifted('k', 'K', Key_k),
    keyAndShifted('l', 'L', Key_l),
    keyAndShifted(';', ':', Key_semicolon),
    keyAndShifted('\'', '"', Key_apostrophe),

    // Row 4
    keyAndShifted('z', 'Z', Key_z),
    keyAndShifted('x', 'X', Key_x),
    keyAndShifted('c', 'C', Key_c),
    keyAndShifted('v', 'V', Key_v),
    keyAndShifted('b', 'B', Key_b),
    keyAndShifted('n', 'N', Key_n),
    keyAndShifted('m', 'M', Key_m),
    keyAndShifted(',', '<', Key_comma),
    keyAndShifted('.', '>', Key_period),
    keyAndShifted('/', '?', Key_slash),

    // Row 5
    [' '] = keyDataStruct(Key_space, 0)
};
// clang-format on
// clang-format can you please not butcher the readability of the code?

#undef keyPressStruct
#undef keyAndShifted
#undef keyAndCapital

static struct KbCmd keyPressCMD(enum KeyCode code, enum KeyState event,
                                uint8_t modifiers) {
    KeyPress kp = (KeyPress){0, code, event, modifiers};
    struct PS2Buf_t b = (struct PS2Buf_t){PS2_KEY_EVENT, {.keyEvent = kp}};
    return (struct KbCmd){CMD_KEY_PRESS, {.kb = b}};
}

static struct KbCmd keyPressCMDFromData(struct KeyData data) {
    return (keyPressCMD(data.c, KeyPressed, data.mods));
}

static struct KbCmd typeWordCMD(char *word) {
    return (struct KbCmd){CMD_TYPE_WORD, {.w = word}};
}

// `loops` must be greater than 0
static struct KbCmd loopStartCMD(int loops) {
    return (struct KbCmd){CMD_LOOP_START, {.loops = loops}};
}

static struct KbCmd loopEndCMD() {
    return (struct KbCmd){CMD_LOOP_END, {}};
}

static struct KbCmd funcCMD(CmdFunc func) {
    return (struct KbCmd){CMD_FUNC, {.func = func}};
}

static struct KbCmd endCMD() {
    return (struct KbCmd){CMD_END, {}};
}

// Returns 0 when exiting normally, and anything else when things go wrong
typedef void (*KeyPressHandler)(struct PS2Buf_t);
typedef int (*ExecFunc)(struct KbCmd, int *, KeyPressHandler);

void baseKeyHandler(struct PS2Buf_t kb) {
    vgaEditor(kb);
}

static int baseExec(struct KbCmd cmd, int *idx, KeyPressHandler kp) {
    switch (cmd.cmd) {
    case CMD_KEY_PRESS:
        kp(cmd.data.kb);
        break;
    case CMD_TYPE_WORD:
        for (char *c = cmd.data.w; *c != 0; c++) {
            kp(keyPressCMDFromData(charToPressCMD[(int)*c]).data.kb);
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

static int execList(struct KbCmd *cmds, ExecFunc f, KeyPressHandler kp) {
    for (int i = 0; cmds[i].cmd != CMD_END; i++) {
        if (f(cmds[i], &i, kp)) {
            char buff[32];
            int len = snprintf(buff, sizeof(buff), "Command %i failed", i);
            serialWrite(COM1, (uint8_t *)buff, len);
            return 1;
        }
    }
    return 0;
}

#endif
