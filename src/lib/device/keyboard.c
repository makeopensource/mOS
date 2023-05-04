#include "keyboard.h"

enum KeyCode codePointDiscard(struct KeyboardState*, uint8_t) {
    return Key_none;
}

#define CAP_DELTA ('a' - 'A')

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