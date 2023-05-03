#ifndef KEYBOARD_H
#define KEYBOARD_H

// scancodes get translated into keycodes
enum KeyCodes {
    Key_none,

    // row 1
    Key_grave, // the ` key
    Key_1,
    Key_2,
    Key_3,
    Key_4,
    Key_5,
    Key_6,
    Key_7,
    Key_8,
    Key_9,
    Key_0,
    Key_minus,
    Key_equal,
    Key_backspace,

    // row 2
    Key_tab,
    Key_q,
    Key_w,
    Key_e,
    Key_r,
    Key_t,
    Key_y,
    Key_u,
    Key_i,
    Key_o,
    Key_p,
    Key_openSquare,
    Key_closeSquare,
    Key_backSlash,

    // row 3
    Key_capsLock,
    Key_a,
    Key_s,
    Key_d,
    Key_f,
    Key_g,
    Key_h,
    Key_j,
    Key_k,
    Key_l,
    Key_semicolon,
    Key_apostrophe,
    Key_enter,

    // row 4
    Key_LShift,
    Key_z,
    Key_x,
    Key_c,
    Key_v,
    Key_b,
    Key_n,
    Key_m,
    Key_comma,
    Key_period,
    Key_slash,
    Key_RShift,

    // row 5
    Key_Lctrl,
    Key_Lcmd, // left "Windows" key, referred to left GUI on osdev
    Key_Lalt,
    Key_space,
    Key_Ralt,
    Key_Rcmd,
    Key_app, // not present on many newer keyboards, replaced by function key
    Key_Rctrl,

    // row 0 (function keys)
    Key_esc,
    Key_f1,
    Key_f2,
    Key_f3,
    Key_f4,
    Key_f5,
    Key_f6,
    Key_f7,
    Key_f8,
    Key_f9,
    Key_f10,
    Key_f11,
    Key_f12,

    /* Extended Utility Keys */

    // row 0
    Key_printScreen,
    Key_scrollLock,
    Key_pause,

    // row 1
    Key_insert,
    Key_home,
    Key_pgUp,

    // row 3
    Key_delete,
    Key_end,
    Key_pgDown,

    // directional (DDR order)
    Key_left,
    Key_down,
    Key_up,
    Key_right,

    // numpad only
    Key_numLock,

    // NOTE: numpad keys do not get unique keycodes, also multimedia is NOT supported

    Key_Code_Count // very important that this is the last and there is no higher value
};



#endif