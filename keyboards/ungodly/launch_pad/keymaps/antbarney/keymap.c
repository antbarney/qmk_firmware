/* Copyright 2020 Ungodly Design <hello@ungodly.design>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H
#include "analog.h"
#include "qmk_midi.h"

#ifndef OLED_ENABLE
#define OLED_ENABLE
#endif

/* Force Numlock on */
void matrix_init_user (void) {
  if (!host_keyboard_led_state().num_lock) {
      tap_code(KC_NUM_LOCK);
  }
}

/* Custom Layer Up/Down Keystrokes */
enum custom_keycodes {
    KC_LUP = SAFE_RANGE, //cycle layers in up direction
    KC_LDN //cycle layers in down direction
};
#define HIGHEST_LAYER 4 //replace X with your highest layer
static uint8_t current_layer = 0;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {

  case LT(0, KC_LUP):
    if (record->tap.count && record->event.pressed) {
        tap_code(KC_NUM_LOCK); // Intercept tap function
    } else if (record->event.pressed) {
      if (current_layer == HIGHEST_LAYER){
        current_layer=0;
      } else {
        current_layer++;
      }
      layer_move(current_layer);
    }
    return false;
  case KC_LUP:
    if(record->event.pressed) {
      if (current_layer == HIGHEST_LAYER){
        current_layer=0;
      } else {
        current_layer++;
      }
      layer_move(current_layer);
    }
    return false;
  case KC_LDN:
    if(record->event.pressed) {
      if (current_layer == 0){
        current_layer=HIGHEST_LAYER;
      } else {
        current_layer--;
      }
      layer_move(current_layer);
    }
    return false;
  default:
    return true;
  }
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  /* Keymap _BL: (Base Layer) Default Numpad Layer
  * ,-------------------.
  * |LAYR| /  | *  |BACK |
  * |----|----|----|-----|
  * | 7  | 8  | 9  |  -  |
  * |----|----|----|-----|
  * | 4  | 5  | 6  |  +  |
  * |----|----|----|-----|
  * | 1  | 2  | 3  | En  |
  * |----|----|----|-----|
  * | 0  | 0  | .  | En  |
  * `--------------------'
  */
  [0] = LAYOUT_ortho_5x4(
    LT(0, KC_LUP), KC_PSLS, KC_PAST, LT(2, KC_BSPC),
    KC_P7,  KC_P8, KC_P9,   KC_PMNS,
    KC_P4,  KC_P5, KC_P6,   KC_PPLS,
    KC_P1,  KC_P2, KC_P3,   KC_PENT,
    KC_P0,  KC_P0, KC_PDOT, KC_PENT
  ),

  /* Keymap _NV: Navigation layer
  * ,-------------------.
  * |LAYR|    |    |NUM |
  * |----|----|----|----|
  * |HOME| UP |PGUP|    |
  * |----|----|----|----|
  * |LEFT|    |RIGH|    |
  * |----|----|----|----|
  * |END |DOWN|PGDN| En |
  * |----|----|----|----|
  * |INS |INS |DEL | En |
  * `-------------------'
  */
  [1] = LAYOUT_ortho_5x4(
    KC_LUP,  KC_NUM_LOCK, DB_TOGG, XXXXXXX,
    KC_HOME, KC_UP,       KC_PGUP, XXXXXXX,
    KC_LEFT, XXXXXXX,     KC_RGHT, XXXXXXX,
    KC_END,  KC_DOWN,     KC_PGDN, KC_PENT,
    KC_INS,  KC_INS,      KC_DEL,  KC_PENT
  ),

  /* Keymap _FN: RGB Function Layer
  * ,-------------------.
  * |LAYR|    |    |RTOG|
  * |----|----|----|----|
  * |HUD |HUI |    |RGBP|
  * |----|----|----|----|
  * |SAD |SAI |    |RMOD|
  * |----|----|----|----|
  * |VAD |VAS |    |    |
  * |----|----|----|----|
  * |RST |RST |    |    |
  * `-------------------'
  */
  [2] = LAYOUT_ortho_5x4(
    KC_LUP,  XXXXXXX,  XXXXXXX, RGB_TOG,
    RGB_HUD, RGB_HUI,  XXXXXXX, RGB_M_P,
    RGB_SAD, RGB_SAI,  XXXXXXX, RGB_MOD,
    RGB_VAD, RGB_VAI,  XXXXXXX, XXXXXXX,
    QK_BOOT, QK_BOOT,  XXXXXXX, XXXXXXX
  ),

  [3] = LAYOUT_ortho_5x4(
    KC_LUP,  XXXXXXX,  XXXXXXX, RGB_TOG,
    RGB_HUD, RGB_HUI,  XXXXXXX, RGB_M_P,
    RGB_SAD, RGB_SAI,  XXXXXXX, RGB_MOD,
    QK_BOOT, QK_BOOT,  XXXXXXX, XXXXXXX,
    QK_BOOT, QK_BOOT,  XXXXXXX, XXXXXXX
  ),

  [4] = LAYOUT_ortho_5x4(
    KC_LUP,  XXXXXXX,  XXXXXXX, RGB_TOG,
    RGB_HUD, RGB_HUI,  XXXXXXX, RGB_M_P,
    RGB_SAD, RGB_SAI,  XXXXXXX, RGB_MOD,
    QK_BOOT, QK_BOOT,  XXXXXXX, XXXXXXX,
    QK_BOOT, QK_BOOT,  XXXXXXX, XXXXXXX
  )
};

// (Optional) Rotary Encoder, Volume Control
bool encoder_update_user(uint8_t index, bool clockwise) {
  if (clockwise) {
      tap_code(KC_VOLU);
  } else {
      tap_code(KC_VOLD);
  }
  return true;
}

// Potentiometer Slider, MIDI Control
uint8_t divisor = 0;
void slider(void){
  if (divisor++) { // only run the slider function 1/256 times it's called
      return;
  }
  midi_send_cc(&midi_device, 2, 0x3E, 0x7F - (analogReadPin(SLIDER_PIN) >> 3));
}

void matrix_scan_user(void) {
  slider();
}

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
	return OLED_ROTATION_270;
}

static void render_logo(void) {
  static const char PROGMEM my_logo[]={
    // 'Logo', 32x64px
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0xcc, 0xcc, 0x33,
    0x33, 0xcc, 0xcc, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x18, 0x18, 0x60, 0x60, 0xff,
    0xff, 0x60, 0x60, 0x18, 0x18, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x7f,
    0x7f, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x1e, 0x61, 0x61, 0x9e,
    0x9e, 0x61, 0x61, 0x1e, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };
  oled_write_raw_P(my_logo, sizeof(my_logo));
}

bool oled_task_user(void) {

  // Host Keyboard LED Status
  led_t led_state = host_keyboard_led_state();
  if (led_state.num_lock) {
    oled_write_ln_P(PSTR("NumLK"), true);
  }
  else {
    oled_write_ln_P(PSTR("NumLK"), false);
  }
  oled_write_ln_P(PSTR("Layer"), false);

  switch (get_highest_layer(layer_state)) {
    case 0:
      // render_rocket_logo();
      oled_write_P(PSTR("0"), true);
      oled_write_P(PSTR("1"), false);
      oled_write_P(PSTR("2"), false);
      oled_write_P(PSTR("3"), false);
      oled_write_ln_P(PSTR("4"), false);
      break;
    case 1:
      // render_nav_logo();
      oled_write_P(PSTR("0"), false);
      oled_write_P(PSTR("1"), true);
      oled_write_P(PSTR("2"), false);
      oled_write_P(PSTR("3"), false);
      oled_write_ln_P(PSTR("4"), false);
      break;
    case 2:
      // render_light_logo();
      oled_write_P(PSTR("0"), false);
      oled_write_P(PSTR("1"), false);
      oled_write_P(PSTR("2"), true);
      oled_write_P(PSTR("3"), false);
      oled_write_ln_P(PSTR("4"), false);
      break;
    case 3:
      // render_light_logo();
      oled_write_P(PSTR("0"), false);
      oled_write_P(PSTR("1"), false);
      oled_write_P(PSTR("2"), false);
      oled_write_P(PSTR("3"), true);
      oled_write_ln_P(PSTR("4"), false);
      break;
    case 4:
      // render_light_logo();
      oled_write_P(PSTR("0"), false);
      oled_write_P(PSTR("1"), false);
      oled_write_P(PSTR("2"), false);
      oled_write_P(PSTR("3"), false);
      oled_write_ln_P(PSTR("4"), true);
      break;
    default:
      // Or use the write_ln shortcut over adding '\n' to the end of your string
      oled_write_ln_P(PSTR("01234"), true);
      break;
    }

    render_logo();
    return false;
}
#endif
