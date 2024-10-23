#include "../../test_helper.h"
#include "../../../../src/lib/device/keyboard.h"
#include "../../../../src/lib/device/ps2.h"
#include "stdio.h"
#include "string.h"

#include <stdarg.h>

struct PS2Buf_t kb (enum KeyCode code, enum KeyState event, uint8_t modifiers) {
    KeyPress kp = (KeyPress){0, code, event, modifiers};
    struct PS2Buf_t b = (struct PS2Buf_t){PS2_KEY_EVENT, {.keyEvent = kp}};
    return b;
}

void test_main() {
    #define NUMKEYS 2
    struct PS2Buf_t b[NUMKEYS] = {
        kb(Key_a, KeyPressed, 0),
        kb(Key_b, KeyPressed, 0),
    };
    for (int i = 0; i < NUMKEYS; i++) {
        vgaEditor(b[i]);
    }
    ASSERT(1);
    char done[] = "test_keyboard done\n";
    serialWrite(COM1, (uint8_t *)(done), sizeof(done) - 1);
}
