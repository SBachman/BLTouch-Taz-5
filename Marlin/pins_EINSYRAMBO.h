/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * Einsy-Rambo pin assignments
 */

#ifndef __AVR_ATmega2560__
  #error "Oops!  Make sure you have 'Arduino Mega 2560 or Rambo' selected from the 'Tools -> Boards' menu."
#endif

#define BOARD_NAME          "Einsy Rambo"
#define LARGE_FLASH         true

// Enable use of TMC2130
#define HAVE_TMC2130
#define X_IS_TMC2130
#define Y_IS_TMC2130
#define Z_IS_TMC2130
#define E0_IS_TMC2130

#if defined(LULZBOT_USE_EARLY_EINSY)
  #define BOARD_X_DIAG_PIN    40 // PG1 as defined in "fastio_1280.h"
  #define BOARD_Y_DIAG_PIN    69 // PK7 as defined in "fastio_1280.h"
  #define BOARD_Z_DIAG_PIN    68 // PK6 as defined in "fastio_1280.h"
  #define BOARD_E0_DIAG_PIN   65 // PK3 as defined in "fastio_1280.h"
#else
  #define BOARD_Z_THRES_PIN   57 // PF3 as defined in "fastio_1280.h"
  #define BOARD_X_DIAG_PIN    64 // PK2 as defined in "fastio_1280.h"
  #define BOARD_Y_DIAG_PIN    69 // PK7 as defined in "fastio_1280.h"
  #define BOARD_Z_DIAG_PIN    68 // PK6 as defined in "fastio_1280.h"
  #define BOARD_E0_DIAG_PIN   65 // PK3 as defined in "fastio_1280.h"
#endif
#define BOARD_X_MIN_PIN     12
#define BOARD_Y_MIN_PIN     11
#define BOARD_Z_MIN_PIN     10
#if defined(LULZBOT_USE_EARLY_EINSY)
  // EINY 0-.1 - EINSY 0.3
  #define BOARD_X_MAX_PIN     30
  #define BOARD_Y_MAX_PIN     24
  #define BOARD_Z_MAX_PIN     23
#endif
#if defined(LULZBOT_USE_EINSY_RETRO)
  // Einsy Rambo Retro 0.0b
  #define BOARD_X_MAX_PIN     81
  #define BOARD_Y_MAX_PIN     57
  #define BOARD_Z_MAX_PIN      7
#endif

//
// Limit Switches
//
// EINY 0-.1 - EINSY 0.3 have MAX switches
#if  (defined(LULZBOT_USE_EARLY_EINSY) || defined(LULZBOT_USE_EINSY_RETRO)) && defined(LULZBOT_SENSORLESS_HOMING)
  #define X_MIN_PIN          BOARD_X_DIAG_PIN
  #define Y_MIN_PIN          BOARD_Y_MIN_PIN
  #define X_MAX_PIN          BOARD_X_MAX_PIN
  #define Y_MAX_PIN          BOARD_Y_DIAG_PIN

  #if defined(LULZBOT_SENSORLESS_HOMING_Z)
    #define Z_MIN_PIN          BOARD_Z_DIAG_PIN
    #define Z_MAX_PIN          BOARD_Z_MAX_PIN
  #else
    #define Z_MIN_PIN          BOARD_Z_MIN_PIN
    #define Z_MAX_PIN          BOARD_Z_MAX_PIN
  #endif

#elif (defined(LULZBOT_USE_EARLY_EINSY) || defined(LULZBOT_USE_EINSY_RETRO))
  #define X_MIN_PIN          BOARD_X_MIN_PIN
  #define Y_MIN_PIN          BOARD_Y_MIN_PIN
  #define Z_MIN_PIN          BOARD_Z_MIN_PIN
  #define X_MAX_PIN          BOARD_X_MAX_PIN
  #define Y_MAX_PIN          BOARD_Y_MAX_PIN
  #define Z_MAX_PIN          BOARD_Z_MAX_PIN

// Einsy 0.4+ lacks MAX switches
#elif defined(LULZBOT_SENSORLESS_HOMING)
  #define X_MIN_PIN          BOARD_X_DIAG_PIN
  #define Y_MIN_PIN          -1
  #define Z_MIN_PIN          BOARD_Y_MIN_PIN
  #define X_MAX_PIN          -1
  #define Y_MAX_PIN          BOARD_Y_DIAG_PIN
  #define Z_MAX_PIN          BOARD_X_MIN_PIN

#else
  #define X_MIN_PIN          BOARD_X_MIN_PIN
  #define Y_MIN_PIN          BOARD_Y_MIN_PIN
  #define Z_MIN_PIN          BOARD_Z_MIN_PIN
  #define X_MAX_PIN          -1 // 30
  #define Y_MAX_PIN          -1 // 24
  #define Z_MAX_PIN          BOARD_Z_THRES_PIN
#endif

//
// Steppers
//
#define X_STEP_PIN         37
#define X_DIR_PIN          49
#define X_ENABLE_PIN       29

#define Y_STEP_PIN         36
#define Y_DIR_PIN          48
#define Y_ENABLE_PIN       28

#define Z_STEP_PIN         35
#define Z_DIR_PIN          47
#define Z_ENABLE_PIN       27

#define E0_STEP_PIN        34
#define E0_DIR_PIN         43
#define E0_ENABLE_PIN      26

#define E1_STEP_PIN        -1
#define E1_DIR_PIN         -1
#define E1_ENABLE_PIN      -1

// Microstepping pins - Mapping not from fastio.h (?)
#define X_MS1_PIN          -1
#define X_MS2_PIN          -1
#define X_CS_PIN           41
#define Y_MS1_PIN          -1
#define Y_MS2_PIN          -1
#define Y_CS_PIN           39
#define Z_MS1_PIN          -1
#define Z_MS2_PIN          -1
#define Z_CS_PIN           67
#define E0_MS1_PIN         -1
#define E0_MS2_PIN         -1
#define E0_CS_PIN          66

#define MOTOR_CURRENT_PWM_XY_PIN 46
#define MOTOR_CURRENT_PWM_Z_PIN  45
#define MOTOR_CURRENT_PWM_E_PIN  44
// Motor current PWM conversion, PWM value = MotorCurrentSetting * 255 / range
#ifndef MOTOR_CURRENT_PWM_RANGE
  #define MOTOR_CURRENT_PWM_RANGE 2000
#endif
#define DEFAULT_PWM_MOTOR_CURRENT  {1300, 1300, 1300}

//
// Temperature Sensors
//
#define TEMP_0_PIN          0   // Analog Input
//#define TEMP_1_PIN          1   // Analog Input
#define TEMP_BED_PIN        2   // Analog Input

//
// Heaters / Fans
//
#define HEATER_0_PIN        3
//#define HEATER_1_PIN        7
//#define HEATER_2_PIN        6
#define HEATER_BED_PIN      4

#define FAN_PIN             8
#define FAN1_PIN            6

//
// Misc. Functions
//
#if defined(LULZBOT_USE_EARLY_EINSY) || defined(LULZBOT_USE_EINSY_RETRO)
  #define SDSS             53 // EINY 0-.1, EINSY 0.3, EinsyRetro
#else
  #define SDSS             77 // EINSY 0.4
#endif
#define LED_PIN            13

//
// LCD / Controller
//
#if ENABLED(ULTRA_LCD) || defined(LULZBOT_USE_TOUCH_UI)

  #if not defined(LULZBOT_DISABLE_KILL_BUTTON)
    #define KILL_PIN         32
  #endif

  #if ENABLED(NEWPANEL) || defined(LULZBOT_USE_TOUCH_UI)

    // Beeper on AUX-4
    #define BEEPER_PIN     84

    #define LCD_PINS_RS    82
    #if defined(LULZBOT_USE_EARLY_EINSY) || defined(LULZBOT_USE_EINSY_RETRO)
      // EINY 0.1, EINSY-0.3, EinsyRetro
      #define LCD_PINS_ENABLE 18
      #define LCD_PINS_D4     19
    #else
      // EINSY 0.4+
      #define LCD_PINS_ENABLE 61
      #define LCD_PINS_D4     59
    #endif
    #define LCD_PINS_D5    70
    #define LCD_PINS_D6    85
    #define LCD_PINS_D7    71

    // buttons are directly attached using AUX-2
    #define BTN_EN1        14
    #define BTN_EN2        72
    #define BTN_ENC         9  // the click

    #if not defined(LULZBOT_USE_USB_STICK)
      #define SD_DETECT_PIN  15
    #endif

  #endif // NEWPANEL
#endif // ULTRA_LCD
