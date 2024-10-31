#include "../../../../src/lib/device/keyboard.h"
#include "../../../../src/lib/device/ps2.h"
#include "../../../../src/lib/video/VGA_text.h"
#include "../../test_helper.h"
#include "stdio.h"
#include "string.h"

#include <stdarg.h>

// in case changes are made to the width of the screen text
// we still have a portable way of selecting the final column
#define END_COL (VGA_WIDTH - 1)

extern int highlight_offset;
extern VGA_Char *cursor;

// enum representing the kinds of commands available
enum CMD { keyPress, checkOffset, checkPosition, setPosition, end };

struct TestCMD {
    enum CMD cmd;
    union {
        struct PS2Buf_t kb;
        int offset;
    } data;
};

// creates a simulated key press command
struct TestCMD kbCMD(enum KeyCode code, enum KeyState event,
                     uint8_t modifiers) {
    KeyPress kp = (KeyPress){0, code, event, modifiers};
    struct PS2Buf_t b = (struct PS2Buf_t){PS2_KEY_EVENT, {.keyEvent = kp}};
    return (struct TestCMD){keyPress, {.kb = b}};
}

// creates an offset check command
struct TestCMD chkOffCMD(int expected) {
    return (struct TestCMD){checkOffset, {.offset = expected}};
}

// creates a position check command
// expected position is relative to the beginning of vga memory
struct TestCMD chkPosCMD(unsigned line, unsigned col) {
    return (struct TestCMD){checkPosition,
                            {.offset = (line * VGA_WIDTH) + col}};
}

// creates a set position command
// position is relative to the beginning of VGA memory
struct TestCMD setPosCMD(unsigned line, unsigned col) {
    return (struct TestCMD){setPosition, {.offset = (line * VGA_WIDTH) + col}};
}

// creates an end command
struct TestCMD endCMD() {
    return (struct TestCMD){end, {}};
}

// execute the supplied command based on its `cmd` field
void execCMD(struct TestCMD cmd) {
    switch (cmd.cmd) {
    case checkOffset: // asserts that the current highlight offset matches the
                      // supplied offset
        ASSERT_M(highlight_offset == cmd.data.offset,
                 "Highlight difference | Expected: %i, Actual: %i",
                 cmd.data.offset, highlight_offset);
        break;
    case checkPosition: // asserts that the relative position from the start of
                        // vga memory matches the supplied position
        ASSERT_M(cursor - VGA_MEMORY == cmd.data.offset,
                 "Position difference | Expected: %i, Actual: %i",
                 cmd.data.offset, cursor - VGA_MEMORY);
        break;
    case setPosition:   // sets the cursor position, checks that the position is in bounds 

        cursor = VGA_MEMORY + cmd.data.offset;
        break;
    case keyPress: // simulates a keypress
        vgaEditor(cmd.data.kb);
        break;
    case end: // end command has no effect on state, it is only used as a
              // terminator
        break;
    }
}

void test_main() {
    cursor = VGA_MEMORY + 160;
    struct TestCMD b[] = {
        // The position checks assume that only 2 lines of text have been
        // written during boot
        chkPosCMD(2, 0),
        kbCMD(Key_a, KeyPressed, 0),
        chkOffCMD(0),
        chkPosCMD(2, 1),
        kbCMD(Key_left, KeyPressed, KEY_MOD_SHIFT),
        chkOffCMD(-1),
        chkPosCMD(2, 0),
        kbCMD(Key_up, KeyPressed, KEY_MOD_SHIFT),
        chkOffCMD(-VGA_WIDTH - 1),
        chkPosCMD(1, 0),
        kbCMD(Key_up, KeyPressed, KEY_MOD_SHIFT),
        chkOffCMD(-(2 * VGA_WIDTH) - 1),
        chkPosCMD(0, 0),
        kbCMD(Key_b, KeyPressed, 0),
        chkOffCMD(0),
        chkPosCMD(0, 1),
        kbCMD(Key_down, KeyPressed, KEY_MOD_SHIFT),
        chkOffCMD(VGA_WIDTH),
        chkPosCMD(1, 1),
        kbCMD(Key_up, KeyPressed, 0),
        chkOffCMD(0),
        chkPosCMD(0, 1),
        setPosCMD(0, END_COL),
        kbCMD(Key_right, KeyPressed, 0),
        chkPosCMD(1, 0),
        kbCMD(Key_left, KeyPressed, 0),
        chkPosCMD(0, END_COL),
        // Remember to end command sequence!
        endCMD(),
    };
    for (int i = 0; b[i].cmd != end; i++) {
        execCMD(b[i]);
    }
    char done[] = "test_keyboard done\n";
    serialWrite(COM1, (uint8_t *)(done), sizeof(done) - 1);
}
