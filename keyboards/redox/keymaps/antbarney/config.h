/*
Copyright 2018 Mattia Dal Ben <matthewdibi@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

/* Use I2C or Serial, not both */
// #define USE_SERIAL
#define USE_I2C

/* Select hand configuration */
// #define MASTER_LEFT
// #define MASTER_RIGHT
#define EE_HANDS

#undef RGBLED_NUM /* ws2812 RGB LED */
#define RGBLIGHT_SPLIT
#define RGBLED_NUM 24
#define RGBLED_SPLIT { 12, 12 }
#define RGBLIGHT_ANIMATIONS
#define RGBLIGHT_LIMIT_VAL 215
#define RGBLIGHT_HUE_STEP 5
#define RGBLIGHT_SAT_STEP 5
#define RGBLIGHT_VAL_STEP 5
#define RGBLIGHT_SLEEP
