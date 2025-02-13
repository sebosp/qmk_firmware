#include QMK_KEYBOARD_H

enum ctrl_keycodes {
    U_T_AUTO = SAFE_RANGE, //USB Extra Port Toggle Auto Detect / Always Active
    U_T_AGCR,              //USB Toggle Automatic GCR control
    DBG_TOG,               //DEBUG Toggle On / Off
    DBG_MTRX,              //DEBUG Toggle Matrix Prints
    DBG_KBD,               //DEBUG Toggle Keyboard Prints
    DBG_MOU,               //DEBUG Toggle Mouse Prints
    MD_BOOT,               //Restart into bootloader after hold timeout
    DVP_ESC, // Grave escape ???
    DVP_AMPR,
    DVP_LBRACKET,
    DVP_LCBR,
    DVP_RCBR,
    DVP_LPRN,
    DVP_AT,
    DVP_EQUAL,
    DVP_ASTERISK,
    DVP_RPRN,
    DVP_PLUS,
    DVP_RBRACKET,
    DVP_EXLM,
    DVP_HASH,
    DVP_SCOLON,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_ESC,  KC_F1,      KC_F2,        KC_F3,    KC_F4,    KC_F5,    KC_F6,     KC_F7,        KC_F8,    KC_F9,    KC_F10,       KC_F11,   KC_F12,               KC_PSCR, KC_SCRL, TO(1), \
        DVP_ESC, DVP_AMPR,   DVP_LBRACKET, DVP_LCBR, DVP_RCBR, DVP_LPRN, DVP_EQUAL, DVP_ASTERISK, DVP_RPRN, DVP_PLUS, DVP_RBRACKET, DVP_EXLM, DVP_HASH, KC_BSPC,    KC_INS,  KC_HOME, KC_PGUP, \
        KC_TAB,  DVP_SCOLON, KC_COMM,      KC_DOT,   KC_P,     KC_Y,     KC_F,      KC_G,         KC_C,     KC_R,     KC_L,         KC_SLSH,  DVP_AT,   KC_BSLS,    KC_DEL,  KC_END,  KC_PGDN, \
        KC_CAPS, KC_A,       KC_O,         KC_E,     KC_U,     KC_I,     KC_D,      KC_H,         KC_T,     KC_N,     KC_S,         KC_MINS,  KC_ENT, \
        KC_LSFT, KC_QUOT,    KC_Q,         KC_J,     KC_K,     KC_X,     KC_B,      KC_M,         KC_W,     KC_V,     KC_Z,         KC_RSFT,                                 KC_UP, \
        KC_LCTL, KC_LGUI,    KC_LALT,                           KC_SPC,                           KC_RALT,  MO(1),    KC_APP,       KC_RCTL,                        KC_LEFT, KC_DOWN, KC_RGHT \
    ),
    [1] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,            KC_MUTE, _______, TO(2),
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,   KC_MPLY, KC_MSTP, KC_VOLU,
        _______, RGB_SPD, RGB_VAI, RGB_SPI, RGB_HUI, RGB_SAI, _______, U_T_AUTO,U_T_AGCR,_______, _______, _______, _______, _______,   KC_MPRV, KC_MNXT, KC_VOLD,
        _______, RGB_RMOD,RGB_VAD, RGB_MOD, RGB_HUD, RGB_SAD, _______, _______, _______, _______, _______, _______, _______,
        _______, RGB_TOG, _______, _______, _______, MD_BOOT, NK_TOGG, _______, _______, _______, _______, _______,                              _______,
        _______, _______, _______,                   _______,                            _______, _______, _______, _______,            _______, _______, _______
    ),
    [2] = LAYOUT(
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,             KC_PSCR, KC_SCRL, TO(0),
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,   KC_INS,  KC_HOME, KC_PGUP,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,   KC_DEL,  KC_END,  KC_PGDN,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,                              KC_UP,
        KC_LCTL, KC_LGUI, KC_LALT,                   KC_SPC,                             KC_RALT, MO(1),   KC_APP,  KC_RCTL,            KC_LEFT, KC_DOWN, KC_RGHT
    ),
};

#define MODS_SHIFT  (get_mods() & MOD_MASK_SHIFT)
#define MODS_CTRL   (get_mods() & MOD_MASK_CTRL)
#define MODS_ALT    (get_mods() & MOD_MASK_ALT)

bool left_shift_down = false;
bool right_shift_down = false;

// Dvorak programmer has numbers when shift keys are pressed and symbols when not pressed.
bool dvp_swap(const char *primary, const char *secondary, bool is_pressed) {
    if (is_pressed) {
        if (left_shift_down)
            unregister_code(KC_LSFT);
        if (right_shift_down)
            unregister_code(KC_RSFT);
        if (left_shift_down || right_shift_down) {
            SEND_STRING(secondary);
        } else {
            SEND_STRING(primary);
        }
        return false;
    }
    return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static uint32_t key_timer;

    switch (keycode) {
        case U_T_AUTO:
            if (record->event.pressed && MODS_SHIFT && MODS_CTRL) {
                TOGGLE_FLAG_AND_PRINT(usb_extra_manual, "USB extra port manual mode");
            }
            return false;
        case U_T_AGCR:
            if (record->event.pressed && MODS_SHIFT && MODS_CTRL) {
                TOGGLE_FLAG_AND_PRINT(usb_gcr_auto, "USB GCR auto mode");
            }
            return false;
        case DBG_TOG:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_enable, "Debug mode");
            }
            return false;
        case DBG_MTRX:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_matrix, "Debug matrix");
            }
            return false;
        case DBG_KBD:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_keyboard, "Debug keyboard");
            }
            return false;
        case DBG_MOU:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_mouse, "Debug mouse");
            }
            return false;
        case MD_BOOT:
            if (record->event.pressed) {
                key_timer = timer_read32();
            } else {
                if (timer_elapsed32(key_timer) >= 500) {
                    reset_keyboard();
                }
            }
            return false;
        case RGB_TOG:
            if (record->event.pressed) {
              switch (rgb_matrix_get_flags()) {
                case LED_FLAG_ALL: {
                    rgb_matrix_set_flags(LED_FLAG_KEYLIGHT | LED_FLAG_MODIFIER | LED_FLAG_INDICATOR);
                    rgb_matrix_set_color_all(0, 0, 0);
                  }
                  break;
                case (LED_FLAG_KEYLIGHT | LED_FLAG_MODIFIER | LED_FLAG_INDICATOR): {
                    rgb_matrix_set_flags(LED_FLAG_UNDERGLOW);
                    rgb_matrix_set_color_all(0, 0, 0);
                  }
                  break;
                case LED_FLAG_UNDERGLOW: {
                    rgb_matrix_set_flags(LED_FLAG_NONE);
                    rgb_matrix_disable_noeeprom();
                  }
                  break;
                default: {
                    rgb_matrix_set_flags(LED_FLAG_ALL);
                    rgb_matrix_enable_noeeprom();
                  }
                  break;
              }
            }
            return false;

        case KC_LSFT:
            left_shift_down = record->event.pressed;
            return true;
        case KC_RSFT:
            right_shift_down = record->event.pressed;
            return true;
        case DVP_ESC:
            return dvp_swap("$", "~", record->event.pressed);
        case DVP_AMPR:
            return dvp_swap("&", "%", record->event.pressed);
        case DVP_LBRACKET:
            return dvp_swap("[", "7", record->event.pressed);
        case DVP_LCBR:
            return dvp_swap("{", "5", record->event.pressed);
        case DVP_RCBR:
            return dvp_swap("}", "3", record->event.pressed);
        case DVP_LPRN:
            return dvp_swap("(", "1", record->event.pressed);
        case DVP_EQUAL:
            return dvp_swap("=", "9", record->event.pressed);
        case DVP_ASTERISK:
            return dvp_swap("*", "0", record->event.pressed);
        case DVP_RPRN:
            return dvp_swap(")", "2", record->event.pressed);
        case DVP_PLUS:
            return dvp_swap("+", "4", record->event.pressed);
        case DVP_RBRACKET:
            return dvp_swap("]", "6", record->event.pressed);
        case DVP_EXLM:
            return dvp_swap("!", "8", record->event.pressed);
        case DVP_HASH:
            return dvp_swap("#", "`", record->event.pressed);
        case DVP_SCOLON:
            return dvp_swap(";", ":", record->event.pressed);
        case KC_COMM:
            return dvp_swap(",", "<", record->event.pressed);
        case KC_DOT:
            return dvp_swap(".", ">", record->event.pressed);
        case KC_SLSH:
            return dvp_swap("/", "?", record->event.pressed);
        case DVP_AT:
            return dvp_swap("@", "^", record->event.pressed);
        case KC_MINS:
            return dvp_swap("-", "_", record->event.pressed);
        default:
            return true; //Process all other keycodes normally
    }
}
