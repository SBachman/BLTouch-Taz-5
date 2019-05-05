/**************************
 * Conditionals_Lulzbot.h *
 **************************/

/****************************************************************************
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   To view a copy of the GNU General Public License, go to the following  *
 *   location: <http://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#ifndef CONDITIONALS_LULZBOT
#define CONDITIONALS_LULZBOT

#include "src/Lulzbot_Extras.h"

/* We define LULZBOT_ macros based on which printer or toolhead we are
 * building for, these macros are then placed where appropriate in the
 * Marlin source.
 *
 * Using our own set of macros and limiting changes to Marlin's files
 * to one-liners makes it easier to merge from upstream. All macros are
 * prefixed with LULZBOT_ so that it is easy to see what was changed and
 * where. If a default setting is commented out in Marlin, we define
 * LULZBOT_{SETTING}_DISABLED here, so we have a record of things that
 * got disabled.
 *
 * Table of Contents:
 * ------------------
 *
 *   1. PRINTER MODEL CHARACTERISTICS
 *   2. GENERAL CONFIGURATION
 *   3. EXPERIMENTAL FEATURES
 *   4. CUSTOMIZED VERSION STRING AND URL
 *   5. MOTHERBOARD AND PIN CONFIGURATION
 *   6. HOMING & AXIS DIRECTIONS
 *   7. STARTUP ROUTINE
 *   8. STEPPER INACTIVITY TIMEOUT
 *   9. AUTOLEVELING / BED PROBE
 *  10. COMMON TOOLHEADS PARAMETERS
 *  11. MINI TOOLHEADS
 *  12. TAZ 4/5/6 TOOLHEADS
 *  13. UNIVERSAL TOOLHEADS
 *  14. TAZ PRO TOOLHEADS
 *  15. AUTO-CALIBRATION (BACKLASH AND NOZZLE OFFSET)
 *  16. TEMPERATURE SETTINGS
 *  17. HEATING ELEMENTS
 *  18. COOLING FAN CONFIGURATION
 *  19. AXIS TRAVEL LIMITS
 *  20. ENDSTOP CONFIGURATION
 *  21. FILAMENT CONFIGURATION (LIN_ADVANCE/RUNOUT)
 *  22. MOTOR DRIVER TYPE
 *  23. TRINAMIC DRIVER CONFIGURATION
 *  24. TRINAMIC SENSORLESS HOMING
 *  25. ADVANCED PAUSE / FILAMENT CHANGE
 *  26. WIPER PAD
 *  27. REWIPE FUNCTIONALITY
 *  28. PROBE QUALITY CHECK
 *  29. BACKLASH COMPENSATION
 *  30. MOTOR CURRENTS
 *  31. ACCELERATION, FEEDRATES AND XYZ MOTOR STEPS
 *  32. LCD OPTIONS
 *  33. CUSTOM SPLASH SCREEN
 *  34. Z-OFFSET AUTO-SAVE
 *
 */

#define LULZBOT_FW_VERSION ".77" // Change this with each update

#if ( \
    !defined(LULZBOT_Gladiola_Mini) && \
    !defined(LULZBOT_Gladiola_MiniLCD) && \
    !defined(LULZBOT_Hibiscus_Mini2) && \
    !defined(LULZBOT_Juniper_TAZ5) && \
    !defined(LULZBOT_Oliveoil_TAZ6) && \
    !defined(LULZBOT_Quiver_TAZPro) && \
    !defined(LULZBOT_Prototype_DemoCLCD) \
)
    #error Must specify printer model. Please see "Configuration_LulzBot.h" for directions.
#endif

#if ( \
    !defined(TOOLHEAD_Gladiola_SingleExtruder) && \
    !defined(TOOLHEAD_Albatross_Flexystruder) && \
    !defined(TOOLHEAD_Finch_Aerostruder) && \
    !defined(TOOLHEAD_CecropiaSilk_SingleExtruderAeroV2) && \
    !defined(TOOLHEAD_AchemonSphinx_SmallLayer) && \
    !defined(TOOLHEAD_BandedTiger_HardenedSteel) && \
    !defined(TOOLHEAD_DingyCutworm_HardenedSteelPlus) && \
    !defined(TOOLHEAD_Tilapia_SingleExtruder) && \
    !defined(TOOLHEAD_Kanyu_Flexystruder) && \
    !defined(TOOLHEAD_Opah_Moarstruder) && \
    !defined(TOOLHEAD_Javelin_DualExtruderV2) && \
    !defined(TOOLHEAD_Longfin_FlexyDually) && \
    !defined(TOOLHEAD_Yellowfin_DualExtruderV3) && \
    !defined(TOOLHEAD_Angelfish_Aerostruder) && \
    !defined(TOOLHEAD_Quiver_DualExtruder) \
)
    #error Must specify toolhead. Please see "Configuration_LulzBot.h" for directions.
#endif

/*********************** PRINTER MODEL CHARACTERISTICS **************************/

#if defined(LULZBOT_Gladiola_Mini)
    #define LULZBOT_CUSTOM_MACHINE_NAME "LulzBot Mini"
    #define LULZBOT_LCD_MACHINE_NAME "Mini"
    #define LULZBOT_IS_MINI
    #define LULZBOT_MINI_BED
    #define LULZBOT_USE_AUTOLEVELING
    #define LULZBOT_USE_MIN_ENDSTOPS
    #define LULZBOT_USE_MAX_ENDSTOPS
    #define LULZBOT_USE_NORMALLY_OPEN_ENDSTOPS
    #define LULZBOT_ENDSTOPS_ALWAYS_ON_DEFAULT
    #define LULZBOT_USE_Z_SCREW
    #define LULZBOT_BACKLASH_COMPENSATION
    #define LULZBOT_BAUDRATE 250000
    #define LULZBOT_MACHINE_UUID "351487b6-ca9a-4c1a-8765-d668b1da6585"
#endif

#if defined(LULZBOT_Gladiola_MiniLCD)
    #define LULZBOT_CUSTOM_MACHINE_NAME "LulzBot Mini LCD"
    #define LULZBOT_LCD_MACHINE_NAME "Mini"
    #define LULZBOT_IS_MINI
    #define LULZBOT_MINI_BED
    #define LULZBOT_USE_REPRAP_LCD_DISPLAY
    #define LULZBOT_USE_AUTOLEVELING
    #define LULZBOT_USE_MIN_ENDSTOPS
    #define LULZBOT_USE_MAX_ENDSTOPS
    #define LULZBOT_USE_NORMALLY_OPEN_ENDSTOPS
    #define LULZBOT_ENDSTOPS_ALWAYS_ON_DEFAULT
    #define LULZBOT_USE_Z_SCREW
    #define LULZBOT_BACKLASH_COMPENSATION
    #define LULZBOT_BAUDRATE 250000
    #define LULZBOT_MACHINE_UUID "083f68f1-028e-494c-98e1-f2e0dfaee9a5"
#endif

#if defined(LULZBOT_Juniper_TAZ5)
    #define LULZBOT_CUSTOM_MACHINE_NAME "LulzBot TAZ 5"
    #define LULZBOT_LCD_MACHINE_NAME "TAZ 5"
    #define LULZBOT_IS_TAZ
    #define LULZBOT_TAZ_BED
    #define LULZBOT_USE_REPRAP_LCD_DISPLAY
    #define LULZBOT_USE_MIN_ENDSTOPS
    #define LULZBOT_USE_NORMALLY_OPEN_ENDSTOPS
    #define LULZBOT_USE_Z_SCREW
    #define LULZBOT_BACKLASH_COMPENSATION
    #define LULZBOT_BAUDRATE 250000
    #define LULZBOT_MACHINE_UUID "c3255c96-4097-4884-8ed0-ded2ff9bae61"
#endif

#if defined(LULZBOT_Oliveoil_TAZ6)
    #define LULZBOT_CUSTOM_MACHINE_NAME "LulzBot TAZ 6"
    #define LULZBOT_LCD_MACHINE_NAME "TAZ 6"
    #define LULZBOT_IS_TAZ
    #define LULZBOT_TAZ_BED
    #define LULZBOT_USE_REPRAP_LCD_DISPLAY
    #define LULZBOT_USE_AUTOLEVELING
    #define LULZBOT_USE_MIN_ENDSTOPS
    #define LULZBOT_USE_MAX_ENDSTOPS
    #define LULZBOT_USE_HOME_BUTTON
    #define LULZBOT_USE_NORMALLY_CLOSED_ENDSTOPS
    #define LULZBOT_ENDSTOPS_ALWAYS_ON_DEFAULT
    #define LULZBOT_USE_Z_SCREW
    #define LULZBOT_BACKLASH_COMPENSATION
    #define LULZBOT_BAUDRATE 250000
    #define LULZBOT_MACHINE_UUID "845f003c-aebd-4e53-a6b9-7d0984fde609"
#endif

#if defined(LULZBOT_Hibiscus_Mini2)
    #define LULZBOT_CUSTOM_MACHINE_NAME "LulzBot Mini 2"
    #define LULZBOT_LCD_MACHINE_NAME "Mini 2"
    #define LULZBOT_IS_MINI
    #define LULZBOT_MINI_BED
    #define LULZBOT_USE_EINSY_RETRO
    #define LULZBOT_USE_REPRAP_LCD_DISPLAY
    #define LULZBOT_TWO_PIECE_BED
    #define LULZBOT_USE_AUTOLEVELING
    #define LULZBOT_SENSORLESS_HOMING
    #define LULZBOT_USE_NORMALLY_CLOSED_ENDSTOPS
    #define LULZBOT_STEALTHCHOP_Z
    #define LULZBOT_HYBRID_THRESHOLD
    #define LULZBOT_USE_Z_BELT
    #define LULZBOT_BACKLASH_COMPENSATION
    #define LULZBOT_BAUDRATE 250000
    #define LULZBOT_PRINTCOUNTER
    #define LULZBOT_MACHINE_UUID "e5502411-d46d-421d-ba3a-a20126d7930f"
    #define LULZBOT_LIGHTWEIGHT_UI
    #define LULZBOT_USE_EXPERIMENTAL_FEATURES
#endif

#if defined(LULZBOT_Quiver_TAZPro)
    #define LULZBOT_CUSTOM_MACHINE_NAME "LulzBot Quiver"
    #define LULZBOT_LCD_MACHINE_NAME "TAZ Pro"
    #define LULZBOT_IS_TAZ
    #define LULZBOT_TAZ_BED
    #define LULZBOT_TWO_PIECE_BED
    #define LULZBOT_USE_ARCHIM2
    #define LULZBOT_USE_TOUCH_UI
    #define LULZBOT_USE_AUTOLEVELING
    #define LULZBOT_SENSORLESS_HOMING
    #define LULZBOT_USE_NORMALLY_CLOSED_ENDSTOPS
    #define LULZBOT_USE_Z_BELT
    #define LULZBOT_BACKLASH_COMPENSATION
    #define LULZBOT_STEALTHCHOP_Z
    #define LULZBOT_HYBRID_THRESHOLD
    #define LULZBOT_BAUDRATE 250000
    #define LULZBOT_PRINTCOUNTER
    #define LULZBOT_MACHINE_UUID "a952577d-8722-483a-999d-acdc9e772b7b"
    #define LULZBOT_USE_EXPERIMENTAL_FEATURES
    #define LULZBOT_USB_FLASH_DRIVE_SUPPORT
    #define LULZBOT_SDSUPPORT
    #define LULZBOT_SDSUPPORT_DEBUG
    #define LULZBOT_HAS_CALIBRATION_CUBE
    #define LULZBOT_FILAMENT_RUNOUT_SENSOR
    #define LULZBOT_BACKUP_EEPROM_INFORMATION
    #define LULZBOT_USB_READ_ERROR_IS_FATAL
#endif

#if defined(LULZBOT_Prototype_DemoCLCD)
    #define LULZBOT_CUSTOM_MACHINE_NAME "LulzBot TAZ"
    #define LULZBOT_LCD_MACHINE_NAME "TAZ"
    #define LULZBOT_IS_MINI
    #define LULZBOT_USE_EINSY_RETRO
    #define LULZBOT_MINI_BED
    #define LULZBOT_USE_TOUCH_UI
    #define LULZBOT_USE_HIGH_RES
    //#define LULZBOT_USE_PORTRAIT_UI
    #define LULZBOT_TWO_PIECE_BED
    #define LULZBOT_USE_AUTOLEVELING
    #define LULZBOT_SENSORLESS_HOMING
    #define LULZBOT_USE_NORMALLY_CLOSED_ENDSTOPS
    #define LULZBOT_STEALTHCHOP_Z
    #define LULZBOT_HYBRID_THRESHOLD
    #define LULZBOT_USE_Z_BELT
    #define LULZBOT_BAUDRATE 250000
    #define LULZBOT_PRINTCOUNTER
    #define LULZBOT_BACKLASH_COMPENSATION
    #define LULZBOT_MACHINE_UUID "e5502411-d46d-421d-ba3a-a20126d7930f"
    //#define LULZBOT_USE_EXPERIMENTAL_FEATURES
    #define LULZBOT_SDSUPPORT
#endif

/**************************** GENERAL CONFIGURATION *****************************/

#define LULZBOT_STRING_CONFIG_H_AUTHOR "(Aleph Objects Inc., LulzBot Git Repository)"
#define LULZBOT_EEPROM_SETTINGS
#define LULZBOT_EMERGENCY_PARSER
#define LULZBOT_NOZZLE_PARK_FEATURE
#define LULZBOT_PAUSE_PARK_NOZZLE_TIMEOUT 300
#define LULZBOT_ADVANCED_OK
#define LULZBOT_TX_BUFFER_SIZE 32
#define LULZBOT_BUFSIZE 5
#define LULZBOT_HOST_KEEPALIVE_FEATURE_DISABLED
#define LULZBOT_PRINTJOB_TIMER_AUTOSTART_DISABLED

// Marlin 1.1.4 has changed the behavior of G92 so that
// it changes software endstops, making it less useful
// for making position adjustments after hitting an
// endstop. We need the old behavior of G92 for the
// Yellowfin start GCODE and it is also a useful
// feature for custom height adjustments (something
// requested in the forums).
//
// Either LULZBOT_G92_BACKWARDS_COMPATIBILITY
// or LULZBOT_NO_WORKSPACE_OFFSETS will restore the
// old behavior, but LULZBOT_NO_WORKSPACE_OFFSETS has
// other problems.

#define LULZBOT_G92_BACKWARDS_COMPATIBILITY
//#define LULZBOT_NO_WORKSPACE_OFFSETS

// Q&A wants to be able to use M226 on endstops switches
#define LULZBOT_NO_PIN_PROTECTION_ON_M226

// Often Marlin shows the "Home XYZ first" message, but
// never clears that message. The following causes
// Marlin to print a new message when the axis are homed
#define LULZBOT_HOMING_MESSAGE_WORKAROUND

/************************* EXPERIMENTAL FEATURES ******************************/

#if defined(LULZBOT_USE_EXPERIMENTAL_FEATURES)
    //#define LULZBOT_USE_STATUS_LED

    //#define LULZBOT_JUNCTION_DEVIATION
    //#define LULZBOT_S_CURVE_ACCELERATION

    #define LULZBOT_GCODE_MACROS
    #define LULZBOT_ADAPTIVE_FAN_SLOWING
#endif

#if defined(LULZBOT_USE_STATUS_LED)
    #define LULZBOT_NEOPIXEL_RGBW_LED
    #define LULZBOT_NEOPIXEL_PIN        32 // X_MAX_PIN
    #define LULZBOT_NEOPIXEL_PIXELS     8
    #undef  LULZBOT_USE_XMAX_PLUG
#endif

/*********************** CUSTOMIZED VERSION STRING AND URL **********************/

// The Makefile and build-lulzbot-firmware.sh has an option to generate
// firmware without any identifying version or build timestamp. This is
// used in internal testing to allow us to binary diff across .hex files.
#if defined(LULZBOT_MASK_VERSION)
    #undef  LULZBOT_FW_VERSION
    #define LULZBOT_FW_VERSION ".xx"
#endif

#define LULZBOT_DETAILED_BUILD_VERSION " FIRMWARE_VERSION:" SHORT_BUILD_VERSION LULZBOT_FW_VERSION " EXTRUDER_TYPE:" LULZBOT_M115_EXTRUDER_TYPE
#define LULZBOT_STRING_DISTRIBUTION_DATE __DATE__ __TIME__
#define LULZBOT_SOURCE_CODE_URL "https://code.alephobjects.com/diffusion/MARLIN"

/******************** MOTHERBOARD AND PIN CONFIGURATION ***********************/

#if defined(LULZBOT_USE_ARCHIM2)
    // Experimental TAZ retrofitted with Archim from UltiMachine
    #define LULZBOT_MOTHERBOARD                   BOARD_ARCHIM2
    #define LULZBOT_CONTROLLER_FAN_PIN            FAN1_PIN
    #define LULZBOT_SERIAL_PORT                   -1
    #define LULZBOT_SPI_SPEED                     SPI_SIXTEENTH_SPEED
    #define LULZBOT_M226_PINS_WORKAROUND

#elif defined(LULZBOT_USE_EINSY_RETRO)
    // Experimental Mini retrofitted with EinsyRambo from UltiMachine
    #define LULZBOT_MOTHERBOARD                   BOARD_EINSY_RETRO
    #define LULZBOT_CONTROLLER_FAN_PIN            FAN1_PIN  // Digital pin 6
    #define LULZBOT_SERIAL_PORT                   0
    #define LULZBOT_SPI_SPEED                     SPI_FULL_SPEED

#elif defined(LULZBOT_IS_MINI)
    #define LULZBOT_MOTHERBOARD                   BOARD_MINIRAMBO
    #define LULZBOT_CONTROLLER_FAN_PIN            FAN1_PIN  // Digital pin 6
    #define LULZBOT_SERIAL_PORT                   0
    #define LULZBOT_SPI_SPEED                     SPI_FULL_SPEED

#elif defined(LULZBOT_IS_TAZ)
    #define LULZBOT_MOTHERBOARD                   BOARD_RAMBO
    #define LULZBOT_CONTROLLER_FAN_PIN            FAN2_PIN  // Digital pin 2
    #define LULZBOT_Z_MIN_PROBE_PIN               SERVO0_PIN // Digital pin 22
    #define LULZBOT_SERIAL_PORT                   0
    #define LULZBOT_SPI_SPEED                     SPI_FULL_SPEED
#endif

/******************************* STARTUP ROUTINE ********************************/

#define LULZBOT_STARTUP lulzbot_startup(); // Defined in Lulzbot_Extras.cpp

/*********************** HOMING & AXIS DIRECTIONS ******************************/

#define LULZBOT_INVERT_X_DIR                      false
#define LULZBOT_INVERT_Y_DIR                      true
#define LULZBOT_INVERT_Z_DIR                      false
#define LULZBOT_INVERT_E0_DIR                     true
#define LULZBOT_INVERT_E1_DIR                     true

#if defined(LULZBOT_IS_MINI)
    #define LULZBOT_X_HOME_DIR             -1 // Home left
    #define LULZBOT_Y_HOME_DIR              1 // Home bed forward
    #define LULZBOT_Z_HOME_DIR              1 // Home to top
    #define LULZBOT_QUICK_HOME

#elif defined(LULZBOT_Juniper_TAZ5)
    #define LULZBOT_X_HOME_DIR             -1 // Home left
    #define LULZBOT_Y_HOME_DIR             -1 // Home bed rear
    #define LULZBOT_Z_HOME_DIR             -1 // Home towards bed
    #define LULZBOT_QUICK_HOME

#elif defined(LULZBOT_IS_TAZ) && !defined(LULZBOT_USE_HOME_BUTTON)
    #define LULZBOT_X_HOME_DIR             -1 // Home left
    #define LULZBOT_Y_HOME_DIR              1 // Home bed forward
    #define LULZBOT_Z_HOME_DIR              1 // Home to top
    #define LULZBOT_QUICK_HOME

#elif defined(LULZBOT_IS_TAZ) &&  defined(LULZBOT_USE_HOME_BUTTON)
    #define LULZBOT_X_HOME_DIR             -1 // Home left
    #define LULZBOT_Y_HOME_DIR              1 // Home bed forward
    #define LULZBOT_Z_HOME_DIR             -1 // Home towards bed
    #define LULZBOT_QUICK_HOME
#endif

#if defined(LULZBOT_MINI_BED) && defined(LULZBOT_USE_Z_BELT)
    #define LULZBOT_HOMING_FEEDRATE_XY             (50*60) // mm/m
    #define LULZBOT_HOMING_FEEDRATE_Z              (40*60) // mm/m

#elif defined(LULZBOT_MINI_BED) && defined(LULZBOT_USE_Z_SCREW)
    #define LULZBOT_HOMING_FEEDRATE_XY             (30*60) // mm/m
    #define LULZBOT_HOMING_FEEDRATE_Z               (8*60) // mm/m

#elif defined(LULZBOT_TAZ_BED) && defined(LULZBOT_USE_Z_BELT)
    #define LULZBOT_HOMING_FEEDRATE_XY             (50*60) // mm/m
    #define LULZBOT_HOMING_FEEDRATE_Z              (30*60) // mm/m

#elif defined(LULZBOT_TAZ_BED) && defined(LULZBOT_USE_Z_SCREW)
    #define LULZBOT_HOMING_FEEDRATE_XY             (50*60) // mm/m
    #define LULZBOT_HOMING_FEEDRATE_Z               (3*60)  // mm/m
#endif // LULZBOT_TAZ_BED

#if defined(LULZBOT_USE_HOME_BUTTON)
    #define LULZBOT_Z_MIN_PROBE_ENDSTOP
    // On the TAZ 6, the bed washers are on Z_MIN_PROBE while the
    // Z-Home button is on Z_MIN, yet we need both to be disabled
    // when z_probe_enabled is false. We added this special case
    // to "endstops.cpp"
    #define LULZBOT_Z_MIN_USES_Z_PROBE_ENABLED

#else
    // The Mini and TAZ Pro lack a home button and probe using the Z_MIN pin.
    #define LULZBOT_Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN
#endif

// Only the TAZ 6 has a Z-homing button
#if defined(LULZBOT_USE_HOME_BUTTON)
    #define LULZBOT_Z_SAFE_HOMING
    #define LULZBOT_Z_SAFE_HOMING_X_POINT         (-19)
    #define LULZBOT_Z_SAFE_HOMING_Y_POINT         (258)
    #define LULZBOT_Z_HOMING_HEIGHT               5

    #define LULZBOT_AFTER_Z_HOME_Z_RAISE         10
    #define LULZBOT_AFTER_Z_HOME_Z_ORIGIN         0

    #define LULZBOT_HOMING_USES_PROBE_PINS
#elif defined(LULZBOT_IS_TAZ) && !defined(LULZBOT_USE_HOME_BUTTON)
    // TAZ 5 safe homing position so fan duct does not hit.
    #define LULZBOT_Z_SAFE_HOMING
    #define LULZBOT_Z_SAFE_HOMING_X_POINT         10
    #define LULZBOT_Z_SAFE_HOMING_Y_POINT         10
    #define LULZBOT_Z_HOMING_HEIGHT               5
#else
    // On the Mini, raise nozzle to clear wiper pad before homing
    #define LULZBOT_Z_HOMING_HEIGHT               4
#endif  // LULZBOT_USE_HOME_BUTTON

#if defined(LULZBOT_USE_HOME_BUTTON) || defined(LULZBOT_SENSORLESS_HOMING) || defined(LULZBOT_ENDSTOPS_ALWAYS_ON_DEFAULT)
    /* Leaving the toolhead resting on the endstops with sensorless homing
     * will likely cause chatter if the machine is immediately re-homed, so
     * don't leave the head sitting on the endstops after homing. */
    #define LULZBOT_BACKOFF_FEEDRATE 5
    #define LULZBOT_BACKOFF_DIST_XY  5
    #define LULZBOT_BACKOFF_DIST_Z   2

    #if defined(LULZBOT_USE_HOME_BUTTON)
        /* On a TAZ, we need to raise the print head after homing to clear the button */
        #undef  LULZBOT_BACKOFF_DIST_Z
        #define LULZBOT_BACKOFF_DIST_Z LULZBOT_AFTER_Z_HOME_Z_RAISE
        #define LULZBOT_BACKOFF_X_POS  LULZBOT_Z_SAFE_HOMING_X_POINT
        #define LULZBOT_BACKOFF_Y_POS  LULZBOT_Z_SAFE_HOMING_Y_POINT
        /* On yellowfin we need to reset the origin to account for the Z home riser. */
        #define LULZBOT_BACKOFF_Z_POS_ADJUSTMENT \
            if((home_all || homeZ)) { \
                planner.synchronize(); \
                current_position[Z_AXIS] = LULZBOT_AFTER_Z_HOME_Z_ORIGIN; \
                sync_plan_position(); \
            }
    #else
        #define LULZBOT_BACKOFF_Z_POS_ADJUSTMENT
        #define LULZBOT_BACKOFF_X_POS (LULZBOT_X_HOME_DIR < 0 ? LULZBOT_BACKOFF_DIST_XY : LULZBOT_X_MAX_POS - LULZBOT_BACKOFF_DIST_XY)
        #define LULZBOT_BACKOFF_Y_POS (LULZBOT_Y_HOME_DIR < 0 ? LULZBOT_BACKOFF_DIST_XY : LULZBOT_Y_MAX_POS - LULZBOT_BACKOFF_DIST_XY)
    #endif
    #define     LULZBOT_BACKOFF_Z_POS (LULZBOT_Z_HOME_DIR < 0 ? LULZBOT_BACKOFF_DIST_Z  : LULZBOT_Z_MAX_POS - LULZBOT_BACKOFF_DIST_Z)

    #define LULZBOT_BACKOFF_AFTER_HOME \
        { \
            LULZBOT_BACKOFF_Z_POS_ADJUSTMENT \
            constexpr int x = LULZBOT_BACKOFF_X_POS; \
            constexpr int y = LULZBOT_BACKOFF_Y_POS; \
            constexpr int z = LULZBOT_BACKOFF_Z_POS; \
            const bool saved_endstop_state = Endstops::are_endstops_enabled_globally(); \
            Endstops::enable_globally(false); \
            do_blocking_move_to_z  ((home_all || homeZ) ? z : current_position[Z_AXIS], LULZBOT_BACKOFF_FEEDRATE); \
            do_blocking_move_to_xy ((home_all || homeX) ? x : current_position[X_AXIS], \
                                    (home_all || homeY) ? y : current_position[Y_AXIS], LULZBOT_BACKOFF_FEEDRATE); \
            Endstops::enable_globally(saved_endstop_state); \
        }
#else
    #define LULZBOT_BACKOFF_AFTER_HOME
#endif

// Enable NO_MOTION_BEFORE_HOMING on newer printers that have no MAX endstops,
// but leave TAZ5 as is so we don't introduce a change for those users.
#if !defined(LULZBOT_USE_MAX_ENDSTOPS) && !defined(LULZBOT_Juniper_TAZ5)
    #define LULZBOT_NO_MOTION_BEFORE_HOMING
#endif

/************************ STEPPER INACTIVITY TIMEOUT ****************************/

#define LULZBOT_DEFAULT_STEPPER_DEACTIVE_TIME 600

#if !defined(LULZBOT_USE_MAX_ENDSTOPS)
    #define LULZBOT_HOME_AFTER_DEACTIVATE
#endif

#if defined(LULZBOT_USE_Z_BELT)
    #define LULZBOT_DISABLE_INACTIVE_Z false
    #define LULZBOT_SD_FINISHED_STEPPERRELEASE false
    #define LULZBOT_SD_FINISHED_RELEASECOMMAND "M84 X Y E"
#else
    #define LULZBOT_DISABLE_INACTIVE_Z true
    #define LULZBOT_SD_FINISHED_STEPPERRELEASE true
    #define LULZBOT_SD_FINISHED_RELEASECOMMAND "M84 X Y Z E"
#endif

/*********************** AUTOLEVELING / BED PROBE *******************************/

#if defined(LULZBOT_USE_AUTOLEVELING) && defined(LULZBOT_MINI_BED) && defined(LULZBOT_USE_Z_BELT)
    #define LULZBOT_STANDARD_LEFT_PROBE_BED_POSITION        -3
    #define LULZBOT_STANDARD_RIGHT_PROBE_BED_POSITION      163
    #define LULZBOT_STANDARD_BACK_PROBE_BED_POSITION       168
    #define LULZBOT_STANDARD_FRONT_PROBE_BED_POSITION       -4

#elif defined(LULZBOT_USE_AUTOLEVELING) && defined(LULZBOT_MINI_BED)
    // In order to work with the Gladiola printers, we need to
    // perform the probe right against the left and front endstops.
    // This is extremely problematic and leads to other problems
    // which are corrected for in the start GCODE.
    #define LULZBOT_STANDARD_LEFT_PROBE_BED_POSITION      LULZBOT_X_MIN_POS
    #define LULZBOT_STANDARD_FRONT_PROBE_BED_POSITION     LULZBOT_Y_MIN_POS

    // The Gladiola has the probe points spaced further apart than
    // earlier models. Since Gladiola FW has to work on earlier
    // printers, we need to add a workaround because G29 hits the
    // endstops and shifts the coordinate system around.
    #define LULZBOT_STANDARD_RIGHT_PROBE_BED_POSITION     161
    #define LULZBOT_STANDARD_BACK_PROBE_BED_POSITION      161
    #define LULZBOT_USE_PRE_GLADIOLA_G29_WORKAROUND

#elif defined(LULZBOT_USE_AUTOLEVELING) && defined(LULZBOT_TAZ_BED)
    #define LULZBOT_STANDARD_LEFT_PROBE_BED_POSITION       -9
    #define LULZBOT_STANDARD_RIGHT_PROBE_BED_POSITION     288
    #define LULZBOT_STANDARD_BACK_PROBE_BED_POSITION      289
    #define LULZBOT_STANDARD_FRONT_PROBE_BED_POSITION      -9
#endif

#if defined(LULZBOT_USE_AUTOLEVELING)
    #define LULZBOT_RESTORE_LEVELING_AFTER_G28
    #define LULZBOT_NOZZLE_CLEAN_FEATURE
    #define LULZBOT_AUTO_BED_LEVELING_LINEAR
#endif

#if defined(LULZBOT_AUTO_BED_LEVELING_LINEAR)
  // Traditionally LulzBot printers have employed a four-point leveling
  // using a degenerate 2x2 grid. This is the traditional behavior.
  #define LULZBOT_GRID_MAX_POINTS_X            2
  #define LULZBOT_GRID_MAX_POINTS_Y            2
  #if defined(LULZBOT_IS_MINI)
    // We can't control the order of probe points exactly, but
    // this makes the probe start closer to the wiper pad.
    #define LULZBOT_PROBE_Y_FIRST
  #else
    // Restore the old probe sequence on the TAZ that starts
    // probing on the washer underneath the wiper pad.
    #define LULZBOT_LAST_PROBE_POINT_ON_BACK_LEFT_CORNER
  #endif
#endif

/* Make sure Marlin allows probe points outside of the bed area */
#if defined(LULZBOT_USE_AUTOLEVELING)
  #if LULZBOT_Z_SAFE_HOMING_X_POINT < LULZBOT_LEFT_PROBE_BED_POSITION
    #define MIN_PROBE_X LULZBOT_Z_SAFE_HOMING_X_POINT
  #else
    #define MIN_PROBE_X LULZBOT_LEFT_PROBE_BED_POSITION
  #endif
  #define MAX_PROBE_X LULZBOT_RIGHT_PROBE_BED_POSITION

  #define MIN_PROBE_Y LULZBOT_FRONT_PROBE_BED_POSITION
  #define MAX_PROBE_Y LULZBOT_BACK_PROBE_BED_POSITION
#endif

/* Auto-leveling was introduced on the Mini and TAZ 6.
 * Define probe parameters related to bed leveling,
 * e.g. the washers on the bed. These are confusingly
 * named Z_MIN_PROBE in Marlin. The Z-Home switch
 * is called Z_MIN_ENDSTOP
 */
#if defined(LULZBOT_USE_AUTOLEVELING)
    #define LULZBOT_FIX_MOUNTED_PROBE
#endif // LULZBOT_USE_AUTOLEVELING

#define LULZBOT_MULTIPLE_PROBING              2
#define LULZBOT_X_PROBE_OFFSET_FROM_EXTRUDER  0
#define LULZBOT_Y_PROBE_OFFSET_FROM_EXTRUDER  0
#define LULZBOT_Z_PROBE_OFFSET_RANGE_MIN      -4
#define LULZBOT_Z_PROBE_OFFSET_RANGE_MAX      5
#define LULZBOT_XY_PROBE_SPEED                6000
#define LULZBOT_Z_PROBE_SPEED_SLOW           (1*60)
#if defined(LULZBOT_USE_Z_BELT)
    #define LULZBOT_Z_PROBE_SPEED_FAST       (20*60)
#else
    #define LULZBOT_Z_PROBE_SPEED_FAST       (8*60)
#endif
#define LULZBOT_Z_CLEARANCE_DEPLOY_PROBE      5
#define LULZBOT_Z_CLEARANCE_BETWEEN_PROBES    5
#define LULZBOT_MIN_PROBE_EDGE_DISABLED

/* We need to disable the extruder motor during probing as
   it causes noise on the probe line of some Minis.
 */
#if defined(LULZBOT_IS_MINI)
    #define LULZBOT_EXTRUDER_MOTOR_SHUTOFF_ON_PROBE(probing) \
        if(probing) { \
            disable_E0(); \
        } else { \
            enable_E0(); \
        }
#else
    #define LULZBOT_EXTRUDER_MOTOR_SHUTOFF_ON_PROBE(probing)
#endif

#define LULZBOT_ENABLE_PROBE_PINS(enable) enable_probe_pins(enable);

/* Make it so M42 S<state> controls the state of the
 * probe lines. This is useful for troubleshooting. */
#define LULZBOT_M42_TOGGLES_PROBE_PINS \
    if (!parser.seenval('P')) { \
      LULZBOT_ENABLE_PROBE_PINS(pin_status); \
      return; \
    }

#if defined(LULZBOT_IS_MINI) && defined(LULZBOT_USE_Z_BELT)
    #define LULZBOT_MENU_AXIS_LEVELING_COMMANDS \
        "M117 Leveling X Axis\n" /* Set LCD status */ \
        "G28\n"                  /* Home Axis */ \
        "G0 X160 F9999\n"        /* Move toolhead to the right */ \
        "G0 Z5 F6000\n"          /* Move to bottom of printer */ \
        "G91\n"                  /* Set relative motion mode */ \
        "M211 S0\n"              /* Turn off soft endstops */ \
        "M400\n"                 /* Finish moves */ \
        "M906 Z600\n"            /* Lower current to 600mA */ \
        "G0 Z-15 F500\n"         /* Skip steppers against lower Z mounts */ \
        "G0 Z5 F500\n"           /* Move Z-Axis up a bit */ \
        "G90\n"                  /* Return to absolute mode */ \
        "M400\n"                 /* Finish moves */ \
        "M906 Z960\n"            /* Restore default current */ \
        "M211 S1\n"              /* Turn soft endstops back on */ \
        "G28\n"                  /* Rehome */ \
        "M117 Leveling done.\n"  /* Set LCD status */

#elif defined(LULZBOT_IS_TAZ) && defined(LULZBOT_USE_Z_BELT)
    #define LULZBOT_MENU_AXIS_LEVELING_COMMANDS \
        "M117 Leveling X Axis\n" /* Set LCD status */ \
        "G28 Z0\n"               /* Home Axis */ \
        "M400\n"                 /* Finish moves */ \
        "M121\n"                 /* Turn off hardware endstops */ \
        "M211 S0\n"              /* Turn off soft endstops */ \
        "M906 Z800\n"            /* Lower current to 800mA */ \
        "G1 Z312 F250\n"         /* Skip steppers on top */ \
        "M400\n"                 /* Finish moves */ \
        "M906 Z960\n"            /* Restore current to 960mA */ \
        "M211 S1\n"              /* Turn on soft endstops */ \
        "M120\n"                 /* Turn on hardware endstops */ \
        "G28 Z0\n"               /* Rehome */ \
        "M117 Leveling done.\n"  /* Set LCD status */
#endif

#if defined(LULZBOT_USE_Z_SCREW)
    // The older Minis seem succeptible to noise in the probe lines.
    // This restores the sampling of endstops as it existed in previous
    // version of Marlin.
    #define LULZBOT_ENDSTOP_NOISE_THRESHOLD 2
#endif

/*************************** COMMON TOOLHEADS PARAMETERS ***********************/

#define LULZBOT_DEFAULT_EJERK                10.0
#define LULZBOT_MANUAL_FEEDRATE_E             1.0 // mm/sec

/**************************** MINI TOOLHEADS ***********************************/

#if defined(TOOLHEAD_Gladiola_SingleExtruder) || defined(TOOLHEAD_Albatross_Flexystruder) || defined(TOOLHEAD_Finch_Aerostruder) || defined(TOOLHEAD_CecropiaSilk_SingleExtruderAeroV2) || defined(TOOLHEAD_AchemonSphinx_SmallLayer) || defined(TOOLHEAD_BandedTiger_HardenedSteel) || defined(TOOLHEAD_DingyCutworm_HardenedSteelPlus)

    #define LULZBOT_EXTRUDERS                  1
    #define LULZBOT_TOOLHEAD_X_MAX_ADJ         0
    #define LULZBOT_TOOLHEAD_X_MIN_ADJ         0
    #define LULZBOT_TOOLHEAD_Y_MAX_ADJ         0
    #define LULZBOT_TOOLHEAD_Y_MIN_ADJ         0
    #define LULZBOT_TOOLHEAD_Z_MAX_ADJ         0
    #define LULZBOT_TOOLHEAD_Z_MIN_ADJ         0
    #define LULZBOT_TOOLHEAD_WIPE_X1_ADJ       0
    #define LULZBOT_TOOLHEAD_WIPE_X2_ADJ       0
    #define LULZBOT_TOOLHEAD_WIPE_Y1_ADJ       0
    #define LULZBOT_TOOLHEAD_WIPE_Y2_ADJ       0

    #if defined(LULZBOT_USE_EINSY_RETRO) || \
        defined(LULZBOT_USE_ARCHIM2) || \
        defined(TOOLHEAD_CecropiaSilk_SingleExtruderAeroV2) || \
        defined(TOOLHEAD_AchemonSphinx_SmallLayer) || \
        defined(TOOLHEAD_BandedTiger_HardenedSteel) || \
        defined(TOOLHEAD_DingyCutworm_HardenedSteelPlus)
        #define LULZBOT_MOTOR_CURRENT_E          960 // mA
    #else
        #define LULZBOT_MOTOR_CURRENT_E         1250 // mA
    #endif
#endif /* TOOLHEAD_Gladiola_SingleExtruder || TOOLHEAD_Albatross_Flexystruder || TOOLHEAD_Finch_Aerostruder || TOOLHEAD_AchemonSphinx_SmallLayer || TOOLHEAD_BandedTiger_HardenedSteel) || TOOLHEAD_DingyCutworm_HardenedSteelPlus */

#if defined(TOOLHEAD_Gladiola_SingleExtruder)
    #define LULZBOT_LCD_TOOLHEAD_NAME              "Single Extruder"
//          16 chars max                            ^^^^^^^^^^^^^^^
    #define LULZBOT_M115_EXTRUDER_TYPE             "SingleExtruder"
    #define LULZBOT_X_MAX_ENDSTOP_INVERTING        LULZBOT_NORMALLY_OPEN_ENDSTOP
    #define LULZBOT_AO_Hexagon
    #define LULZBOT_E_STEPS                        833
#endif /* TOOLHEAD_Gladiola_SingleExtruder */

#if defined(TOOLHEAD_Albatross_Flexystruder)
    #define LULZBOT_LCD_TOOLHEAD_NAME              "Flexystruder"
//          16 chars max                            ^^^^^^^^^^^^^^^
    #define LULZBOT_M115_EXTRUDER_TYPE             "Flexystruder"
    #define LULZBOT_X_MAX_ENDSTOP_INVERTING        LULZBOT_NORMALLY_OPEN_ENDSTOP
    #define LULZBOT_AO_Hexagon
        #define LULZBOT_E_STEPS                    833
#endif /* TOOLHEAD_Albatross_Flexystruder */

#if defined(TOOLHEAD_Finch_Aerostruder)
    // Prototype Aero toolhead for Mini
    #define LULZBOT_LCD_TOOLHEAD_NAME              "Aerostruder"
//          16 chars max                            ^^^^^^^^^^^^^^^
    #define LULZBOT_M115_EXTRUDER_TYPE             "Aerostruder"
    #define LULZBOT_X_MAX_ENDSTOP_INVERTING        LULZBOT_NORMALLY_OPEN_ENDSTOP
    #define LULZBOT_E3D_Titan_Aero_V6
    #define LULZBOT_E_STEPS                        420
#endif /* TOOLHEAD_Finch_Aerostruder */

/****************************** TAZ 4/5/6 TOOLHEADS ********************************/

#if defined(TOOLHEAD_Tilapia_SingleExtruder) || defined(TOOLHEAD_Angelfish_Aerostruder)
    #define LULZBOT_EXTRUDERS                  1
    #define LULZBOT_TOOLHEAD_X_MAX_ADJ         0
    #define LULZBOT_TOOLHEAD_X_MIN_ADJ         0
    #define LULZBOT_TOOLHEAD_Y_MAX_ADJ         0
    #define LULZBOT_TOOLHEAD_Y_MIN_ADJ         0
    #define LULZBOT_TOOLHEAD_Z_MAX_ADJ         0
    #define LULZBOT_TOOLHEAD_Z_MIN_ADJ         0
    #define LULZBOT_TOOLHEAD_WIPE_X1_ADJ       0
    #define LULZBOT_TOOLHEAD_WIPE_X2_ADJ       0
    #define LULZBOT_TOOLHEAD_WIPE_Y1_ADJ       0
    #define LULZBOT_TOOLHEAD_WIPE_Y2_ADJ       0
#endif /* TOOLHEAD_Tilapia_SingleExtruder || TOOLHEAD_Angelfish_Aerostruder */

#if defined(TOOLHEAD_Tilapia_SingleExtruder)
    #define LULZBOT_LCD_TOOLHEAD_NAME          "Single Extruder"
//          16 chars max                        ^^^^^^^^^^^^^^^
    #define LULZBOT_M115_EXTRUDER_TYPE         "SingleExtruder"
    #define LULZBOT_MOTOR_CURRENT_E            750 // mA
    #define LULZBOT_X_MAX_ENDSTOP_INVERTING    LULZBOT_NORMALLY_CLOSED_ENDSTOP
    #define LULZBOT_AO_Hexagon
    #define LULZBOT_E_STEPS                    830
#endif /* TOOLHEAD_Tilapia_SingleExtruder */

#if defined(TOOLHEAD_Angelfish_Aerostruder)
    // Prototype Aero for TAZ
    #define LULZBOT_LCD_TOOLHEAD_NAME          "Aerostruder"
//          16 chars max                        ^^^^^^^^^^^^^^^
    #define LULZBOT_M115_EXTRUDER_TYPE         "Aerostruder"
    #define LULZBOT_MOTOR_CURRENT_E            875 // mA
    #define LULZBOT_X_MAX_ENDSTOP_INVERTING    LULZBOT_NORMALLY_CLOSED_ENDSTOP
    #define LULZBOT_E3D_Titan_Aero_V6
    #define LULZBOT_E_STEPS                    420
#endif /* TOOLHEAD_Angelfish_Aerostruder */

#if defined(TOOLHEAD_Kanyu_Flexystruder)
    #define LULZBOT_LCD_TOOLHEAD_NAME          "Flexystruder"
//          16 chars max                        ^^^^^^^^^^^^^^^
    #define LULZBOT_M115_EXTRUDER_TYPE         "Flexystruder"
    #define LULZBOT_MOTOR_CURRENT_E            410 // mA
    #define LULZBOT_EXTRUDERS                  1
    #define LULZBOT_X_MAX_ENDSTOP_INVERTING    LULZBOT_NORMALLY_OPEN_ENDSTOP
    #define LULZBOT_TOOLHEAD_X_MAX_ADJ        -12
    #define LULZBOT_TOOLHEAD_X_MIN_ADJ        -7
    #define LULZBOT_TOOLHEAD_Y_MAX_ADJ        -1
    #define LULZBOT_TOOLHEAD_Y_MIN_ADJ         0
    #define LULZBOT_TOOLHEAD_Z_MAX_ADJ         0
    #define LULZBOT_TOOLHEAD_Z_MIN_ADJ         0
    #define LULZBOT_TOOLHEAD_WIPE_X1_ADJ       0
    #define LULZBOT_TOOLHEAD_WIPE_X2_ADJ       0
    #define LULZBOT_TOOLHEAD_WIPE_Y1_ADJ       0
    #define LULZBOT_TOOLHEAD_WIPE_Y2_ADJ       0
    #define LULZBOT_AO_Hexagon
    #define LULZBOT_E_STEPS                    830
#endif /* TOOLHEAD_Kanyu_Flexystruder */

#if defined(TOOLHEAD_Opah_Moarstruder)
    #define LULZBOT_LCD_TOOLHEAD_NAME          "MOARstruder"
//          16 chars max                        ^^^^^^^^^^^^^^^
    #define LULZBOT_M115_EXTRUDER_TYPE         "MOARstruder"
    #define LULZBOT_DEFAULT_ACCELERATION       250
    #define LULZBOT_DEFAULT_TRAVEL_ACCELERATION 250
    #define LULZBOT_MOTOR_CURRENT_E            750 // mA
    #define LULZBOT_EXTRUDERS                  1
    #define LULZBOT_X_MAX_ENDSTOP_INVERTING    LULZBOT_NORMALLY_OPEN_ENDSTOP
    #define LULZBOT_TOOLHEAD_X_MAX_ADJ        -10
    #define LULZBOT_TOOLHEAD_X_MIN_ADJ         0
    #define LULZBOT_TOOLHEAD_Y_MAX_ADJ         0
    #define LULZBOT_TOOLHEAD_Y_MIN_ADJ         0
    #define LULZBOT_TOOLHEAD_Z_MAX_ADJ         0
    #define LULZBOT_TOOLHEAD_Z_MIN_ADJ         0
    #define LULZBOT_TOOLHEAD_WIPE_X1_ADJ       0
    #define LULZBOT_TOOLHEAD_WIPE_X2_ADJ       0
    #define LULZBOT_TOOLHEAD_WIPE_Y1_ADJ       0
    #define LULZBOT_TOOLHEAD_WIPE_Y2_ADJ       0
    #define LULZBOT_Moarstruder
    #define LULZBOT_E_STEPS                    830
#endif /* TOOLHEAD_Opah_Moarstruder */

#if defined(TOOLHEAD_Javelin_DualExtruderV2) || defined(TOOLHEAD_Longfin_FlexyDually) || defined(TOOLHEAD_Yellowfin_DualExtruderV3)
    #define LULZBOT_EXTRUDER_FAN_ON_PIN_6      // For backwards compatibility with TAZ 4
    #define LULZBOT_MOTOR_CURRENT_E0           875 // mA
    #define LULZBOT_MOTOR_CURRENT_E1           875 // mA
    #define LULZBOT_EXTRUDERS                  2
    #define LULZBOT_TOOLHEAD_X_MAX_ADJ        -12
    #define LULZBOT_TOOLHEAD_X_MIN_ADJ        -2
    #define LULZBOT_TOOLHEAD_Y_MAX_ADJ         0
    #define LULZBOT_TOOLHEAD_Y_MIN_ADJ         0
    #define LULZBOT_TOOLHEAD_Z_MAX_ADJ         0
    #define LULZBOT_TOOLHEAD_Z_MIN_ADJ         0
    #define LULZBOT_TOOLHEAD_WIPE_X1_ADJ       0
    #define LULZBOT_TOOLHEAD_WIPE_X2_ADJ       0
    #define LULZBOT_TOOLHEAD_WIPE_Y1_ADJ       0
    #define LULZBOT_TOOLHEAD_WIPE_Y2_ADJ       0
    #define LULZBOT_TEMP_SENSOR_1              5
    #define LULZBOT_NO_MOVE_ON_TOOLHEAD_CHANGE
    #define LULZBOT_DISTINCT_E_FACTORS
#endif /* TOOLHEAD_Javelin_DualExtruderV2 || TOOLHEAD_Longfin_FlexyDually || TOOLHEAD_Yellowfin_DualExtruderV3 */

#if defined(TOOLHEAD_Javelin_DualExtruderV2)
    #define LULZBOT_LCD_TOOLHEAD_NAME              "Dual Extruder 2"
//          16 chars max                            ^^^^^^^^^^^^^^^
    #define LULZBOT_M115_EXTRUDER_TYPE         "DualExtruder v2"
    #undef  LULZBOT_TOOLHEAD_WIPE_Y2_ADJ
    #define LULZBOT_TOOLHEAD_WIPE_Y2_ADJ       48
    #define LULZBOT_X_MAX_ENDSTOP_INVERTING    LULZBOT_NORMALLY_OPEN_ENDSTOP
    #define LULZBOT_E_STEPS                    830
    #define LULZBOT_AO_Hexagon
#endif /* TOOLHEAD_Javelin_DualExtruderV2 */

#if defined(TOOLHEAD_Longfin_FlexyDually)
    #define LULZBOT_LCD_TOOLHEAD_NAME              "FlexyDually v2"
//          16 chars max                            ^^^^^^^^^^^^^^^
    #define LULZBOT_M115_EXTRUDER_TYPE         "FlexyDually v2"
    #undef   LULZBOT_TOOLHEAD_WIPE_Y2_ADJ
    #define LULZBOT_TOOLHEAD_WIPE_Y2_ADJ       48
    #define LULZBOT_X_MAX_ENDSTOP_INVERTING    LULZBOT_NORMALLY_OPEN_ENDSTOP
    #define LULZBOT_E_STEPS                    830
    #define LULZBOT_AO_Hexagon
#endif /* TOOLHEAD_Longfin_FlexyDually */

#if defined(TOOLHEAD_Yellowfin_DualExtruderV3)
    // Prototype Dual v3 for TAZ.
    #define LULZBOT_LCD_TOOLHEAD_NAME              "Dual Extruder 3"
//          16 chars max                            ^^^^^^^^^^^^^^^
    #define LULZBOT_M115_EXTRUDER_TYPE         "DualExtruder v3"
    #undef  LULZBOT_AFTER_Z_HOME_Z_RAISE
    #define LULZBOT_AFTER_Z_HOME_Z_RAISE           16
    #undef  LULZBOT_AFTER_Z_HOME_Z_ORIGIN
    #define LULZBOT_AFTER_Z_HOME_Z_ORIGIN          5.5
    #undef  LULZBOT_TOOLHEAD_X_MIN_ADJ
    #undef  LULZBOT_TOOLHEAD_X_MAX_ADJ
    #undef  LULZBOT_TOOLHEAD_Y_MIN_ADJ
    #undef  LULZBOT_TOOLHEAD_Y_MAX_ADJ
    #define LULZBOT_TOOLHEAD_X_MIN_ADJ             1
    #define LULZBOT_TOOLHEAD_X_MAX_ADJ            -7
    #define LULZBOT_TOOLHEAD_Y_MIN_ADJ            -7
    #define LULZBOT_TOOLHEAD_Y_MAX_ADJ            -7
    #undef  LULZBOT_TOOLHEAD_WIPE_X1_ADJ
    #undef  LULZBOT_TOOLHEAD_WIPE_X2_ADJ
    #define LULZBOT_TOOLHEAD_WIPE_X1_ADJ          -2
    #define LULZBOT_TOOLHEAD_WIPE_X2_ADJ          -2
    #undef  LULZBOT_Z_HOMING_HEIGHT
    #define LULZBOT_Z_HOMING_HEIGHT                10
    #undef  LULZBOT_Z_CLEARANCE_DEPLOY_PROBE
    #define LULZBOT_Z_CLEARANCE_DEPLOY_PROBE       10
    #undef  LULZBOT_Z_CLEARANCE_BETWEEN_PROBES
    #define LULZBOT_Z_CLEARANCE_BETWEEN_PROBES     10
    // Move the rear homing position back to avoid the Z homing adaptor
    #undef  LULZBOT_STANDARD_BACK_PROBE_BED_POSITION
    #define LULZBOT_STANDARD_BACK_PROBE_BED_POSITION       291
    // Adjust so left nozzle probes on the left washers;
    // right nozzles on the right nozzle.
    #undef  LULZBOT_STANDARD_LEFT_PROBE_BED_POSITION
    #define LULZBOT_STANDARD_LEFT_PROBE_BED_POSITION        -3
    #undef  LULZBOT_STANDARD_RIGHT_PROBE_BED_POSITION
    #define LULZBOT_STANDARD_RIGHT_PROBE_BED_POSITION      282
    #define LULZBOT_X_MAX_ENDSTOP_INVERTING       LULZBOT_NORMALLY_CLOSED_ENDSTOP
    #define LULZBOT_SWAP_EXTRUDERS
    #undef  LULZBOT_INVERT_E1_DIR
    #define LULZBOT_INVERT_E1_DIR                 false
    #define LULZBOT_E_STEPS                        760
    #define LULZBOT_E3D_SOMEstruder_x2
#endif /* TOOLHEAD_Yellowfin_DualExtruderV3 */

/******************************** UNIVERSAL TOOLHEADS *************************/

#if defined(TOOLHEAD_CecropiaSilk_SingleExtruderAeroV2)
    #define LULZBOT_LCD_TOOLHEAD_NAME              "SE 0.5mm AeroV2"
//          16 chars max                            ^^^^^^^^^^^^^^^
    #define LULZBOT_M115_EXTRUDER_TYPE             "SingleExtruderAeroV2"
    #define LULZBOT_X_MAX_ENDSTOP_INVERTING        LULZBOT_NORMALLY_CLOSED_ENDSTOP
    #define LULZBOT_E3D_Titan_Aero_V6
    #define LULZBOT_E_STEPS                        420
    #define LULZBOT_MAY_USE_V2_ADAPTER
#endif /* TOOLHEAD_CecropiaSilk_SingleExtruderAeroV2 */

#if defined(TOOLHEAD_AchemonSphinx_SmallLayer)
    #define LULZBOT_LCD_TOOLHEAD_NAME              "SL 0.25mm Micro"
//          16 chars max                            ^^^^^^^^^^^^^^^
    #define LULZBOT_M115_EXTRUDER_TYPE             "SmallLayer"
    #define LULZBOT_X_MAX_ENDSTOP_INVERTING        LULZBOT_NORMALLY_CLOSED_ENDSTOP
    #define LULZBOT_E3D_Titan_Aero_V6
    #define LULZBOT_E_STEPS                        420
    #define LULZBOT_Z_PROBE_OFFSET_FROM_EXTRUDER   -1.24
    #undef  LULZBOT_MANUAL_FEEDRATE_E
    #define LULZBOT_MANUAL_FEEDRATE_E               0.7 // mm/sec
    #define LULZBOT_MAY_USE_V2_ADAPTER
#endif /* TOOLHEAD_AchemonSphinx_SmallLayer */

#if defined(TOOLHEAD_BandedTiger_HardenedSteel)
    #define LULZBOT_LCD_TOOLHEAD_NAME              "HS 0.8mm"
//          16 chars max                            ^^^^^^^^^^^^^^^
    #define LULZBOT_M115_EXTRUDER_TYPE             "HardenedSteel"
    #define LULZBOT_X_MAX_ENDSTOP_INVERTING        LULZBOT_NORMALLY_CLOSED_ENDSTOP
    #define LULZBOT_E3D_Titan_Aero_Volcano
    #define LULZBOT_E_STEPS                        420
    #define LULZBOT_MAY_USE_V2_ADAPTER
#endif /* TOOLHEAD_BandedTiger_HardenedSteel */

#if defined(TOOLHEAD_DingyCutworm_HardenedSteelPlus)
    #define LULZBOT_LCD_TOOLHEAD_NAME              "HS+ 1.2mm"
//          16 chars max                            ^^^^^^^^^^^^^^^
    #define LULZBOT_M115_EXTRUDER_TYPE             "HardenedSteelPlus"
    #define LULZBOT_X_MAX_ENDSTOP_INVERTING        LULZBOT_NORMALLY_CLOSED_ENDSTOP
    #define LULZBOT_E3D_Titan_Aero_Volcano
    #define LULZBOT_E_STEPS                        420
    #define LULZBOT_MAY_USE_V2_ADAPTER
#endif /* TOOLHEAD_DingyCutworm_HardenedSteelPlus */

// Using the V2 toolheads on the TAZ and older Minis requires an
// adapter plate that shifts the coordinate system
#if defined(LULZBOT_MAY_USE_AERO_V2_ADAPTER) && defined(LULZBOT_USE_Z_SCREW)
    #undef  LULZBOT_X_MAX_ENDSTOP_INVERTING
    #define LULZBOT_X_MAX_ENDSTOP_INVERTING LULZBOT_NO_ENDSTOP
    #define LULZBOT_NO_MOTION_BEFORE_HOMING

    #if defined(LULZBOT_IS_TAZ)
        #define LULZBOT_TOOLHEAD_Y_OFFSET -2.0
    #else
        #define LULZBOT_TOOLHEAD_Y_OFFSET -7.2
    #endif
    #if LULZBOT_Y_HOME_DIR > 0
        #undef  LULZBOT_TOOLHEAD_Y_MAX_ADJ
        #define LULZBOT_TOOLHEAD_Y_MAX_ADJ LULZBOT_TOOLHEAD_Y_OFFSET
    #else
        #undef  LULZBOT_TOOLHEAD_Y_MIN_ADJ
        #define LULZBOT_TOOLHEAD_Y_MIN_ADJ LULZBOT_TOOLHEAD_Y_OFFSET
    #endif
#endif

/*********************************** TAZ PRO TOOLHEADS ************************/

#if defined(TOOLHEAD_Quiver_DualExtruder)
    #define LULZBOT_LCD_TOOLHEAD_NAME              "Quivering Aeros"
//          16 chars max                            ^^^^^^^^^^^^^^^
    #define LULZBOT_M115_EXTRUDER_TYPE             "AerostruderDual"
    #define LULZBOT_TOOLHEAD_X_MAX_ADJ             -21
    #define LULZBOT_TOOLHEAD_X_MIN_ADJ             -21
    #define LULZBOT_TOOLHEAD_Y_MAX_ADJ             -21
    #define LULZBOT_TOOLHEAD_Y_MIN_ADJ             -21
    #define LULZBOT_TOOLHEAD_Z_MAX_ADJ             -7
    #define LULZBOT_TOOLHEAD_Z_MIN_ADJ             -7
    #define LULZBOT_TOOLHEAD_WIPE_X1_ADJ            0
    #define LULZBOT_TOOLHEAD_WIPE_X2_ADJ            0
    #define LULZBOT_TOOLHEAD_WIPE_Y1_ADJ            0
    #define LULZBOT_TOOLHEAD_WIPE_Y2_ADJ            0
    #define LULZBOT_EXTRUDERS                       2
    #define LULZBOT_NUM_SERVOS                      2
    #define LULZBOT_SERVO_DELAY                    {1000, 1000}
    #define LULZBOT_SWITCHING_NOZZLE
    #define LULZBOT_SWITCHING_NOZZLE_SERVO_ANGLES  { 55,   120}
    #define LULZBOT_SWITCHING_NOZZLE_OPPOSING_SERVOS
    #define LULZBOT_SWITCHING_NOZZLE_NO_Z_LIFT
    #define LULZBOT_HOTEND_OFFSET_X                {0.0, 43}
    #define LULZBOT_HOTEND_OFFSET_Y                {0.0,  0}
    #define LULZBOT_E_STEPS                        420
    #define LULZBOT_X_MAX_ENDSTOP_INVERTING        LULZBOT_NO_ENDSTOP
    #define LULZBOT_E3D_Titan_Aero_V6
    #define LULZBOT_TEMP_SENSOR_1                  5
    #define LULZBOT_MOTOR_CURRENT_E0               960 // mA
    #define LULZBOT_MOTOR_CURRENT_E1               960 // mA
#endif /* TOOLHEAD_Quiver_DualExtruder */

/************** AUTO-CALIBRATION (BACKLASH AND NOZZLE OFFSET) ****************/

#if defined(LULZBOT_HAS_CALIBRATION_CUBE)
    #define LULZBOT_CALIBRATE_ON_CUBE

#elif defined(LULZBOT_IS_MINI)
    #define LULZBOT_CALIBRATE_ON_BACK_RIGHT_WASHER
#endif

#if defined(LULZBOT_IS_MINI)
    #if defined(LULZBOT_CALIBRATE_ON_FRONT_LEFT_WASHER)
        #define CALIBRATION_CUBE_DIMENSIONS              {22.0,   22.0,  1.5} // mm
        #define CALIBRATION_CUBE_CENTER                  {-8.9,   -7.6,  0}   // mm
        #define CALIBRATION_CUBE_RIGHT_SIDE_MEASUREMENT
        #define CALIBRATION_CUBE_BACK_SIDE_MEASUREMENT
        #define LULZBOT_CALIBRATION_GCODE

    #elif defined(LULZBOT_CALIBRATE_ON_FRONT_RIGHT_WASHER)
        #define CALIBRATION_CUBE_DIMENSIONS              { 22.0,   22.0, 1.5} // mm
        #define CALIBRATION_CUBE_CENTER                  {169.5,   -7.6, 0}   // mm
        #define CALIBRATION_CUBE_LEFT_SIDE_MEASUREMENT
        #define CALIBRATION_CUBE_BACK_SIDE_MEASUREMENT
        #define LULZBOT_CALIBRATION_GCODE

    #elif defined(LULZBOT_CALIBRATE_ON_BACK_LEFT_WASHER)
        #define CALIBRATION_CUBE_DIMENSIONS              { 22.0,   22.0, 1.5} // mm
        #define CALIBRATION_CUBE_CENTER                  { -8.9,  171.3, 0}   // mm
        #define CALIBRATION_CUBE_RIGHT_SIDE_MEASUREMENT
        #define CALIBRATION_CUBE_FRONT_SIDE_MEASUREMENT
        #define LULZBOT_CALIBRATION_GCODE

    #elif defined(LULZBOT_CALIBRATE_ON_BACK_RIGHT_WASHER)
        #define CALIBRATION_CUBE_DIMENSIONS              { 22.0,   22.0, 1.5} // mm
        #define CALIBRATION_CUBE_CENTER                  {169.5,  171.3, 0}   // mm
        #define CALIBRATION_CUBE_LEFT_SIDE_MEASUREMENT
        #define CALIBRATION_CUBE_FRONT_SIDE_MEASUREMENT
        #define LULZBOT_CALIBRATION_GCODE
    #endif

#elif defined(LULZBOT_IS_TAZ) && defined(LULZBOT_USE_Z_BELT)
    #ifdef LULZBOT_CALIBRATE_ON_CUBE
        #define CALIBRATION_CUBE_DIMENSIONS              {  10.0,  10.0,  10.0} // mm
        #define CALIBRATION_CUBE_CENTER                  { 264.0, -22.0,  -2.0} // mm

        #define CALIBRATION_CUBE_TOP_CENTER_MEASUREMENT
        #define CALIBRATION_CUBE_RIGHT_SIDE_MEASUREMENT
        #if defined(TOOLHEAD_Quiver_DualExtruder)
            #define CALIBRATION_CUBE_FRONT_SIDE_MEASUREMENT
        #endif
        #define CALIBRATION_CUBE_LEFT_SIDE_MEASUREMENT
        #define CALIBRATION_CUBE_BACK_SIDE_MEASUREMENT
        #define LULZBOT_CALIBRATION_GCODE
    #endif
#endif

#if defined(LULZBOT_CALIBRATION_GCODE)
    #define LULZBOT_CALIBRATION_SCRIPT \
        "M117 Starting Auto-Calibration\n"   /* Status message */ \
        "T0\n"                               /* Switch to first nozzle */ \
        "M218 T1 X43 Y0 Z0\n"                /* Restore default nozzle offset */ \
        "G28\n"                              /* Auto-Home */ \
        LULZBOT_MENU_AXIS_LEVELING_COMMANDS  /* Level X-Axis */ \
        "G12\n"                              /* Wipe the Nozzles */ \
        "G425\n"                             /* Calibrate Nozzles */ \
        "M500\n"                             /* Save settings */ \
        "M117 Calibration data saved"        /* Status message */
#endif

/*************************** TEMPERATURE SETTINGS *****************************/

#define LULZBOT_TEMP_SENSOR_0                        5
#define LULZBOT_TEMP_SENSOR_BED                      7
#define LULZBOT_TEMP_RESIDENCY_TIME                  1
#define LULZBOT_TEMP_HYSTERESIS                     10
#define LULZBOT_TEMP_WINDOW                         10

#define LULZBOT_TEMP_BED_RESIDENCY_TIME              1
#define LULZBOT_TEMP_BED_HYSTERESIS                  5
#define LULZBOT_TEMP_BED_WINDOW                      5

#define LULZBOT_HEATER_MAXTEMP                     305
#define LULZBOT_EXTRUDE_MINTEMP                    120

#define LULZBOT_THERMAL_PROTECTION_PERIOD           15     // Seconds
#define LULZBOT_THERMAL_PROTECTION_HYSTERESIS       30     // Degrees Celsius

#define LULZBOT_THERMAL_PROTECTION_BED_PERIOD       15     // Seconds
#define LULZBOT_THERMAL_PROTECTION_BED_HYSTERESIS   10     // Degrees Celsius

#define LULZBOT_PREHEAT_1_TEMP_HOTEND              200     // PLA

#if defined(LULZBOT_IS_MINI)
    // Heater current: 24V/5.5 Ohms = 4.4A
    #define LULZBOT_MAX_BED_POWER      255  // limits duty cycle to bed; 255=full current
    #define LULZBOT_WATCH_TEMP_PERIOD   20  // Seconds
    #define LULZBOT_WATCH_TEMP_INCREASE  2  // Degrees Celsius
#elif defined(LULZBOT_IS_TAZ)
    // Heater current: 24V/1.6 Ohms = 15A
    // Set Max Bed Power to 80% for a safety margin on the 15A fuse.
    #define LULZBOT_MAX_BED_POWER      206  // limits duty cycle to bed; 255=full current
    #define LULZBOT_WATCH_TEMP_PERIOD   40  // Seconds
    #define LULZBOT_WATCH_TEMP_INCREASE 10  // Degrees Celsius
#endif

// Marlin 1.1.5 no longer issues MIN_TEMP errors and appears to handle
// thermal runaway via other means. However, since our users expect a
// MIN_TEMP error when disconnecting their print head, this could be
// perceived as a safety issue. This is a workaround in "temperature.cpp"
// to re-enable that functionality.

#define LULZBOT_MIN_TEMP_WORKAROUND \
    static int delayBeforeStartMeasuring = OVERSAMPLENR; \
        if(delayBeforeStartMeasuring > 0) { \
            delayBeforeStartMeasuring--; \
        } else { \
            if (current_temperature[e] > HEATER_0_MAXTEMP) max_temp_error(0); \
            if (current_temperature[e] < HEATER_0_MINTEMP) min_temp_error(0); \
        }

/******************************** HEATING ELEMENTS *****************************/

#define LULZBOT_PIDTEMP
#define LULZBOT_PIDTEMPBED

// Hotend variants

#if defined(LULZBOT_Moarstruder)
    // LulzBot MOARstruder (40w)
    #define LULZBOT_DEFAULT_Kp 55.64
    #define LULZBOT_DEFAULT_Ki 6.79
    #define LULZBOT_DEFAULT_Kd 113.94
#endif /* LULZBOT_Moarstruder */

#if defined(LULZBOT_E3D_SOMEstruder_x2)
    // Side-by-side LulzBot E3D SOMEstruder on Yellowfin Dual
    #define LULZBOT_DEFAULT_Kp 47.45
    #define LULZBOT_DEFAULT_Ki 4.83
    #define LULZBOT_DEFAULT_Kd 116.63
#endif /* LULZBOT_E3D_SOMEstruder_x2 */

#if defined(LULZBOT_AO_Hexagon)
    // LulzBot AO-Hexagon (30w)
    #define LULZBOT_DEFAULT_Kp 28.79
    #define LULZBOT_DEFAULT_Ki 1.91
    #define LULZBOT_DEFAULT_Kd 108.51
#endif /* LULZBOT_AO_Hexagon */

#if defined(LULZBOT_E3D_Titan_Aero_V6)
    // E3D Titan Aero with LulzBot V6 block
    #define LULZBOT_DEFAULT_Kp 21.00
    #define LULZBOT_DEFAULT_Ki  1.78
    #define LULZBOT_DEFAULT_Kd 61.93
#endif /* LULZBOT_E3D_Titan_Aero */

#if defined(LULZBOT_E3D_Titan_Aero_Volcano)
    // E3D Titan Aero with Volcano block
    #define LULZBOT_DEFAULT_Kp 37.55
    #define LULZBOT_DEFAULT_Ki  5.39
    #define LULZBOT_DEFAULT_Kd 65.36
#endif

// Heated bed variants

//24V 360W silicone heater from NPH on 3mm borosilicate (TAZ 2.2+)
#if defined(LULZBOT_TAZ_BED) && !defined(LULZBOT_TWO_PIECE_BED)
  #define LULZBOT_DEFAULT_bedKp                 162
  #define LULZBOT_DEFAULT_bedKi                 17
  #define LULZBOT_DEFAULT_bedKd                 378

// Modular two piece bed (Mini 2/TAZ Pro)
#elif defined(LULZBOT_TAZ_BED) && defined(LULZBOT_TWO_PIECE_BED)
  #define LULZBOT_DEFAULT_bedKp                 286.02
  #define LULZBOT_DEFAULT_bedKi                 54.55
  #define LULZBOT_DEFAULT_bedKd                 374.90

//24V 360W silicone heater from NPH on 3mm borosilicate (TAZ 2.2+)
#elif defined(LULZBOT_MINI_BED) && !defined(LULZBOT_TWO_PIECE_BED)
  #define LULZBOT_DEFAULT_bedKp                 294
  #define LULZBOT_DEFAULT_bedKi                 65
  #define LULZBOT_DEFAULT_bedKd                 382

// Modular two piece bed (Mini 2+)
#elif defined(LULZBOT_MINI_BED) && defined(LULZBOT_TWO_PIECE_BED)
  #define LULZBOT_DEFAULT_bedKp                 384.33
  #define LULZBOT_DEFAULT_bedKi                  72.17
  #define LULZBOT_DEFAULT_bedKd                 511.64
#endif

/************************** COOLING FAN CONFIGURATION ***************************/

// For the Pelonis C4010L24BPLB1b-7 fan, we need a relative low
// PWM frequency of about 122Hz in order for the fan to turn.


#if defined(LULZBOT_USE_ARCHIM2)
    // On the Archim, it is necessary to use soft PWM to get the
    // frequency down in the kilohertz
    #define LULZBOT_FAN_SOFT_PWM
#else
    // By default, FAST_PWM_FAN appears to PWM at ~31kHz, but if we
    // set a prescale of 4, it divides this by 256 to get us down to
    // the frequency we need.
    #define LULZBOT_FAST_PWM_FAN
    #define LULZBOT_FAST_PWM_SCALE                4
#endif

#define LULZBOT_FAN_KICKSTART_TIME              100
#define LULZBOT_FAN_MIN_PWM                      70
#define LULZBOT_SOFT_PWM_SCALE                    4

#define LULZBOT_USE_CONTROLLER_FAN
#if defined(LULZBOT_USE_EINSY_RETRO)
    // The TMC drivers need a bit more cooling.
    #define LULZBOT_CONTROLLERFAN_SPEED                    255
    #define LULZBOT_CONTROLLERFAN_SPEED_WHEN_ONLY_Z_ACTIVE 120
#elif defined(LULZBOT_IS_MINI)
    // The Mini fan runs rather loud at full speed.
    #define LULZBOT_CONTROLLERFAN_SPEED         120
#elif defined(LULZBOT_IS_TAZ)
    #define LULZBOT_CONTROLLERFAN_SPEED         255
#endif

/******************************* AXIS TRAVEL LIMITS *****************************/

/* Define min and max travel limits based on the printer model using a standard
 * toolhead, then define adjustments from the standard for alternative toolheads.
 * This allows us to accomodate toolheads that might be wider or taller than the
 * standard.
 */

#if defined(LULZBOT_IS_MINI) && defined(LULZBOT_USE_Z_SCREW)
    #define LULZBOT_STANDARD_X_MAX_POS         162
    #define LULZBOT_STANDARD_X_MIN_POS          -3
    #define LULZBOT_STANDARD_Y_MAX_POS         190
    #define LULZBOT_STANDARD_Y_MIN_POS          -7

    #define LULZBOT_STANDARD_X_BED_SIZE        155
    #define LULZBOT_STANDARD_Y_BED_SIZE        155

#elif defined(LULZBOT_IS_MINI) && defined(LULZBOT_USE_Z_BELT)
    #define LULZBOT_STANDARD_X_MAX_POS         173
    #define LULZBOT_STANDARD_X_MIN_POS          -3
    #define LULZBOT_STANDARD_Y_MAX_POS         192
    #define LULZBOT_STANDARD_Y_MIN_POS          -5

    #define LULZBOT_STANDARD_X_BED_SIZE        157
    #define LULZBOT_STANDARD_Y_BED_SIZE        157

#elif defined(LULZBOT_Juniper_TAZ5)
    #define LULZBOT_STANDARD_X_MAX_POS         298
    #define LULZBOT_STANDARD_X_MIN_POS           0
    #define LULZBOT_STANDARD_Y_MAX_POS         276
    #define LULZBOT_STANDARD_Y_MIN_POS           0

    #define LULZBOT_STANDARD_X_BED_SIZE        288
    #define LULZBOT_STANDARD_Y_BED_SIZE        275

#elif defined(LULZBOT_IS_TAZ) && defined(LULZBOT_USE_Z_BELT)
    #define LULZBOT_STANDARD_X_MAX_POS         321
    #define LULZBOT_STANDARD_X_MIN_POS          -5
    #define LULZBOT_STANDARD_Y_MAX_POS         311
    #define LULZBOT_STANDARD_Y_MIN_POS         -17

    #define LULZBOT_STANDARD_X_BED_SIZE        280
    #define LULZBOT_STANDARD_Y_BED_SIZE        280

#elif defined(LULZBOT_IS_TAZ) && defined(LULZBOT_USE_Z_SCREW)
    #define LULZBOT_STANDARD_X_MAX_POS         300
    #define LULZBOT_STANDARD_X_MIN_POS         -20
    #define LULZBOT_STANDARD_Y_MAX_POS         303
    #define LULZBOT_STANDARD_Y_MIN_POS         -20

    #define LULZBOT_STANDARD_X_BED_SIZE        280
    #define LULZBOT_STANDARD_Y_BED_SIZE        280
#endif

#if defined(LULZBOT_IS_MINI) && defined(LULZBOT_USE_Z_SCREW)
    #define LULZBOT_STANDARD_Z_MIN_POS          -2
    #define LULZBOT_STANDARD_Z_MAX_POS         159

#elif defined(LULZBOT_IS_MINI) && defined(LULZBOT_USE_Z_BELT)
    #define LULZBOT_STANDARD_Z_MIN_POS           0
    #define LULZBOT_STANDARD_Z_MAX_POS         183

#elif defined(LULZBOT_Juniper_TAZ5)
    #define LULZBOT_STANDARD_Z_MIN_POS           0
    #define LULZBOT_STANDARD_Z_MAX_POS         250

#elif defined(LULZBOT_IS_TAZ) && defined(LULZBOT_USE_Z_SCREW)
    #define LULZBOT_STANDARD_Z_MIN_POS           0
    #define LULZBOT_STANDARD_Z_MAX_POS         270

#elif defined(LULZBOT_IS_TAZ) && defined(LULZBOT_USE_Z_BELT)
    #define LULZBOT_STANDARD_Z_MIN_POS          -2
    #define LULZBOT_STANDARD_Z_MAX_POS         299
#endif

#define LULZBOT_X_MAX_POS (LULZBOT_STANDARD_X_MAX_POS + LULZBOT_TOOLHEAD_X_MAX_ADJ)
#define LULZBOT_X_MIN_POS (LULZBOT_STANDARD_X_MIN_POS + LULZBOT_TOOLHEAD_X_MIN_ADJ)
#define LULZBOT_Y_MAX_POS (LULZBOT_STANDARD_Y_MAX_POS + LULZBOT_TOOLHEAD_Y_MAX_ADJ)
#define LULZBOT_Y_MIN_POS (LULZBOT_STANDARD_Y_MIN_POS + LULZBOT_TOOLHEAD_Y_MIN_ADJ)
#define LULZBOT_Z_MAX_POS (LULZBOT_STANDARD_Z_MAX_POS + LULZBOT_TOOLHEAD_Z_MAX_ADJ)
#define LULZBOT_Z_MIN_POS (LULZBOT_STANDARD_Z_MIN_POS + LULZBOT_TOOLHEAD_Z_MIN_ADJ)

#define LULZBOT_X_BED_SIZE min(LULZBOT_X_MAX_POS, LULZBOT_STANDARD_X_BED_SIZE)
#define LULZBOT_Y_BED_SIZE min(LULZBOT_Y_MAX_POS, LULZBOT_STANDARD_Y_BED_SIZE)

#define LULZBOT_LEFT_PROBE_BED_POSITION    max(LULZBOT_STANDARD_LEFT_PROBE_BED_POSITION,  LULZBOT_X_MIN_POS)
#define LULZBOT_RIGHT_PROBE_BED_POSITION   min(LULZBOT_STANDARD_RIGHT_PROBE_BED_POSITION, LULZBOT_X_MAX_POS)
#define LULZBOT_BACK_PROBE_BED_POSITION    min(LULZBOT_STANDARD_BACK_PROBE_BED_POSITION,  LULZBOT_Y_MAX_POS)
#define LULZBOT_FRONT_PROBE_BED_POSITION   max(LULZBOT_STANDARD_FRONT_PROBE_BED_POSITION, LULZBOT_Y_MIN_POS)

/**************************** ENDSTOP CONFIGURATION ****************************/

// Whether endstops are inverting
#define LULZBOT_NORMALLY_CLOSED_ENDSTOP           0
#define LULZBOT_NORMALLY_OPEN_ENDSTOP             1
#define LULZBOT_NO_ENDSTOP                        1

#if defined(LULZBOT_USE_MIN_ENDSTOPS)
    #define LULZBOT_USE_XMIN_PLUG
    #define LULZBOT_USE_YMIN_PLUG
#endif
#if defined(LULZBOT_USE_MIN_ENDSTOPS) || defined(LULZBOT_Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN)
    #define LULZBOT_USE_ZMIN_PLUG
#endif

// Z-Max Endstops were introduced on the Mini and TAZ 6
#if defined(LULZBOT_USE_MAX_ENDSTOPS)
    #define LULZBOT_USE_XMAX_PLUG
    #define LULZBOT_USE_YMAX_PLUG
    #define LULZBOT_USE_ZMAX_PLUG
#endif

#define LULZBOT_ABORT_ON_ENDSTOP_HIT_FEATURE_ENABLED

// Workaround for bug in Marlin 1.1.5 where motion is stopped a X or Y = 0,
// On the Hibiscus, enable software endstops
#if defined(LULZBOT_USE_Z_BELT)
    #define LULZBOT_MIN_SOFTWARE_ENDSTOPS
#endif

/* Endstop settings are determined by printer model, except for the
 * X_MAX which varies by toolhead. */

#if defined(LULZBOT_USE_NORMALLY_CLOSED_ENDSTOPS)
    // TAZ 6+ and Hibiscus Mini onwards use normally closed endstops.
    // This is safer, as a loose connector or broken wire will halt
    // the axis
    #define LULZBOT_X_MIN_ENDSTOP_INVERTING   LULZBOT_NORMALLY_CLOSED_ENDSTOP
    #define LULZBOT_Y_MIN_ENDSTOP_INVERTING   LULZBOT_NORMALLY_CLOSED_ENDSTOP


    // LULZBOT_X_MAX_ENDSTOP_INVERTING varies by toolhead
    #define LULZBOT_Y_MAX_ENDSTOP_INVERTING   LULZBOT_NORMALLY_CLOSED_ENDSTOP
    #define LULZBOT_Z_MAX_ENDSTOP_INVERTING   LULZBOT_NORMALLY_CLOSED_ENDSTOP

#elif defined(LULZBOT_USE_NORMALLY_OPEN_ENDSTOPS)
    #define LULZBOT_X_MIN_ENDSTOP_INVERTING   LULZBOT_NORMALLY_OPEN_ENDSTOP
    #define LULZBOT_Y_MIN_ENDSTOP_INVERTING   LULZBOT_NORMALLY_OPEN_ENDSTOP

    // LULZBOT_X_MAX_ENDSTOP_INVERTING varies by toolhead
    #define LULZBOT_Y_MAX_ENDSTOP_INVERTING   LULZBOT_NORMALLY_OPEN_ENDSTOP
    #define LULZBOT_Z_MAX_ENDSTOP_INVERTING   LULZBOT_NORMALLY_OPEN_ENDSTOP
#endif

// Electrical probing pins are always open until contact is made
#define LULZBOT_Z_MIN_ENDSTOP_INVERTING       LULZBOT_NORMALLY_OPEN_ENDSTOP
#define LULZBOT_Z_MIN_PROBE_ENDSTOP_INVERTING LULZBOT_NORMALLY_OPEN_ENDSTOP

/*************************** FILAMENT CONFIGURATION **************************/

// Enable linear advance, but leave K at zero so
// it is not used unless the user requests it.
#define LULZBOT_LIN_ADVANCE
#define LULZBOT_LIN_ADVANCE_K 0.0

#define LULZBOT_NO_VOLUMETRICS

#if defined(LULZBOT_FILAMENT_RUNOUT_SENSOR)
    #define LULZBOT_FILAMENT_RUNOUT_SENSOR
    #define LULZBOT_NUM_RUNOUT_SENSORS LULZBOT_EXTRUDERS
    #define LULZBOT_FILAMENT_RUNOUT_SCRIPT "M25\n"

    #define LULZBOT_RUNOUT_DISTANCE_MM 14
    #define LULZBOT_FILAMENT_MOTION_SENSOR

    #define LULZBOT_FILAMENT_RUNOUT_SENSOR_DEBUG

    #define LULZBOT_ACTION_ON_FILAMENT_RUNOUT "pause: filament_runout"
#endif

/******************************* MOTOR DRIVER TYPE ******************************/

#if defined(LULZBOT_USE_EINSY_RETRO) || defined(LULZBOT_USE_ARCHIM2)
    #define LULZBOT_DRIVER_TYPE  TMC2130
#else
    #define LULZBOT_DRIVER_TYPE  A4988
#endif

#define LULZBOT_X_DRIVER_TYPE  LULZBOT_DRIVER_TYPE
#define LULZBOT_Y_DRIVER_TYPE  LULZBOT_DRIVER_TYPE
#define LULZBOT_Z_DRIVER_TYPE  LULZBOT_DRIVER_TYPE
#define LULZBOT_E0_DRIVER_TYPE LULZBOT_DRIVER_TYPE
#define LULZBOT_E1_DRIVER_TYPE LULZBOT_DRIVER_TYPE

/************************ TRINAMIC DRIVER CONFIGURATION  ************************/

#if defined(LULZBOT_USE_EINSY_RETRO) || defined(LULZBOT_USE_ARCHIM2)
    #define LULZBOT_TMC_DEBUG
    #define LULZBOT_MONITOR_DRIVER_STATUS

    #define LULZBOT_R_SENSE         0.12
    #define LULZBOT_HOLD_MULTIPLIER 0.5

    #if defined(LULZBOT_USE_ARCHIM2)
        #define LULZBOT_TMC_USE_SW_SPI
        #define LULZBOT_SHAFT_DIR 0
    #elif defined(LULZBOT_USE_EINSY_RETRO)
        #define LULZBOT_SHAFT_DIR 1 /* Match direction to the Mini-Rambo */
    #endif

    // If true, use STEALTHCHOP, otherwise use COOLSTEP
    #if defined(LULZBOT_STEALTHCHOP_XY) || defined(LULZBOT_STEALTHCHOP_Z)
        // According to Jason at UltiMachine, setting the lower the
        // stealth freq the cooler the motor drivers will operate.
        #define LULZBOT_STEALTH_FREQ 0
    #endif

    #define LULZBOT_Y_HYBRID_THRESHOLD 72
    #define LULZBOT_X_HYBRID_THRESHOLD 72

    #define LULZBOT_TMC_INIT(st) \
        st.shaft(LULZBOT_SHAFT_DIR); \
        /* Enable coolstep */ \
        st.semin(1); \
        st.semax(3);

    #if defined(LULZBOT_IS_MINI)
        // Low-noise stepper settings for Mini 2
        #define LULZBOT_TMC_LOW_NOISE(st) \
            st.toff(  1); /* TOFF   = [1..15]  */ \
            st.hstrt( 0); /* HSTART = [0..7]   */ \
            st.hend(  0); /* HEND   = [0..15]  */ \
            st.tbl(   1); /* TBL    = [0..3]   */

        /* X axis driver temperature tuning notes over a 10 minute print:
         *  - TOFF caused the greatest effect on driver temperature (~40C) and needs
         *    to be at 1 to keep the drivers from overheating (was tested at 1-3)
         *  - TBL can be either 0, 1 or 2, but 3 will cause overheating and 1 appears
         *    to run the coolest.
         *  - Setting HSTRT higher than 5 cause the drivers to warm up.
         *  - Setting HSTRT lower than 3 causes the motor to stall.
         *  - Setting HEND higher than 1 causes drivers to overheat.
         */
        #define LULZBOT_TMC_LOW_HEAT(st) \
            st.toff(  1); /* TOFF   = [1..15]  */ \
            st.hstrt( 4); /* HSTART = [0..7]   */ \
            st.hend(  0); /* HEND   = [0..15]  */ \
            st.tbl(   1); /* TBL    = [0..3]   */

        #define LULZBOT_TMC_ADV { \
                LULZBOT_TMC_LOW_HEAT (stepperX)  \
                LULZBOT_TMC_LOW_HEAT (stepperY)  \
                LULZBOT_TMC_LOW_NOISE(stepperZ)  \
                LULZBOT_TMC_LOW_NOISE(stepperE0) \
                LULZBOT_TMC_INIT(stepperX) \
                LULZBOT_TMC_INIT(stepperY) \
                LULZBOT_TMC_INIT(stepperZ) \
                LULZBOT_TMC_INIT(stepperE0) \
            }
    #else
        #if LULZBOT_EXTRUDERS == 1
            #define LULZBOT_TMC_ADV { \
                    LULZBOT_TMC_INIT(stepperX) \
                    LULZBOT_TMC_INIT(stepperY) \
                    LULZBOT_TMC_INIT(stepperZ) \
                    LULZBOT_TMC_INIT(stepperE0) \
                }
        #else
            #define LULZBOT_TMC_ADV { \
                    LULZBOT_TMC_INIT(stepperX) \
                    LULZBOT_TMC_INIT(stepperY) \
                    LULZBOT_TMC_INIT(stepperZ) \
                    LULZBOT_TMC_INIT(stepperE0) \
                    LULZBOT_TMC_INIT(stepperE1) \
                }
        #endif
    #endif
#endif

/*********************** TRINAMIC SENSORLESS HOMING ***************************/

#if defined(LULZBOT_SENSORLESS_HOMING)
    #if defined(LULZBOT_IS_TAZ)
        #define LULZBOT_X_STALL_SENSITIVITY 6
        #define LULZBOT_Y_STALL_SENSITIVITY 5

    #elif defined(LULZBOT_IS_MINI)
        #define LULZBOT_X_STALL_SENSITIVITY 4
        #define LULZBOT_Y_STALL_SENSITIVITY 4
    #endif

    #define LULZBOT_USE_XMIN_PLUG // Uses Stallguard
    #define LULZBOT_USE_YMAX_PLUG // Uses Stallguard
    #define LULZBOT_USE_ZMIN_PLUG
    #define LULZBOT_USE_ZMAX_PLUG

    #undef  LULZBOT_X_MIN_ENDSTOP_INVERTING
    #undef  LULZBOT_Y_MAX_ENDSTOP_INVERTING
    #define LULZBOT_X_MIN_ENDSTOP_INVERTING LULZBOT_NORMALLY_OPEN_ENDSTOP
    #define LULZBOT_Y_MAX_ENDSTOP_INVERTING LULZBOT_NORMALLY_OPEN_ENDSTOP

    // Quickhome does not work with sensorless homing
    #undef LULZBOT_QUICK_HOME

    #define LULZBOT_X_HOME_BUMP_MM                0
    #define LULZBOT_Y_HOME_BUMP_MM                0
    #define LULZBOT_Z_HOME_BUMP_MM                2
#else
    #define LULZBOT_X_HOME_BUMP_MM                5
    #define LULZBOT_Y_HOME_BUMP_MM                5
    #define LULZBOT_Z_HOME_BUMP_MM                2
#endif

/************************** ADVANCED PAUSE / FILAMENT CHANGE ***********************/

#if defined(LULZBOT_USE_REPRAP_LCD_DISPLAY) || defined(LULZBOT_USE_TOUCH_UI)
    #define LULZBOT_ADVANCED_PAUSE_FEATURE
    #define LULZBOT_FILAMENT_CHANGE_FAST_LOAD_FEEDRATE LULZBOT_MANUAL_FEEDRATE_E
    #define LULZBOT_FILAMENT_CHANGE_FAST_LOAD_LENGTH 40
    #define LULZBOT_ADVANCED_PAUSE_PURGE_LENGTH  20
    #define LULZBOT_ADVANCED_PAUSE_PURGE_FEEDRATE LULZBOT_MANUAL_FEEDRATE_E
    #define LULZBOT_PAUSE_PARK_RETRACT_FEEDRATE   10  // mm/s
    #define LULZBOT_FILAMENT_CHANGE_UNLOAD_LENGTH 80
    #define LULZBOT_FILAMENT_CHANGE_UNLOAD_FEEDRATE 5 // mm/s
    #define LULZBOT_HOME_BEFORE_FILAMENT_CHANGE
    #define LULZBOT_PARK_HEAD_ON_PAUSE

    // Clarify some of the status messages
    #define MSG_FILAMENT_CHANGE_HEADER_PAUSE  _UxGT("Changing Filament")
    #define MSG_FILAMENT_CHANGE_RESUME_1      _UxGT("")
    #define MSG_FILAMENT_CHANGE_RESUME_2      _UxGT("Please wait.")

    #define MSG_FILAMENT_CHANGE_OPTION_HEADER _UxGT("")
    #define MSG_FILAMENT_CHANGE_OPTION_RESUME _UxGT("End filament change")

    // In Marlin 1.1.9, the filament unload sequence makes no sense
    // All we want is a slow purge for the Aerostruder, followed by
    // a retract.
    #define LULZBOT_AEROSTRUDER_UNLOAD_WORKAROUND
    #define LULZBOT_AEROSTRUDER_UNLOAD_PURGE_LENGTH   6
    #define LULZBOT_AEROSTRUDER_UNLOAD_PURGE_FEEDRATE LULZBOT_MANUAL_FEEDRATE_E

    // In Marlin 1.1.9, an unnecessary purge is happening after a resume from a pause.
    #define LULZBOT_ADVANCED_PAUSE_PURGE_WORKAROUND
#endif

#if defined(LULZBOT_IS_MINI)
    #define LULZBOT_NOZZLE_PARK_POINT {  10, (LULZBOT_Y_MAX_POS - 10), 20 }
#else
    // Match the purge location of the v3 dual so a single tray can be used.
    #define LULZBOT_NOZZLE_PARK_POINT { 100, (LULZBOT_Y_MAX_POS -  1), 20 }
#endif

#define LULZBOT_ACTION_ON_PAUSE_AND_RESUME

#if defined(LULZBOT_IS_MINI)
    #define LULZBOT_AFTER_ABORT_PRINT_ACTION GcodeSuite::process_subcommands_now_P(PSTR("G28 Z\nG0 X80 Y190 F3000\nM117 Print aborted."));

#elif defined(LULZBOT_Juniper_TAZ5)
    #define LULZBOT_AFTER_ABORT_PRINT_ACTION GcodeSuite::process_subcommands_now_P(PSTR("G0 X170 Y290 F3000\nM117 Print aborted."));

#elif defined(LULZBOT_IS_TAZ)
    #define LULZBOT_AFTER_ABORT_PRINT_ACTION GcodeSuite::process_subcommands_now_P(PSTR("G91\nG0 Z15 F600\nG90\nG0 X170 Y290 F3000\nM117 Print aborted."));
#endif

/*********************************** WIPER PAD **********************************/

// Nozzle wiping points (varies by toolhead, as the nozzle position varies)
#if defined(LULZBOT_USE_AUTOLEVELING) && defined(LULZBOT_MINI_BED) && defined(LULZBOT_USE_Z_BELT)
    // Mini has a horizontal wiping pad on the back of the bed
    #define LULZBOT_LEFT_WIPE_X1                       45
    #define LULZBOT_LEFT_WIPE_X2                       115
    #define LULZBOT_LEFT_WIPE_Y1                       174
    #define LULZBOT_LEFT_WIPE_Y2                       174
    #define LULZBOT_LEFT_WIPE_Z                        1

#elif defined(LULZBOT_USE_AUTOLEVELING) && defined(LULZBOT_MINI_BED) && defined(LULZBOT_USE_Z_SCREW)
    // Mini has a horizontal wiping pad on the back of the bed
    #define LULZBOT_LEFT_WIPE_X1                       45
    #define LULZBOT_LEFT_WIPE_X2                       115
    #define LULZBOT_LEFT_WIPE_Y1                       172
    #define LULZBOT_LEFT_WIPE_Y2                       172
    #define LULZBOT_LEFT_WIPE_Z                       -0.5

#elif defined(LULZBOT_USE_AUTOLEVELING) && defined(LULZBOT_TAZ_BED)
    // TAZ has a vertical wiping pad on the left side of the bed
    #define LULZBOT_LEFT_WIPE_X1                      -16
    #define LULZBOT_LEFT_WIPE_X2                      -16
    #define LULZBOT_LEFT_WIPE_Y1                       95
    #define LULZBOT_LEFT_WIPE_Y2                       25
    #define LULZBOT_LEFT_WIPE_Z                        1

    #if defined(LULZBOT_Quiver_TAZPro)
        // The Quiver has an wipe pad on the right side of the bed.
        #define LULZBOT_RIGHT_WIPE_X1                   298
        #define LULZBOT_RIGHT_WIPE_X2                   298
        #define LULZBOT_RIGHT_WIPE_Y1                    95
        #define LULZBOT_RIGHT_WIPE_Y2                    25
        #define LULZBOT_RIGHT_WIPE_Z                      1
    #endif
#endif

#define _LULZBOT_NOZZLE_CLEAN_START_POINT(side) { \
    LULZBOT_ ## side ## _WIPE_X1 + LULZBOT_TOOLHEAD_WIPE_X1_ADJ, \
    LULZBOT_ ## side ## _WIPE_Y1 + LULZBOT_TOOLHEAD_WIPE_Y1_ADJ, \
    LULZBOT_ ## side ## _WIPE_Z \
}

#define _LULZBOT_NOZZLE_CLEAN_END_POINT(side)   { \
    LULZBOT_ ## side ## _WIPE_X2 + LULZBOT_TOOLHEAD_WIPE_X2_ADJ, \
    LULZBOT_ ## side ## _WIPE_Y2 + LULZBOT_TOOLHEAD_WIPE_Y2_ADJ, \
    LULZBOT_ ## side ## _WIPE_Z \
}

#define __LULZBOT_WIPE_GCODE(x,y1,y2,z) \
    "G1 X" #x " Y" #y2 " F5000\n"                 /* Move above second wiper pad */ \
    "G1 Z1\n"                                     /* Push nozzle into wiper */ \
    "M109 R170\n"                                 /* Wait for wipe temp */ \
    "G1 X" #x " Y" #y2 " F4000\n"                 /* Slow wipe */ \
    "G1 X" #x " Y" #y1 " F4000\n"                 /* Slow wipe */ \
    "G1 X" #x " Y" #y2 " F4000\n"                 /* Slow wipe */ \
    "G1 X" #x " Y" #y1 " F4000\n"                 /* Slow wipe */ \
    "G1 X" #x " Y" #y2 " F4000\n"                 /* Slow wipe */ \
    "G1 X" #x " Y" #y1 " F4000\n"                 /* Slow wipe */ \
    "G1 X" #x " Y" #y2 " F4000\n"                 /* Slow wipe */ \
    "G1 X" #x " Y" #y1 " F4000\n"                 /* Slow wipe */ \
    "G1 X" #x " Y" #y2 " F4000\n"                 /* Slow wipe */ \
    "G1 X" #x " Y" #y1 " F4000\n"                 /* Slow wipe */ \
    "G1 X" #x " Y" #y2 " F4000\n"                 /* Slow wipe */ \
    "G1 X" #x " Y" #y1 " F4000\n"                 /* Slow wipe */ \
    "G1 Z25\n"                                    /* Raise nozzle */

#define _LULZBOT_WIPE_GCODE(x,y1,y2,z) __LULZBOT_WIPE_GCODE(x,y1,y2,z)

#define LULZBOT_WIPE_GCODE(side) _LULZBOT_WIPE_GCODE(LULZBOT_ ## side ## _WIPE_X1, \
                                                     LULZBOT_ ## side ## _WIPE_Y1, \
                                                     LULZBOT_ ## side ## _WIPE_Y2, \
                                                     LULZBOT_ ## side ## _WIPE_Z )

#if defined(LULZBOT_Quiver_TAZPro) && LULZBOT_EXTRUDERS == 1
    #define LULZBOT_NOZZLE_CLEAN_START_POINT _LULZBOT_NOZZLE_CLEAN_START_POINT(RIGHT)
    #define LULZBOT_NOZZLE_CLEAN_END_POINT   _LULZBOT_NOZZLE_CLEAN_END_POINT(RIGHT)
#else
    #define LULZBOT_NOZZLE_CLEAN_START_POINT _LULZBOT_NOZZLE_CLEAN_START_POINT(LEFT)
    #define LULZBOT_NOZZLE_CLEAN_END_POINT   _LULZBOT_NOZZLE_CLEAN_END_POINT(LEFT)
#endif

/*************************** REWIPE FUNCTIONALITY *******************************/

#if defined(LULZBOT_USE_AUTOLEVELING)
    //#define LULZBOT_DEBUG_MACROS // Uncomment to debug macro expansions

    #define LULZBOT_G29_RETRY_AND_RECOVER
    #define LULZBOT_G29_MAX_RETRIES      3
    #define LULZBOT_G29_HALT_ON_FAILURE

    #define LULZBOT_NOZZLE_CLEAN_GOBACK_DISABLED

    // Limit on pushing into the bed
    #if defined(LULZBOT_IS_TAZ) && defined(LULZBOT_USE_Z_SCREW)
        #define LULZBOT_Z_PROBE_LOW_POINT    0
    #elif defined(LULZBOT_IS_TAZ) && defined(LULZBOT_USE_Z_BELT)
        #define LULZBOT_Z_PROBE_LOW_POINT   -2
    #elif defined(LULZBOT_IS_MINI)
        #define LULZBOT_Z_PROBE_LOW_POINT   -4
    #endif

    #if LULZBOT_EXTRUDERS == 1
        #define LULZBOT_WIPE_HEAT_TEMP  "M104 S170 T0\n"               /* Preheat to wipe temp */
        #define LULZBOT_WIPE_WAIT_TEMP  "M109 R170 T0\n"               /* Wait for wipe temp */
        #define LULZBOT_WIPE_DONE_TEMP  "M109 R160 T0\n"               /* Drop to probe temp */
    #else
        #define LULZBOT_WIPE_HEAT_TEMP  "M104 S170 T0\nM104 S170 T1\n" /* Preheat to wipe temp */
        #define LULZBOT_WIPE_WAIT_TEMP  "M109 R170 T0\nM109 R170 T1\n" /* Wait for wipe temp */
        #define LULZBOT_WIPE_DONE_TEMP  "M109 R160 T0\nM109 R160 T1\n" /* Wait for probe temp */
    #endif

    #if defined(LULZBOT_Quiver_TAZPro) && LULZBOT_EXTRUDERS == 1
        // When using a single toolhead on Quiver, wipe on the right pad.
        #define LULZBOT_REWIPE_E0 LULZBOT_WIPE_GCODE(RIGHT)            /* Wipe nozzle */
    #else
        #define LULZBOT_REWIPE_E0 "T0\n" LULZBOT_WIPE_GCODE(LEFT)      /* Wipe nozzle */
    #endif

    #if defined(LULZBOT_Quiver_TAZPro) && defined(TOOLHEAD_Quiver_DualExtruder)
        #define LULZBOT_REWIPE_E1 \
            "G0 Z5\n"                             /* Raise nozzle */ \
            "G0 X150 F5000\n"                     /* Move over to switch extruders */ \
            "T1\n"                                /* Switch to second extruder */ \
            LULZBOT_WIPE_GCODE(RIGHT)             /* Wipe on the rightmost pad */ \
            "G0 X150 F5000\n"                     /* Move over to switch extruders */ \
            "T0\n"                                /* Switch to first extruder */
    #else
        #define LULZBOT_REWIPE_E1
    #endif

    #define LULZBOT_WIPE_SEQUENCE_COMMANDS \
        "M117 Hot End Heating...\n"               /* Status message */ \
        LULZBOT_WIPE_HEAT_TEMP                    /* Preheat extruders */ \
        "G28 O1\n"                                /* Home if needed */ \
        LULZBOT_WIPE_WAIT_TEMP                    /* Wait for wipe temp */ \
        "M117 Rewiping nozzle\n"                  /* Status message */ \
        LULZBOT_REWIPE_E0                         /* Wipe first extruder */ \
        LULZBOT_REWIPE_E1                         /* Wipe second extruder */ \
        "M106 S255 \n"                            /* Turn on fan to blow away fuzzies */ \
        "G4 S3\n"                                 /* Wait 3 seconds */ \
        LULZBOT_WIPE_DONE_TEMP                    /* Drop to probe temp */ \
        "M107\n"                                  /* Turn off fan */

    #if defined(LULZBOT_USE_Z_BELT)
        #define LULZBOT_G29_RECOVER_COMMANDS \
            LULZBOT_MENU_AXIS_LEVELING_COMMANDS   /* Level X axis */ \
            "G12\n"                               /* Perform wipe sequence */ \
            "M109 R160\n"                         /* Setting probing temperature */ \
            "M117 Probing bed"                    /* Status message */

    #elif defined(LULZBOT_USE_PRE_GLADIOLA_G29_WORKAROUND)
        #define LULZBOT_G29_RECOVER_COMMANDS \
            "M121\n"                              /* Turn off endstops so we can move */ \
            "G0 Z10\n"                            /* Raise nozzle */ \
            "G28 X0 Y0\n"                         /* G29 on older minis shifts the coordinate system */ \
            "G12\n"                               /* Perform wipe sequence */ \
            "M109 R160\n"                         /* Setting probing temperature */ \
            "M117 Probing bed"                    /* Status message */

    #else
        #define LULZBOT_G29_RECOVER_COMMANDS \
            "G0 Z10\n"                            /* Raise nozzle */ \
            "G12\n"                               /* Perform wipe sequence */ \
            "M109 R160\n"                         /* Set probing temperature */ \
            "M117 Probing bed"                    /* Status message */
    #endif

    #define LULZBOT_G29_FAILURE_COMMANDS \
            "M117 Bed leveling failed.\n"         /* Status message */ \
            "G0 Z10\n"                            /* Raise head */ \
            "G0 E0\n"                             /* Prime filament */ \
            "M300 P25 S880\n"                     /* Play tone */ \
            "M300 P50 S0\n"                       /* Silence */ \
            "M300 P25 S880\n"                     /* Play tone */ \
            "M300 P50 S0\n"                       /* Silence */ \
            "M300 P25 S880\n"                     /* Play tone */ \
            "G4 S1"                               /* Dwell to allow sound to end */

    #define LULZBOT_G29_SUCCESS_COMMANDS \
            "M117 Probe successful\n"             /* Status message */

    #if defined(LULZBOT_DEBUG_MACROS)
        #pragma message("\n\nLULZBOT_WIPE_SEQUENCE_COMMANDS:\n\n" LULZBOT_WIPE_SEQUENCE_COMMANDS "\n\n")
        #pragma message("\n\nLULZBOT_G29_RECOVER_COMMANDS:\n\n"   LULZBOT_G29_RECOVER_COMMANDS "\n\n")
        #pragma message("\n\nLULZBOT_G29_FAILURE_COMMANDS:\n\n"   LULZBOT_G29_FAILURE_COMMANDS "\n\n")
        #error Dump complete
    #endif
#endif

/******************************** PROBE QUALITY CHECK *************************/

#if defined(LULZBOT_USE_AUTOLEVELING)
    #define LULZBOT_BED_LEVELING_DECL vector_3 bp[4];
    #define LULZBOT_BED_LEVELING_POINT(i,x,y,z) bp[i] = vector_3(x,y,z);
    #define LULZBOT_BED_LEVELING_SUMMARY \
        { \
            vector_3 norm = vector_3::cross(bp[0]-bp[1],bp[1]-bp[2]); \
            float a = norm.x, b = norm.y, c = norm.z, d = -bp[0].x*a -bp[0].y*b -bp[0].z*c; \
            float dist = abs(a * bp[3].x + b * bp[3].y + c * bp[3].z + d)/sqrt( a*a + b*b + c*c ); \
            SERIAL_ECHOPAIR("4th probe point, distance from plane: ", dist); \
            SERIAL_EOL(); \
        }
#else
    #define LULZBOT_BED_LEVELING_DECL
    #define LULZBOT_BED_LEVELING_POINT(i,x,y,z)
    #define LULZBOT_BED_LEVELING_SUMMARY
#endif

/****************************** BACKLASH COMPENSATION **************************/

#if defined(LULZBOT_BACKLASH_COMPENSATION)
    #define LULZBOT_BACKLASH_SMOOTHING_MM 3
    #define LULZBOT_BACKLASH_GCODE

    #if defined(LULZBOT_IS_MINI)
        #define LULZBOT_MEASURE_BACKLASH_WHEN_PROBING
    #endif
#endif

/******************************** MOTOR CURRENTS *******************************/

// Values for XYZ vary by printer model, values for E vary by toolhead.

#if defined(LULZBOT_IS_TAZ) && (defined(LULZBOT_USE_Z_BELT) || defined(LULZBOT_USE_ARCHIM2))
    // These values specify the maximum current, but actual
    // currents may be lower when used with COOLCONF
    #define LULZBOT_MOTOR_CURRENT_X              975    // mA
    #define LULZBOT_MOTOR_CURRENT_Y              975    // mA
    #define LULZBOT_MOTOR_CURRENT_Z              975    // mA

#elif defined(LULZBOT_IS_MINI) && (defined(LULZBOT_USE_Z_BELT) || defined(LULZBOT_USE_EINSY_RETRO))
    // These values specify the maximum current, but actual
    // currents may be lower when used with COOLCONF
    #define LULZBOT_MOTOR_CURRENT_X               920    // mA
    #define LULZBOT_MOTOR_CURRENT_Y               920    // mA
    #define LULZBOT_MOTOR_CURRENT_Z               960    // mA

#elif defined(LULZBOT_IS_MINI) && defined(LULZBOT_USE_Z_SCREW)
    #define LULZBOT_MOTOR_CURRENT_XY              1300   // mA
    #define LULZBOT_MOTOR_CURRENT_Z               1630   // mA

#elif defined(LULZBOT_Juniper_TAZ5)
    #define LULZBOT_MOTOR_CURRENT_X               950    // mA
    #define LULZBOT_MOTOR_CURRENT_Y               950    // mA
    #define LULZBOT_MOTOR_CURRENT_Z               1275   // mA

#elif defined(LULZBOT_IS_TAZ) && defined(LULZBOT_USE_Z_SCREW)
    #define LULZBOT_MOTOR_CURRENT_X               950    // mA
    #define LULZBOT_MOTOR_CURRENT_Y               950    // mA
    #define LULZBOT_MOTOR_CURRENT_Z               1075   // mA

#endif

#if defined(LULZBOT_USE_EINSY_RETRO) || defined(LULZBOT_USE_ARCHIM2)
    // Neither define LULZBOT_PWM_MOTOR_CURRENT nor LULZBOT_DIGIPOT_MOTOR_CURRENT,
    // as the current is set in Configuration_adv.h under the HAVE_TMC2130 block

    #if defined(LULZBOT_MOTOR_CURRENT_XY)
        #define LULZBOT_X_CURRENT  LULZBOT_MOTOR_CURRENT_XY
        #define LULZBOT_Y_CURRENT  LULZBOT_MOTOR_CURRENT_XY
    #else
        #define LULZBOT_X_CURRENT  LULZBOT_MOTOR_CURRENT_X
        #define LULZBOT_Y_CURRENT  LULZBOT_MOTOR_CURRENT_Y
    #endif
    #define LULZBOT_Z_CURRENT  LULZBOT_MOTOR_CURRENT_Z
    #if LULZBOT_EXTRUDERS == 1
        #define LULZBOT_E0_CURRENT LULZBOT_MOTOR_CURRENT_E
    #else
        #define LULZBOT_E0_CURRENT LULZBOT_MOTOR_CURRENT_E0
        #define LULZBOT_E1_CURRENT LULZBOT_MOTOR_CURRENT_E1
    #endif

#elif defined(LULZBOT_IS_MINI)
        #define LULZBOT_PWM_MOTOR_CURRENT { \
            LULZBOT_MOTOR_CURRENT_XY, \
            LULZBOT_MOTOR_CURRENT_Z, \
            LULZBOT_MOTOR_CURRENT_E \
        } // Values in milliamps

#elif defined(LULZBOT_IS_TAZ)
    // Values 0-255 (RAMBO 135 = ~0.75A, 185 = ~1A)
    #define DIGIPOT_CURRENT(mA) ((mA-750)/5+135)

    #if LULZBOT_EXTRUDERS == 1
        #define LULZBOT_MOTOR_CURRENT_E0 LULZBOT_MOTOR_CURRENT_E
        #define LULZBOT_MOTOR_CURRENT_E1 LULZBOT_MOTOR_CURRENT_E
    #endif

    #define LULZBOT_DIGIPOT_MOTOR_CURRENT { \
        DIGIPOT_CURRENT(LULZBOT_MOTOR_CURRENT_X), \
        DIGIPOT_CURRENT(LULZBOT_MOTOR_CURRENT_Y), \
        DIGIPOT_CURRENT(LULZBOT_MOTOR_CURRENT_Z), \
        DIGIPOT_CURRENT(LULZBOT_MOTOR_CURRENT_E0), \
        DIGIPOT_CURRENT(LULZBOT_MOTOR_CURRENT_E1) \
    } // Values 0-255 (RAMBO 135 = ~0.75A, 185 = ~1A)
#else
    #error Motor currents not defined
#endif

/************ ACCELERATION, FEEDRATES AND XYZ MOTOR STEPS *******************/

// Values for XYZ vary by printer model, values for E vary by toolhead.

#if defined(LULZBOT_USE_Z_SCREW)
    // Older printers had a fudge factor for ABS shrinkage.
    #define LULZBOT_XY_STEPS                      100.5
#else
    // In the Mini 2 and TAZ Pro going forward, use true XY steps.
    #define LULZBOT_XY_STEPS                      100
#endif

#if defined(LULZBOT_IS_MINI)

    #if defined(LULZBOT_JUNCTION_DEVIATION)
        // Use tuned acceleration factors
        #define LULZBOT_DEFAULT_ACCELERATION          1000
        #define LULZBOT_DEFAULT_TRAVEL_ACCELERATION   1000
    #else
        #define LULZBOT_DEFAULT_ACCELERATION          2000
        #define LULZBOT_DEFAULT_TRAVEL_ACCELERATION   2000
    #endif

    #define LULZBOT_DEFAULT_XJERK                 12.0
    #define LULZBOT_DEFAULT_YJERK                 12.0
    #define LULZBOT_DEFAULT_ZJERK                  0.4

    #if ! defined(LULZBOT_Z_PROBE_OFFSET_FROM_EXTRUDER)
        #if defined(LULZBOT_USE_Z_BELT)
            #define LULZBOT_Z_PROBE_OFFSET_FROM_EXTRUDER  -1.1
        #else
            #define LULZBOT_Z_PROBE_OFFSET_FROM_EXTRUDER  -1.375
        #endif
    #endif

#elif defined(LULZBOT_IS_TAZ)
    #define LULZBOT_DEFAULT_XJERK                 8.0
    #define LULZBOT_DEFAULT_YJERK                 8.0
    #define LULZBOT_DEFAULT_ZJERK                 0.4
    #if ! defined(LULZBOT_DEFAULT_ACCELERATION)
        #define LULZBOT_DEFAULT_ACCELERATION      500
    #endif
    #if ! defined(LULZBOT_DEFAULT_TRAVEL_ACCELERATION)
        #define LULZBOT_DEFAULT_TRAVEL_ACCELERATION 500
    #endif

    #if !defined(LULZBOT_Z_PROBE_OFFSET_FROM_EXTRUDER)
        #define LULZBOT_Z_PROBE_OFFSET_FROM_EXTRUDER -1.200
    #endif
#endif

#if defined(LULZBOT_IS_MINI) && defined(LULZBOT_USE_Z_SCREW)
    #define LULZBOT_Z_STEPS                       1600
    #define LULZBOT_Z_MICROSTEPS                  16

    #define LULZBOT_DEFAULT_MAX_FEEDRATE          {300, 300, 8, 40}      // (mm/sec)
    #define LULZBOT_DEFAULT_MAX_ACCELERATION      {9000,9000,100,1000}

#elif defined(LULZBOT_IS_MINI) && defined(LULZBOT_USE_Z_BELT)
    #define LULZBOT_Z_STEPS                       200
    #define LULZBOT_Z_MICROSTEPS                  32
    #define LULZBOT_DEFAULT_MAX_FEEDRATE          {300, 300, 300, 40}    // (mm/sec)
    #if defined(LULZBOT_JUNCTION_DEVIATION)
        // Use tuned acceleration factors
        #define LULZBOT_DEFAULT_MAX_ACCELERATION  {1000,1000,200,1000}
    #else
        #define LULZBOT_DEFAULT_MAX_ACCELERATION  {9000,9000,200,1000}
    #endif

#elif defined(LULZBOT_IS_TAZ) && defined(LULZBOT_USE_Z_SCREW)
    #if LULZBOT_EXTRUDERS > 1 && defined(LULZBOT_DISTINCT_E_FACTORS)
        #define LULZBOT_DEFAULT_MAX_FEEDRATE          {300, 300, 6, 25, 25}      // (mm/sec) value of 6 assumes upgraded z motors otherwise use 3
        #define LULZBOT_DEFAULT_MAX_ACCELERATION      {3000,3000,100,1000,1000} //tuned values
    #else
        #define LULZBOT_DEFAULT_MAX_FEEDRATE          {300, 300, 3, 25}      // (mm/sec)
        #define LULZBOT_DEFAULT_MAX_ACCELERATION      {9000,9000,100,9000}
    #endif
    #define LULZBOT_Z_STEPS                       1601.27 //tuned value - orignial is 1600
    #define LULZBOT_Z_MICROSTEPS                  16

#elif defined(LULZBOT_IS_TAZ) && defined(LULZBOT_USE_Z_BELT)
    #define LULZBOT_DEFAULT_MAX_FEEDRATE          {300, 300, 30, 25}     // (mm/sec)
    #define LULZBOT_DEFAULT_MAX_ACCELERATION      {9000,9000,200,9000}
    #define LULZBOT_Z_STEPS                       500
    #define LULZBOT_Z_MICROSTEPS                  16
#endif

#if defined(LULZBOT_USE_REPRAP_LCD_DISPLAY)
    #if defined(LULZBOT_USE_Z_BELT)
        #define LULZBOT_MANUAL_FEEDRATE_Z 40
    #else
        #define LULZBOT_MANUAL_FEEDRATE_Z  4
    #endif
    #define LULZBOT_MANUAL_FEEDRATE {50*60, 50*60, LULZBOT_MANUAL_FEEDRATE_Z*60, LULZBOT_MANUAL_FEEDRATE_E*60} // (mm/min)
#endif

#if defined(LULZBOT_USE_TOUCH_UI)
    #define LULZBOT_MAX_MANUAL_FEEDRATE {300*60, 300*60, 300*60, LULZBOT_MANUAL_FEEDRATE_E*60} // (mm/min)
#endif

#if defined(LULZBOT_DISTINCT_E_FACTORS) && LULZBOT_EXTRUDERS == 2 && !defined(LULZBOT_SWITCHING_EXTRUDER)
    #define LULZBOT_DEFAULT_AXIS_STEPS_PER_UNIT   {LULZBOT_XY_STEPS,LULZBOT_XY_STEPS,LULZBOT_Z_STEPS,LULZBOT_E_STEPS,LULZBOT_E_STEPS}
#else
    #define LULZBOT_DEFAULT_AXIS_STEPS_PER_UNIT   {LULZBOT_XY_STEPS,LULZBOT_XY_STEPS,LULZBOT_Z_STEPS,LULZBOT_E_STEPS}
#endif

#if defined(LULZBOT_USE_ARCHIM2)
    #define LULZBOT_MIN_STEPS_PER_SEGMENT 1
#else
    #define LULZBOT_MIN_STEPS_PER_SEGMENT 6
#endif

/*********************************** LCD OPTIONS *******************************/

#if defined(LULZBOT_USE_REPRAP_LCD_DISPLAY)
    #define LULZBOT_REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER
    #define LULZBOT_SDSUPPORT
    #define LULZBOT_XYZ_HOLLOW_FRAME_DISABLE
    #define LULZBOT_MENU_HOLLOW_FRAME_DISABLE
    #define LULZBOT_USE_SMALL_INFOFONT
    #if defined(LULZBOT_USE_AUTOLEVELING)
        #define LULZBOT_MENU_BED_LEVELING_GCODE "G28 XY\nM109 S175\nG28 Z\nM109 R145\nG12\nG29\nM104 S0"
    #endif
    #define LULZBOT_SHOW_CUSTOM_BOOTSCREEN
    #define LULZBOT_ENCODER_PULSES_PER_STEP 2
    #define LULZBOT_ENCODER_STEPS_PER_MENU_ITEM 1
    #define LULZBOT_COOLING_MESSAGES
    #if defined(LULZBOT_IS_MINI) || defined(LULZBOT_USE_ARCHIM2)
        // In the experimental Gladiola_MiniLCD, the encoder direction is reversed.
        #define LULZBOT_REVERSE_ENCODER_DIRECTION
    #endif
    /* Marlin shows three extruders on a dual:
     *   Extruder    - The active nozzle (varies)
     *   Extruder 1  - The primary extruder
     *   Extruder 2  - The secondary extruder
     *
     * The following causes the active nozzle to be
     * hidden as seeing three nozzles may be
     * confusing to users.
     */
    #define LULZBOT_HIDE_ACTIVE_NOZZLE_IN_LCD
    #define LULZBOT_HIDE_PID_CONFIG_IN_LCD
    #define LULZBOT_HIDE_EXTRA_FAN_CONFIG_IN_LCD
    #define LULZBOT_HIDE_PREHEAT_CHOICES
    #define LULZBOT_HIDE_INITIALIZE_EEPROM
    #define LULZBOT_ABOUT_FIRMWARE_MENU
    #define LULZBOT_NO_BED_LEVELING_IN_LCD
    #define LULZBOT_PRECISION_ESTEP
    #if defined(LULZBOT_USE_Z_SCREW)
        #define LULZBOT_PRECISION_XYZ_STEPS float62
    #else
        #define LULZBOT_PRECISION_XYZ_STEPS float3
    #endif
    #define LULZBOT_PRECISION_E_STEPS float3
    #define LULZBOT_PRECISION_ZOFFSET ftostr52
    #define LULZBOT_LCD_SET_PROGRESS_MANUALLY
    #define LULZBOT_SCROLL_LONG_FILENAMES
    #define LULZBOT_BABYSTEP_ZPROBE_GFX_OVERLAY
    #define LULZBOT_DISABLE_KILL_BUTTON
    #define LULZBOT_EXTRUDER_STR "Extruder"
    #define LULZBOT_RESET_SELECTION_TO_FIRST_ON_MENU_BACK
    #define LULZBOT_CPU_ST7920_DELAY_2 300 // Increase delay from 250 to 300 to avoid display lock ups
    #define MSG_MOVE_E _UxGT("Extruder ") // Add space to extruder string
    #define MSG_MOTION _UxGT("Movement") // Motion -> Movement

    // Indirection to allow TYPE to be a #defined variable
    #define LULZ_MULTIPLIER_ITEM_EDIT_CALLBACK(TYPE, LABEL, ...) MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(TYPE, LABEL, __VA_ARGS__)
#else
    #define LULZBOT_PRECISION_XYZ_STEPS float62
    #define LULZBOT_PRECISION_E_STEPS   float62
#endif

#if defined(LULZBOT_LIGHTWEIGHT_UI)
    #define WELCOME_MSG _UxGT(LULZBOT_LCD_MACHINE_NAME " ready.")
    #define LULZBOT_STATUS_EXPIRE_SECONDS 0
#else
    #define WELCOME_MSG _UxGT("LulzBot " LULZBOT_LCD_MACHINE_NAME " ready.")
#endif

#if defined(LULZBOT_USE_TOUCH_UI)
    #define LULZBOT_EXTENSIBLE_UI
    #define LULZBOT_DISABLE_SD_DETECT_INVERTED
    #undef  LULZBOT_REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER
    #define LULZBOT_DISABLE_DUE_SD_MMC
    #define LULZBOT_LCD_SET_PROGRESS_MANUALLY
    #define LULZBOT_COOLING_MESSAGES
    #define LULZBOT_EXTRUDER_STR "Hot End"
    #define LULZBOT_NO_CONFIRM_REHEAT_AFTER_PAUSING
#endif

#if defined(LULZBOT_SDSUPPORT_DEBUG)
    #define LULZBOT_SDCARD_CHECK_BYTE(n) \
        static bool spi_error = false; \
        if(n != -1 && !isprint(n) && n != '\n' && n != '\r') spi_error = true;

    #define LULZBOT_SDCARD_COMMAND_DONE(cmd) \
        if(spi_error) { \
            SERIAL_ECHOLNPAIR("Likely SPI read error: ", cmd); \
            spi_error = false; \
        }
#else
    #define LULZBOT_SDCARD_CHECK_BYTE(n)
    #define LULZBOT_SDCARD_COMMAND_DONE(cmd)
#endif

#if defined(LULZBOT_USB_FLASH_DRIVE_SUPPORT)
    #define LULZBOT_SDSUPPORT
    #define MSG_SD_INSERTED "Media inserted"
    #define MSG_SD_REMOVED  "Media removed"
#endif

#if defined(LULZBOT_USE_AUTOLEVELING) && (defined(LULZBOT_USE_REPRAP_LCD_DISPLAY) || defined(LULZBOT_USE_TOUCH_UI))
    #define LULZBOT_BABYSTEPPING
    #define LULZBOT_BABYSTEP_ZPROBE_OFFSET
    #define LULZBOT_BABYSTEP_XY
    #if LULZBOT_EXTRUDERS > 1
        #define LULZBOT_BABYSTEP_HOTEND_Z_OFFSET
    #endif
    #if defined(LULZBOT_USE_ARCHIM2)
        #define LULZBOT_BABYSTEP_IN_PLANNER
    #endif
#endif

/***************************** CUSTOM SPLASH SCREEN *****************************/

#define LULZBOT_CUSTOM_BOOTSCREEN \
    void draw_custom_bootscreen(const u8g_pgm_uint8_t * const bmp, const bool erase=true) { \
        u8g.firstPage(); \
        do { \
            u8g.drawBitmapP(0,0,CEILING(CUSTOM_BOOTSCREEN_BMPWIDTH, 8),CUSTOM_BOOTSCREEN_BMPHEIGHT,custom_start_bmp); \
            u8g.setFont(u8g_font_6x13); \
            u8g.drawStr(57,17,LULZBOT_LCD_MACHINE_NAME); \
            u8g.setFont(u8g_font_04b_03); \
            u8g.drawStr(58,28,LULZBOT_LCD_TOOLHEAD_NAME); \
            u8g.setFont(u8g_font_5x8); \
            u8g.drawStr(59,41,"LulzBot.com"); \
            u8g.setFont(u8g_font_5x8); \
            u8g.drawStr(61,62,"v"); \
            u8g.drawStr(66,62,SHORT_BUILD_VERSION LULZBOT_FW_VERSION); \
        } while( u8g.nextPage() ); \
        safe_delay(CUSTOM_BOOTSCREEN_TIMEOUT); \
        u8g.setFont(MENU_FONT_NAME); \
    }

/*************************** Z-OFFSET AUTO-SAVE  ********************************/

/* Historically, the Lulzbot firmware would save the Z-Offset into the EEPROM
 * each time it is changed. The latest Marlin made this more difficult since they
 * added a CRC to the EEPROM. We work around this by bracketing the EEPROM_READ
 * and EEPROM_WRITE routines such that the CRC ignores the Z-Offset value. That
 * code also captures the eeprom_index so we can write only that value later.
 */

/* The following goes in "configuration_store.cpp", before and after
 * "EEPROM_WRITE(zprobe_zoffset)" and "EEPROM_READ(zprobe_zoffset)"
 */
#if defined(LULZBOT_USE_AUTOLEVELING) && defined(LULZBOT_USE_REPRAP_LCD_DISPLAY)
    #define LULZBOT_EEPROM_BEFORE_ZOFFSET \
        const uint16_t eeprom_saved_crc = working_crc; \
        eeprom_zoffset_index = eeprom_index;

    #define LULZBOT_EEPROM_AFTER_ZOFFSET \
        working_crc = eeprom_saved_crc;

    /* The following goes in "configuration_store.h" */
    #define LULZBOT_SAVE_ZOFFSET_TO_EEPROM_DECL \
        static int eeprom_zoffset_index; \
        static void store_zoffset();

    /* The following goes in "configuration_store.cpp" */
    #define LULZBOT_SAVE_ZOFFSET_TO_EEPROM_IMPL \
        int MarlinSettings::eeprom_zoffset_index = -1; \
        void MarlinSettings::store_zoffset() { \
            if(eeprom_zoffset_index > 0) { \
                uint16_t working_crc; \
                int eeprom_index = eeprom_zoffset_index; \
                EEPROM_WRITE(zprobe_zoffset); \
                SERIAL_ECHO_START(); \
                SERIAL_ECHOPAIR("Updating zoffset in EEPROM: ", zprobe_zoffset); \
                SERIAL_ECHOPAIR("; EEPROM Index: ", eeprom_zoffset_index); \
                SERIAL_ECHOLNPGM(""); \
            } \
        }

    /* The following goes in "ultralcd.cpp" in "lcd_babystep_zoffset" */
    #define LULZBOT_SAVE_ZOFFSET_TO_EEPROM settings.store_zoffset();
#else
    #define LULZBOT_EEPROM_BEFORE_ZOFFSET
    #define LULZBOT_EEPROM_AFTER_ZOFFSET
    #define LULZBOT_SAVE_ZOFFSET_TO_EEPROM_DECL
    #define LULZBOT_SAVE_ZOFFSET_TO_EEPROM_IMPL
    #define LULZBOT_SAVE_ZOFFSET_TO_EEPROM
#endif

#endif /* CONDITIONALS_LULZBOT */
