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
struct LoopNode loopStack[8];
int current = -1;

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

#define keyDataStruct(code, modifiers)                                        \
    (struct KeyData) {code, modifiers}

#define keyAndShifted(unshiftedChar, shiftedChar, key) \
    [unshiftedChar] = keyDataStruct(key, 0), \
    [shiftedChar] = keyDataStruct(key, KEY_MOD_SHIFT),

#define keyAndCapital(char, key) \
    [char] = keyDataStruct(key, 0), \
    [char - 32] = keyDataStruct(key, KEY_MOD_SHIFT),

struct KeyData charToPressCMD[] = {
    // Row 1
    keyAndShifted('`', '~', Key_grave)
    keyAndShifted('1', '!', Key_1)
    keyAndShifted('2', '@', Key_2)
    keyAndShifted('3', '#', Key_3)
    keyAndShifted('4', '$', Key_4)
    keyAndShifted('5', '%', Key_5)
    keyAndShifted('6', '^', Key_6)
    keyAndShifted('7', '&', Key_7)
    keyAndShifted('8', '*', Key_8)
    keyAndShifted('9', '(', Key_9)
    keyAndShifted('0', ')', Key_0)
    keyAndShifted('-', '_', Key_minus)
    keyAndShifted('=', '+', Key_equal)

    // Row 2
    keyAndCapital('q', Key_q)
    keyAndCapital('w', Key_w)
    keyAndCapital('e', Key_e)
    keyAndCapital('r', Key_r)
    keyAndCapital('t', Key_t)
    keyAndCapital('y', Key_y)
    keyAndCapital('u', Key_u)
    keyAndCapital('i', Key_i)
    keyAndCapital('o', Key_o)
    keyAndCapital('p', Key_p)
    keyAndShifted('[', '{', Key_openSquare)
    keyAndShifted(']', '}', Key_closeSquare)
    keyAndShifted('\\', '|', Key_backSlash)

    // Row 3
    keyAndCapital('a', Key_a)
    keyAndCapital('s', Key_s)
    keyAndCapital('d', Key_d)
    keyAndCapital('f', Key_f)
    keyAndCapital('g', Key_g)
    keyAndCapital('h', Key_h)
    keyAndCapital('j', Key_j)
    keyAndCapital('k', Key_k)
    keyAndCapital('l', Key_l)
    keyAndShifted(';', ':', Key_semicolon)
    keyAndShifted('\'', '"', Key_apostrophe)

    // Row 4
    keyAndCapital('z', Key_z)
    keyAndCapital('x', Key_x)
    keyAndCapital('c', Key_c)
    keyAndCapital('v', Key_v)
    keyAndCapital('b', Key_b)
    keyAndCapital('n', Key_n)
    keyAndCapital('m', Key_m)
    keyAndShifted(',', '<', Key_comma)
    keyAndShifted('.', '>', Key_period)
    keyAndShifted('/', '?', Key_slash)
};

#undef keyPressStruct
#undef keyAndShifted
#undef keyAndCapital

struct KbCmd keyPressCMD(enum KeyCode code, enum KeyState event,
                         uint8_t modifiers) {
    KeyPress kp = (KeyPress){0, code, event, modifiers};
    struct PS2Buf_t b = (struct PS2Buf_t){PS2_KEY_EVENT, {.keyEvent = kp}};
    return (struct KbCmd){CMD_KEY_PRESS, {.kb = b}};
}

struct KbCmd keyPressCMDFromData(struct KeyData data) {
    KeyPress kp = (KeyPress){0, data.c, KeyPressed, data.mods};
    struct PS2Buf_t b = (struct PS2Buf_t){PS2_KEY_EVENT, {.keyEvent = kp}};
    return (struct KbCmd){CMD_KEY_PRESS, {.kb = b}};
}

struct KbCmd typeWordCMD(char *word) {
    return (struct KbCmd){CMD_TYPE_WORD, {.w = word}};
}

// `loops` must be greater than 0
struct KbCmd loopStartCMD(int loops) {
    return (struct KbCmd){CMD_LOOP_START, {.loops = loops}};
}

struct KbCmd loopEndCMD() {
    return (struct KbCmd){CMD_LOOP_END, {}};
}

struct KbCmd funcCMD(CmdFunc func) {
    return (struct KbCmd){CMD_FUNC, {.func = func}};
}

struct KbCmd endCMD() {
    return (struct KbCmd){CMD_END, {}};
}

// Returns 0 when exiting normally, and anything else when shit went fuck
typedef int (*ExecFunc)(struct KbCmd, int *);

int baseExec(struct KbCmd cmd, int *idx) {
    switch (cmd.cmd) {
    case CMD_KEY_PRESS:
        vgaEditor(cmd.data.kb);
        break;
    case CMD_TYPE_WORD:
        for(char *c = cmd.data.w; c != 0; c++) {
            vgaEditor(keyPressCMDFromData(charToPressCMD[(int)*c]).data.kb);
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

int execList(struct KbCmd *cmds, ExecFunc f) {
    for (int i = 0; cmds[i].cmd != CMD_END; i++) {
        if (f(cmds[i], &i)) {
            char buff[32];
            int len = snprintf(buff, 32, "Command %i failed", i);
            serialWrite(COM1, (uint8_t *)buff, len);
            return 1;
        }
    }
    return 0;
}

#endif
