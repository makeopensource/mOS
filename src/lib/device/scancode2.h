bool isExtended = false;
bool isReleased = false;

enum ScanCode2_Base {
    SC2_F9_DOWN = 0x01,
    SC2_F5_DOWN = 0x03,
    SC2_F3_DOWN,
    SC2_F1_DOWN,
    SC2_F2_DOWN,
    SC2_F12_DOWN,
    SC2_F10_DOWN = 0x09,
    SC2_F8_DOWN,
    SC2_F6_DOWN,
    SC2_F4_DOWN,
    SC2_TAB_DOWN,
    SC2_BACKTICK_DOWN,
    SC2_LALT_DOWN = 0x11,
    SC2_LSHIFT_DOWN,
    SC2_LCTL_DOWN = 0x14,
    SC2_Q_DOWN,
    SC2_1_DOWN,
    SC2_Z_DOWN = 0x1A,
    SC2_S_DOWN,
    SC2_A_DOWN,
    SC2_W_DOWN,
    SC2_2_DOWN,
    SC2_C_DOWN = 0x21,
    SC2_X_DOWN,
    SC2_D_DOWN,
    SC2_E_DOWN,
    SC2_4_DOWN,
    SC2_3_DOWN,
    SC2_SPC_DOWN = 0x29,
    SC2_V_DOWN,
    SC2_F_DOWN,
    SC2_T_DOWN,
    SC2_R_DOWN,
    SC2_5_DOWN,
    SC2_N_DOWN = 0x31,
    SC2_B_DOWN,
    SC2_H_DOWN,
    SC2_G_DOWN,
    SC2_Y_DOWN,
    SC2_6_DOWN,
    SC2_M_DOWN = 0x3A,
    SC2_J_DOWN,
    SC2_U_DOWN,
    SC2_7_DOWN,
    SC2_8_DOWN,
    SC2_COMMA_DOWN = 0x41,
    SC2_K_DOWN,
    SC2_I_DOWN,
    SC2_O_DOWN,
    SC2_0_DOWN,
    SC2_9_DOWN,
    SC2_PERIOD_DOWN = 0x49,
    SC2_FWDSLASH_DOWN,
    SC2_L_DOWN,
    SC2_SEMICOLON_DOWN,
    SC2_P_DOWN,
    SC2_DASH_DOWN,
    SC2_SINGLEQUOTE_DOWN = 0x52,
    SC2_LBRACKET_DOWN = 0x54,
    SC2_EQ_DOWN,
    SC2_CAPSLOCK_DOWN = 0x58,
    SC2_RSHIFT_DOWN,
    SC2_ENTER_DOWN,
    SC2_RBRACKET_DOWN,
    SC2_BACKSPACE_DOWN = 0x66,
    SC2_KPD1_DOWN = 0x69,
    SC2_KPD4_DOWN = 0x6B,
    SC2_KPD7_DOWN,
    SC2_KPD0_DOWN = 0x70,
    SC2_KPDPERIOD_DOWN,
    SC2_KPD2_DOWN,
    SC2_KPD5_DOWN,
    SC2_KPD6_DOWN,
    SC2_KPD8_DOWN,
    SC2_ESC_DOWN,
    SC2_NUMLOCK_DOWN,
    SC2_F11_DOWN,
    SC2_KPDPLUS_DOWN,
    SC2_KPD3_DOWN,
    SC2_KPDMINUS_DOWN,
    SC2_KPDMULT_DOWN,
    SC2_KPD9_DOWN,
    SC2_SCROLLLOCK_DOWN,
    SC2_F7_DOWN = 0x83,

    SC2_EXTENDED_DOWN = 0xE0,

    //run through Base again
    SC2_NEXT_RELEASE = 0xF0,
};

enum ScanCode2_Extended {
    SC2_WWW_DOWN = 0x10,
    SC2_RALT_DOWN,
    SC2_RCTL_DOWN = 0x14,
    SC2_PREVTRACK_DOWN,
    SC2_WWWFAV_DOWN = 0x18,
    SC2_LGUI_DOWN = 0x1F,
    SC2_WWWREFRESH_DOWN,
    SC2_VOLDEC_DOWN,
    SC2_MUTE_DOWN = 0x23,
    SC2_RGUI_DOWN = 0x27,
    SC2_WWWSTOP_DOWN,
    SC2_CALC_DOWN = 0x2B,
    SC2_APP_DOWN = 0x2F,
    SC2_WWWFWD_DOWN,
    SC2_VOLINC_DOWN = 0x32,
    SC2_PLAYPAUSE_DOWN = 0x34,
    SC2_POWER_DOWN = 0x37,
    SC2_WWWBACK_DOWN,
    SC2_WWWHOME_DOWN = 0x3A,
    SC2_STOP_DOWN,
    SC2_SLEEP_DOWN = 0x3F,
    SC2_MYCOMP_DOWN,
    SC2_EMAIL_DOWN = 0x48,
    SC2_KPDSLASH_DOWN = 0x4A,
    SC2_NEXTTRACK_DOWN = 0x4D,
    SC2_MEDIASELECT_DOWN = 0x50,
    SC2_KPDENTER_DOWN = 0x5A,
    SC2_WAKE_DOWN = 0x5E,
    SC2_END_DOWN = 0x69,
    SC2_CURSORL_DOWN = 0x6B,
    SC2_HOME_DOWN,
    SC2_INSERT_DOWN = 0x70,
    SC2_DELETE_DOWN,
    SC2_CURSORD_DOWN,
    SC2_CURSORR_DOWN = 0x74,
    SC2_CURSORU_DOWN,
    SC2_PGDOWN_DOWN = 0x7A,
    SC2_PGUP_DOWN = 0x7D,

    //if released, in reverse
    SC2_PRTSC_PART1_DOWN = 0x12,
    SC2_PRTSC_PART2_DOWN = 0x7C,

    //run through Extended again
    SC2_NEXT_RELEASE_E = 0xF0,
};

/*
    Scan Code 2 State Machine (errors not shown)
     - E0 = 0xE0 = SC2_EXTENDED_DOWN
     - F0 = 0xF0 = SC2_NEXT_RELEASED or SC2_NEXT_RELEASED_E
     - PRTSC_1 = 0x12 = SC2_PRTSC_PART1_DOWN
     - PRTSC_2 = 0x7C = SC2_PRTSC_PART2_DOWN
     - * is any valid code not specified
------------------------------------------------------------------------------------------------------
    *        E0          F0                        *                                                 |
  \->(Base) -> (Extended) -> (Extended [Released]) -> (goto Base)                                    |
   \  |         |                   |                                                                |
    \-|         \-> (goto Base)     \-> (Base) -> (Extended) -> (Extended [Released]) -> (goto Base) |
      |          |*                 PRTSC_2    E0           F0                      PRTSC_1          |
      |          |                                                                                   |
      |           \-> (Base) -> (Extended) -> (goto Base)                                            |
      |         PRTSC_1      E0          PRTSC_2                                                     |
      |                                                                                              |
      \---> (Base [Released]) -> (goto Base)                                                         |  
        F0                    *                                                                      |
------------------------------------------------------------------------------------------------------
*/