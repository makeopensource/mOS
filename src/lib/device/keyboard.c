#include "keyboard.h"

#include "../pit/pit.h"

#include "stddef.h"

#define CAP_DELTA ('a' - 'A')

uint8_t getActiveModifiers(const struct KeyboardState* state) {
    uint8_t mods = 0;

    if (state->keystates[(unsigned)(Key_capsLock)] == KeyToggled) {
        mods |= KEY_MOD_CAPS;
    }

    if (state->keystates[(unsigned)(Key_LShift)] == KeyPressed || state->keystates[(unsigned)(Key_RShift)] == KeyPressed) {
        mods |= KEY_MOD_SHIFT;
    }

    if (state->keystates[(unsigned)(Key_Lctrl)] == KeyPressed || state->keystates[(unsigned)(Key_Rctrl)] == KeyPressed) {
        mods |= KEY_MOD_CTRL;
    }

    if (state->keystates[(unsigned)(Key_numLock)] == KeyToggled) {
        mods |= KEY_MOD_NUMLOCK;
    }

    if (state->keystates[(unsigned)(Key_scrollLock)] == KeyToggled) {
        mods |= KEY_MOD_SCRLOCK;
    }

    if (state->keystates[(unsigned)(Key_Lalt)] == KeyPressed || state->keystates[(unsigned)(Key_Ralt)] == KeyPressed) {
        mods |= KEY_MOD_ALT;
    }

    if (state->keystates[(unsigned)(Key_Lcmd)] == KeyPressed || state->keystates[(unsigned)(Key_Rcmd)] == KeyPressed) {
        mods |= KEY_MOD_CMD;
    }

    return mods;
}

char keyPressToASCII(KeyPress press) {
    bool shifted = press.modifiers & KEY_MOD_SHIFT;
    bool capitalize = (bool)(press.modifiers & KEY_MOD_CAPS) ^ shifted;
    bool numLock = press.modifiers & KEY_MOD_NUMLOCK;

    // for branchless letters
    char letter_offset = capitalize * CAP_DELTA;

    switch (press.code) {
    case Key_grave:
        if (shifted)
            return '~';
        
        return '`';
    case Key_1:
        if (shifted)
            return '!';

        return '1';
    case Key_2:
        if (shifted)
            return '@';
        
        return '2';
    case Key_3:
        if (shifted)
            return '#';

        return '3';
    case Key_4:
        if (shifted)
            return '$';
        
        return '4';
    case Key_5:
        if (shifted)
            return '%';

        return '5';
    case Key_6:
        if (shifted)
            return '^';

        return '6';
    case Key_7:
        if (shifted)
            return '&';

        return '7';
    case Key_8:
        if (shifted)
            return '*';

        return '8';
    case Key_9:
        if (shifted)
            return '(';

        return '9';
    case Key_0:
        if (shifted)
            return ')';

        return '0';
    case Key_minus:
        if (shifted)
            return '_';

        return '-';
    case Key_equal:
        if (shifted)
            return '+';
        
        return '=';
    case Key_backspace:
        return 8; // yes, backspace has an ascii char
    case Key_tab:
        return '\t';
    case Key_q: // using branchless since it's more compact
        return 'q' - letter_offset;
    case Key_w:
        return 'w' - letter_offset;
    case Key_e:
        return 'e' - letter_offset;
    case Key_r:
        return 'r' - letter_offset;
    case Key_t:
        return 't' - letter_offset;
    case Key_y:
        return 'y' - letter_offset;
    case Key_u:
        return 'u' - letter_offset;
    case Key_i:
        return 'i' - letter_offset;
    case Key_o:
        return 'o' - letter_offset;
    case Key_p:
        return 'p' - letter_offset;
    case Key_openSquare:
        if (shifted)
            return '{';

        return '[';
    case Key_closeSquare:
        if (shifted)
            return '}';

        return ']';
    case Key_backSlash:
        if (shifted)
            return '|';

        return '\\';
    case Key_a:
        return 'a' - letter_offset;
    case Key_s:
        return 's' - letter_offset;
    case Key_d:
        return 'd' - letter_offset;
    case Key_f:
        return 'f' - letter_offset;
    case Key_g:
        return 'g' - letter_offset;
    case Key_h:
        return 'h' - letter_offset;
    case Key_j:
        return 'j' - letter_offset;
    case Key_k:
        return 'k' - letter_offset;
    case Key_l:
        return 'l' - letter_offset;
    case Key_semicolon:
        if (shifted)
            return ':';

        return ';';
    case Key_apostrophe:
        if (shifted)
            return '"';

        return '\'';
    case Key_enter:
        return '\n';
    case Key_z:
        return 'z' - letter_offset;
    case Key_x:
        return 'x' - letter_offset;
    case Key_c:
        return 'c' - letter_offset;
    case Key_v:
        return 'v' - letter_offset;
    case Key_b:
        return 'b' - letter_offset;
    case Key_n:
        return 'n' - letter_offset;
    case Key_m:
        return 'm' - letter_offset;
    case Key_comma:
        if (shifted)
            return '<';

        return ',';
    case Key_period:
        if (shifted)
            return '>';

        return '.';
    case Key_slash:
        if (shifted)
            return '?';

        return '/';
    case Key_space:
        return ' ';
    case Key_esc:
        return 27; // did you know esacpe has an ascii code?
    
    case Key_numDiv:
        return '/';
    case Key_numMult:
        return '*';
    case Key_numAdd:
        return '+';
    case Key_numSub:
        return '-';
    case Key_numEnter:
        return '\n';
    case Key_numDecimal:
        return '.';

    case Key_num7:
        if (numLock)
            return '7';
        
        return '\0';
    case Key_num8:
        if (numLock)
            return '8';
        
        return '\0';
    case Key_num9:
        if (numLock)
            return '9';
        
        return '\0';
    case Key_num4:
        if (numLock)
            return '4';
        
        return '\0';
    case Key_num5:
        if (numLock)
            return '5';
        
        return '\0';
    case Key_num6:
        if (numLock)
            return '6';
        
        return '\0';
    case Key_num1:
        if (numLock)
            return '1';
        
        return '\0';
    case Key_num2:
        if (numLock)
            return '2';
        
        return '\0';
    case Key_num3:
        if (numLock)
            return '3';
        
        return '\0';
    case Key_num0:
        if (numLock)
            return '0';
        
        return '\0';
    default:
        return '\0';
    }
}


static const KeyPress nonePress = { 0, Key_none, KeyReleased, 0 };

KeyPress codePointDiscard(struct KeyboardState*, uint8_t) {
    return nonePress;
}

KeyPress codePointPS2SC1(struct KeyboardState* state, uint8_t code) {
    if (state == NULL) return nonePress;

    if (code == 0xE0) {
        state->extended = 1;
        return nonePress;
    }

    if (code == 0xE1) {
        state->extended = 3;
        return nonePress;
    }

    bool released = code & 0b10000000; // highest bit is set on release
    KeyPress out = {get_ticks(), Key_none, KeyReleased, 0 };
    if (!released)
        out.event = KeyPressed;

    bool numLock = state->keystates[(unsigned)(Key_numLock)] == KeyToggled;

    if (state->extended == 3) { // 1st extended byte of 0xE1
        state->extended = 2;
        return nonePress;
    }
    else if (state->extended == 2) { // 2nd extended byte of 0xE1
        state->extended = 0;
        return nonePress;
    }
    else if (state->extended == 1) { // 0xE0 multibyte
        switch (code & 0b01111111) {
        case 0x1C:
            out.code = Key_numEnter;
            break;
        case 0x1D:
            out.code = Key_Rctrl;
            break;
        case 0x35:
            out.code = Key_numDiv;
            break;
        case 0x38:
            out.code = Key_Ralt;
            break;
        case 0x47:
            out.code = Key_home;
            break;
        case 0x48:
            out.code = Key_up;
            break;
        case 0x49:
            out.code = Key_pgUp;
            break;
        case 0x4B:
            out.code = Key_left;
            break;
        case 0x4D:
            out.code = Key_right;
            break;
        case 0x4F:
            out.code = Key_end;
            break;
        case 0x50:
            out.code = Key_down;
            break;
        case 0x51:
            out.code = Key_pgDown;
            break;
        case 0x52:
            out.code = Key_insert;
            break;
        case 0x53:
            out.code = Key_delete;
            break;
        case 0x5B:
            out.code = Key_Lcmd;
            break;
        case 0x5C:
            out.code = Key_Rcmd;
            break;
        case 0x5D:
            out.code = Key_app;
            break;
        case 0x9C:
            out.code = Key_numEnter;
            break;
        case 0x9D:
            out.code = Key_Rctrl;
            break;
        case 0xB5:
            out.code = Key_numDiv;
            break;
        case 0xB8:
            out.code = Key_Ralt;
            break;
        default: // multimedia or non-existant
            break;
        }
        state->extended = 0;
    }
    else if (state->extended == 0) {
        switch (code & 0b01111111) {
        case 0x1:
            out.code = Key_esc;
            break;
        case 0x2:
            out.code = Key_1;
            break;
        case 0x3:
            out.code = Key_2;
            break;
        case 0x4:
            out.code = Key_3;
            break;
        case 0x5:
            out.code = Key_4;
            break;
        case 0x6:
            out.code = Key_5;
            break;
        case 0x7:
            out.code = Key_6;
            break;
        case 0x8:
            out.code = Key_7;
            break;
        case 0x9:
            out.code = Key_8;
            break;
        case 0xA:
            out.code = Key_9;
            break;
        case 0xB:
            out.code = Key_0;
            break;
        case 0xC:
            out.code = Key_minus;
            break;
        case 0xD:
            out.code = Key_equal;
            break;
        case 0xE:
            out.code = Key_backspace;
            break;
        case 0xF:
            out.code = Key_tab;
            break;
        case 0x10:
            out.code = Key_q;
            break;
        case 0x11:
            out.code = Key_w;
            break;
        case 0x12:
            out.code = Key_e;
            break;
        case 0x13:
            out.code = Key_r;
            break;
        case 0x14:
            out.code = Key_t;
            break;
        case 0x15:
            out.code = Key_y;
            break;
        case 0x16:
            out.code = Key_u;
            break;
        case 0x17:
            out.code = Key_i;
            break;
        case 0x18:
            out.code = Key_o;
            break;
        case 0x19:
            out.code = Key_p;
            break;
        case 0x1A:
            out.code = Key_openSquare;
            break;
        case 0x1B:
            out.code = Key_closeSquare;
            break;
        case 0x1C:
            out.code = Key_enter;
            break;
        case 0x1D:
            out.code = Key_Lctrl;
            break;
        case 0x1E:
            out.code = Key_a;
            break;
        case 0x1F:
            out.code = Key_s;
            break;
        case 0x20:
            out.code = Key_d;
            break;
        case 0x21:
            out.code = Key_f;
            break;
        case 0x22:
            out.code = Key_g;
            break;
        case 0x23:
            out.code = Key_h;
            break;
        case 0x24:
            out.code = Key_j;
            break;
        case 0x25:
            out.code = Key_k;
            break;
        case 0x26:
            out.code = Key_l;
            break;
        case 0x27:
            out.code = Key_semicolon;
            break;
        case 0x28:
            out.code = Key_apostrophe;
            break;
        case 0x29:
            out.code = Key_grave;
            break;
        case 0x2A:
            out.code = Key_LShift;
            break;
        case 0x2B:
            out.code = Key_backSlash;
            break;
        case 0x2C:
            out.code = Key_z;
            break;
        case 0x2D:
            out.code = Key_x;
            break;
        case 0x2E:
            out.code = Key_c;
            break;
        case 0x2F:
            out.code = Key_v;
            break;
        case 0x30:
            out.code = Key_b;
            break;
        case 0x31:
            out.code = Key_n;
            break;
        case 0x32:
            out.code = Key_m;
            break;
        case 0x33:
            out.code = Key_comma;
            break;
        case 0x34:
            out.code = Key_period;
            break;
        case 0x35:
            out.code = Key_slash;
            break;
        case 0x36:
            out.code = Key_RShift;
            break;
        case 0x37:
            out.code = Key_numMult;
            break;
        case 0x38:
            out.code = Key_Lalt;
            break;
        case 0x39:
            out.code = Key_space;
            break;
        case 0x3A:
            out.code = Key_capsLock;

            // toggle if not toggled, otherwise let it be released/pressed
            if (!released && state->keystates[(unsigned)(Key_capsLock)] != KeyToggled) {
                out.event = KeyToggled;
            }
            break;
        case 0x3B:
            out.code = Key_f1;
            break;
        case 0x3C:
            out.code = Key_f2;
            break;
        case 0x3D:
            out.code = Key_f3;
            break;
        case 0x3E:
            out.code = Key_f4;
            break;
        case 0x3F:
            out.code = Key_f5;
            break;
        case 0x40:
            out.code = Key_f6;
            break;
        case 0x41:
            out.code = Key_f7;
            break;
        case 0x42:
            out.code = Key_f8;
            break;
        case 0x43:
            out.code = Key_f9;
            break;
        case 0x44:
            out.code = Key_f10;
            break;
        case 0x45:
            out.code = Key_numLock;

            // toggle if not toggled, otherwise let it be released/pressed
            if (!released && state->keystates[(unsigned)(Key_numLock)] != KeyToggled) {
                out.event = KeyToggled;
            }
            break;
        case 0x46:
            out.code = Key_scrollLock;

            // toggle if not toggled, otherwise let it be released/pressed
            if (!released && state->keystates[(unsigned)(Key_scrollLock)] != KeyToggled) {
                out.event = KeyToggled;
            }
            break;
        case 0x47:
            if (numLock) {
                out.code = Key_num7;
            }
            else {
                out.code = Key_home;
            }
            break;
        case 0x48:
            if (numLock) {
                out.code = Key_num8;
            } else {
                out.code = Key_up;
            }
            break;
        case 0x49:
            if (numLock) {
                out.code = Key_num9;
            }
            else {
                out.code = Key_pgUp;
            }
            break;
        case 0x4A:
            out.code = Key_numSub;
            break;
        case 0x4B:
            if (numLock) {
                out.code = Key_num4;
            } else {
                out.code = Key_left;
            }
            break;
        case 0x4C:
            if (numLock) {
                out.code = Key_num5;
            }
            break;
        case 0x4D:
            if (numLock) {
                out.code = Key_num6;
            }
            else {
                out.code = Key_right;
            }
            break;
        case 0x4E:
            out.code = Key_numAdd;
            break;
        case 0x4F:
            if (numLock) {
                out.code = Key_num1;
            }
            else {
                out.code = Key_end;
            }
            break;
        case 0x50:
            if (numLock) {
                out.code = Key_num2;
            }
            else {
                out.code = Key_down;
            }
            break;
        case 0x51:
            if (numLock) {
                out.code = Key_num3;
            }
            else {
                out.code = Key_pgDown;
            }
            break;
        case 0x52:
            if (numLock) {
                out.code = Key_num0;
            }
            break;
        case 0x53:
            out.code = Key_numDecimal;
            break;
        case 0x57:
            out.code = Key_f11;
            break;
        case 0x58:
            out.code = Key_f12;
            break;
        default:
            break;
        }
    }
    else { // odd state
        state->extended = 0;
        return nonePress;
    }

    out.modifiers = getActiveModifiers(state);
    
    state->keystates[(unsigned)(out.code)] = out.event;

    if (out.event == KeyToggled) {
        if (released) {
            out.event = KeyReleased;
        }
        else {
            out.event = KeyPressed;
        }
    }

    return out;
}