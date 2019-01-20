#ifndef CONDITIONALS_LULZBOT
#define CONDITIONALS_LULZBOT

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
 */

#define LULZBOT_FW_VERSION ".34" // Change this with each update

#if ( \
    !defined(LULZBOT_Gladiola_Mini) && \
    !defined(LULZBOT_Gladiola_MiniLCD) && \
    !defined(LULZBOT_Juniper_TAZ5) && \
    !defined(LULZBOT_Oliveoil_TAZ6) && \
    !defined(LULZBOT_Hibiscus_Mini2) \
) || ( \
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
    !defined(TOOLHEAD_Angelfish_Aerostruder) \
)
    #error Must specify model and toolhead. Please see "Configuration_LulzBot.h" for directions.
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
    #define LULZBOT_USE_Z_BACKLASH_COMPENSATION
    #define LULZBOT_BAUDRATE 250000
    #define LULZBOT_PRINTCOUNTER
    #define LULZBOT_UUID "351487b6-ca9a-4c1a-8765-d668b1da6585"
#endif

#if defined(LULZBOT_Gladiola_MiniLCD)
    #define LULZBOT_CUSTOM_MACHINE_NAME "LulzBot Mini LCD"
    #define LULZBOT_LCD_MACHINE_NAME "Mini"
    #define LULZBOT_IS_MINI
    #define LULZBOT_MINI_BED
    #define LULZBOT_USE_LCD_DISPLAY
    #define LULZBOT_USE_AUTOLEVELING
    #define LULZBOT_USE_MIN_ENDSTOPS
    #define LULZBOT_USE_MAX_ENDSTOPS
    #define LULZBOT_USE_NORMALLY_OPEN_ENDSTOPS
    #define LULZBOT_ENDSTOPS_ALWAYS_ON_DEFAULT
    #define LULZBOT_USE_Z_SCREW
    #define LULZBOT_USE_Z_BACKLASH_COMPENSATION
    #define LULZBOT_BAUDRATE 250000
    #define LULZBOT_PRINTCOUNTER
    #define LULZBOT_UUID "083f68f1-028e-494c-98e1-f2e0dfaee9a5"
#endif

#if defined(LULZBOT_Juniper_TAZ5)
    #define LULZBOT_CUSTOM_MACHINE_NAME "LulzBot TAZ 5"
    #define LULZBOT_LCD_MACHINE_NAME "TAZ 5"
    #define LULZBOT_IS_TAZ
    #define LULZBOT_TAZ_BED
    #define LULZBOT_USE_LCD_DISPLAY
    #define LULZBOT_USE_MIN_ENDSTOPS
    #define LULZBOT_USE_NORMALLY_OPEN_ENDSTOPS
    #define LULZBOT_USE_Z_SCREW
    #define LULZBOT_USE_Z_BACKLASH_COMPENSATION
    #define LULZBOT_BAUDRATE 250000
    #define LULZBOT_PRINTCOUNTER
    #define LULZBOT_UUID "c3255c96-4097-4884-8ed0-ded2ff9bae61"
#endif

#if defined(LULZBOT_Oliveoil_TAZ6)
    #define LULZBOT_CUSTOM_MACHINE_NAME "LulzBot TAZ 6"
    #define LULZBOT_LCD_MACHINE_NAME "TAZ 6"
    #define LULZBOT_IS_TAZ
    #define LULZBOT_TAZ_BED
    #define LULZBOT_USE_LCD_DISPLAY
    #define LULZBOT_USE_AUTOLEVELING
    #define LULZBOT_USE_MIN_ENDSTOPS
    #define LULZBOT_USE_MAX_ENDSTOPS
    #define LULZBOT_USE_HOME_BUTTON
    #define LULZBOT_USE_NORMALLY_CLOSED_ENDSTOPS
    #define LULZBOT_ENDSTOPS_ALWAYS_ON_DEFAULT
    #define LULZBOT_USE_Z_SCREW
    #define LULZBOT_USE_Z_BACKLASH_COMPENSATION
    #define LULZBOT_BAUDRATE 250000
    #define LULZBOT_PRINTCOUNTER
    #define LULZBOT_UUID "845f003c-aebd-4e53-a6b9-7d0984fde609"
#endif

#if defined(LULZBOT_Hibiscus_Mini2)
    #define LULZBOT_CUSTOM_MACHINE_NAME "LulzBot Hibiscus"
    #define LULZBOT_LCD_MACHINE_NAME "Mini 2"
    #define LULZBOT_IS_MINI
    #define LULZBOT_MINI_BED
    #define LULZBOT_USE_EINSY_RETRO
    #define LULZBOT_USE_LCD_DISPLAY
    #define LULZBOT_TWO_PIECE_BED
    #define LULZBOT_USE_AUTOLEVELING
    #define LULZBOT_SENSORLESS_HOMING
    #define LULZBOT_USE_NORMALLY_CLOSED_ENDSTOPS
    #define LULZBOT_USE_TMC_STEALTHCHOP_Z
    #define LULZBOT_USE_Z_BELT
    #define LULZBOT_USE_Z_BACKLASH_COMPENSATION
    #define LULZBOT_BAUDRATE 250000
    #define LULZBOT_PRINTCOUNTER
    #define LULZBOT_USE_32_MICROSTEPS_ON_Z
    #define LULZBOT_UUID "e5502411-d46d-421d-ba3a-a20126d7930f"
    #define LULZBOT_LIGHTWEIGHT_UI
#endif

/****************************** DEBUGGING OPTIONS *******************************/

//#define LULZBOT_TMC_SHOW_CURRENT_ADJUSTMENTS
//#define LULZBOT_MONITOR_TMC_TEMPERATURE

/**************************** GENERAL CONFIGURATION *****************************/

#define LULZBOT_STRING_CONFIG_H_AUTHOR "(Aleph Objects Inc., LulzBot Git Repository)"
#define LULZBOT_EEPROM_SETTINGS
#define LULZBOT_EMERGENCY_PARSER
#define LULZBOT_NOZZLE_PARK_FEATURE
#define LULZBOT_PAUSE_PARK_NOZZLE_TIMEOUT 300
#define LULZBOT_AUTO_REPORT_TEMPERATURES
#define LULZBOT_ADVANCED_OK
#define LULZBOT_TX_BUFFER_SIZE 32
#define LULZBOT_BUFSIZE 5
#define LULZBOT_CLARIFY_ERROR_MESSAGES
#define LULZBOT_PRINTJOB_TIMER_AUTOSTART_DISABLED

// Marlin 1.1.4 has changed the behavior of G92 so that
// it changes software endstops, making it less useful
// for making position adjustments after hitting an
// endstop. We need the old behavior of G92 for the
// Yellowfin start GCODE and it is also a useful
// feature for custom height adjustments (something
// requested in the forums). The following restores
// the old behavior.

#define LULZBOT_G92_BACKWARDS_COMPATIBILITY

// Prior branches of the LulzBot firmware used G26
// to reset a probe failed condition. Marlin upstrem
// now redefines that for unified bed leveling. The
// following maps G26 to M999 for equivalent behavior,
// so long as UBL is disabled.
#define LULZBOT_G26_BACKWARDS_COMPATIBILITY

// The following should be kept more or less like M999
#define LULZBOT_G26_RESET_ACTION \
  Running = true; \
  lcd_reset_alert_level();

// Q&A wants to be able to use M226 on endstops switches
#define LULZBOT_NO_PIN_PROTECTION_ON_M226

// Often Marlin shows the "Home XYZ first" message, but
// never clears that message. The following causes
// Marlin to print a new message when the axis are homed
#define LULZBOT_HOMING_MESSAGE_WORKAROUND

/************************* EXPERIMENTAL FEATURES ******************************/

#if defined(LULZBOT_USE_EXPERIMENTAL_FEATURES)
#endif

/**************************** LINEAR ADVANCE **********************************/

// Enable linear advance, but leave K at zero so
// it is not used unless the user requests it.
#define LULZBOT_LIN_ADVANCE
#define LULZBOT_LIN_ADVANCE_K 0

/******************** MOTHERBOARD AND PIN CONFIGURATION ***********************/

// Whether endstops are inverting
#define LULZBOT_NORMALLY_CLOSED_ENDSTOP       false
#define LULZBOT_NORMALLY_OPEN_ENDSTOP         true
#define LULZBOT_NO_ENDSTOP                    true

#if defined(LULZBOT_IS_MINI) && defined(LULZBOT_USE_EINSY_RETRO)
    // Experimental Mini retrofitted with EinsyRambo from UltiMachine
    #define LULZBOT_MOTHERBOARD                   BOARD_EINSY_RETRO
    #define LULZBOT_CONTROLLER_FAN_PIN            FAN1_PIN  // Digital pin 6

#elif defined(LULZBOT_IS_MINI)
    #define LULZBOT_MOTHERBOARD                   BOARD_MINIRAMBO
    #define LULZBOT_CONTROLLER_FAN_PIN            FAN1_PIN  // Digital pin 6

#elif defined(LULZBOT_IS_TAZ)
    #define LULZBOT_MOTHERBOARD                   BOARD_RAMBO
    #define LULZBOT_CONTROLLER_FAN_PIN            FAN2_PIN  // Digital pin 2
#endif

#if defined(LULZBOT_USE_HOME_BUTTON)
    #define LULZBOT_Z_MIN_PROBE_ENDSTOP
    #define LULZBOT_Z_MIN_PROBE_PIN               SERVO0_PIN // Digital pin 22
    // On the TAZ 6, the bed washers are on Z_MIN_PROBE while the
    // Z-Home button is on Z_MIN, yet we need both to be disabled
    // when z_probe_enabled is false. We added this special case
    // to "endstops.cpp"
    #define LULZBOT_Z_MIN_USES_Z_PROBE_ENABLED

#else
    // The Mini 2 lacks a home button and probes using the Z_MIN pin.
    #define LULZBOT_Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN
#endif

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
    #define LULZBOT_QUICKHOME

#elif defined(LULZBOT_Juniper_TAZ5)
    #define LULZBOT_X_HOME_DIR             -1 // Home left
    #define LULZBOT_Y_HOME_DIR             -1 // Home bed rear
    #define LULZBOT_Z_HOME_DIR             -1 // Home towards bed
    #define LULZBOT_QUICKHOME

#elif defined(LULZBOT_IS_TAZ) && !defined(LULZBOT_USE_HOME_BUTTON)
    #define LULZBOT_X_HOME_DIR             -1 // Home left
    #define LULZBOT_Y_HOME_DIR              1 // Home bed forward
    #define LULZBOT_Z_HOME_DIR              1 // Home to top
    #define LULZBOT_QUICKHOME

#elif defined(LULZBOT_IS_TAZ) &&  defined(LULZBOT_USE_HOME_BUTTON)
    #define LULZBOT_X_HOME_DIR             -1 // Home left
    #define LULZBOT_Y_HOME_DIR              1 // Home bed forward
    #define LULZBOT_Z_HOME_DIR             -1 // Home towards bed
    #define LULZBOT_QUICKHOME
#endif

#if defined(LULZBOT_MINI_BED) && defined(LULZBOT_USE_Z_BELT)
    #define LULZBOT_HOMING_FEEDRATE_XY             (50*60) // mm/m
    #define LULZBOT_HOMING_FEEDRATE_Z              (40*60) // mm/m

#elif defined(LULZBOT_MINI_BED) && defined(LULZBOT_USE_Z_SCREW)
    #define LULZBOT_HOMING_FEEDRATE_XY            (30*60) // mm/m
    #define LULZBOT_HOMING_FEEDRATE_Z              (8*60) // mm/m

#elif defined(LULZBOT_TAZ_BED) && defined(LULZBOT_USE_Z_BELT)
    #define LULZBOT_HOMING_FEEDRATE_XY            (50*60) // mm/m
    #define LULZBOT_HOMING_FEEDRATE_Z             (10*60) // mm/m

#elif defined(LULZBOT_TAZ_BED) && defined(LULZBOT_USE_Z_SCREW)
    #define LULZBOT_HOMING_FEEDRATE_XY            (50*60) // mm/m
    #define LULZBOT_HOMING_FEEDRATE_Z             (3*60)  // mm/m

#endif // LULZBOT_TAZ_BED

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
                SYNC_PLAN_POSITION_KINEMATIC(); \
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
            const bool saved_endstop_state = Endstops::enabled_globally; \
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

#if !defined(LULZBOT_USE_MAX_ENDSTOPS)
    #define LULZBOT_HOME_AFTER_DEACTIVATE
#endif

#if defined(LULZBOT_USE_Z_BELT)
    #define LULZBOT_DISABLE_INACTIVE_Z false
    #define LULZBOT_SD_FINISHED_STEPPERRELEASE false
    #define LULZBOT_SD_FINISHED_RELEASECOMMAND "M84 X Y E"
    #define LULZBOT_ENABLE_Z_MOTOR_ON_STARTUP enable_Z();
#else
    #define LULZBOT_DISABLE_INACTIVE_Z true
    #define LULZBOT_SD_FINISHED_STEPPERRELEASE true
    #define LULZBOT_SD_FINISHED_RELEASECOMMAND "M84 X Y Z E"
    #define LULZBOT_ENABLE_Z_MOTOR_ON_STARTUP
#endif

/*********************** AUTOLEVELING / BED PROBE *******************************/

#if defined(LULZBOT_USE_AUTOLEVELING) && defined(LULZBOT_MINI_BED) && defined(LULZBOT_USE_Z_BELT)
    #define LULZBOT_LEFT_PROBE_BED_POSITION        -3
    #define LULZBOT_RIGHT_PROBE_BED_POSITION      163
    #define LULZBOT_BACK_PROBE_BED_POSITION       168
    #define LULZBOT_FRONT_PROBE_BED_POSITION       -4

#elif defined(LULZBOT_USE_AUTOLEVELING) && defined(LULZBOT_MINI_BED)
    // In order to work with the Gladiola printers, we need to
    // perform the probe right against the left and front endstops.
    // This is extremely problematic and leads to other problems
    // which are corrected for in the start GCODE.
    #define LULZBOT_LEFT_PROBE_BED_POSITION      LULZBOT_X_MIN_POS
    #define LULZBOT_FRONT_PROBE_BED_POSITION     LULZBOT_Y_MIN_POS

    // The Gladiola has the probe points spaced further apart than
    // earlier models. Since Gladiola FW has to work on earlier
    // printers, we need to add a workaround because G29 hits the
    // endstops and shifts the coordinate system around.
    #define LULZBOT_RIGHT_PROBE_BED_POSITION     161
    #define LULZBOT_BACK_PROBE_BED_POSITION      161
    #define LULZBOT_USE_PRE_GLADIOLA_G29_WORKAROUND

#elif defined(LULZBOT_USE_AUTOLEVELING) && defined(LULZBOT_TAZ_BED)
    #define LULZBOT_LEFT_PROBE_BED_POSITION       -9
    #define LULZBOT_RIGHT_PROBE_BED_POSITION     288
    #define LULZBOT_BACK_PROBE_BED_POSITION      289
    #define LULZBOT_FRONT_PROBE_BED_POSITION      -9
#endif

#if defined(LULZBOT_USE_AUTOLEVELING)
    #define LULZBOT_RESTORE_LEVELING_AFTER_G28
    #define LULZBOT_NOZZLE_CLEAN_FEATURE
    // Select type of leveling to use:
    //#define LULZBOT_AUTO_BED_LEVELING_BILINEAR
    #define LULZBOT_AUTO_BED_LEVELING_LINEAR
    //#define LULZBOT_AUTO_BED_LEVELING_3POINT
#endif

#if defined(LULZBOT_AUTO_BED_LEVELING_3POINT)
  // Experimental three point leveling.
  #define LULZBOT_PROBE_PT_1_X LULZBOT_LEFT_PROBE_BED_POSITION
  #define LULZBOT_PROBE_PT_1_Y LULZBOT_FRONT_PROBE_BED_POSITION
  #define LULZBOT_PROBE_PT_2_X LULZBOT_RIGHT_PROBE_BED_POSITION
  #define LULZBOT_PROBE_PT_2_Y LULZBOT_FRONT_PROBE_BED_POSITION
  #define LULZBOT_PROBE_PT_3_X LULZBOT_RIGHT_PROBE_BED_POSITION
  #define LULZBOT_PROBE_PT_3_Y LULZBOT_BACK_PROBE_BED_POSITION
#elif defined(LULZBOT_AUTO_BED_LEVELING_LINEAR) || defined(LULZBOT_AUTO_BED_LEVELING_BILINEAR)
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
#define LULZBOT_Z_PROBE_OFFSET_RANGE_MIN      -2
#define LULZBOT_Z_PROBE_OFFSET_RANGE_MAX      5
#define LULZBOT_XY_PROBE_SPEED                6000
#define LULZBOT_Z_PROBE_SPEED_SLOW           (1*60)
#if defined(LULZBOT_USE_Z_BELT)
    #define LULZBOT_Z_PROBE_SPEED_FAST       (20*60)
#else
    #define LULZBOT_Z_PROBE_SPEED_FAST       (8*60)
#endif
#define LULZBOT_Z_CLEARANCE_DEPLOY_PROBE_WORKAROUND
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

/* Enable the probe pins only only when homing/probing,
 * as this helps reduce EMI by grounding the lines.
 *
 * On Mini:
 *   Z_MIN_PIN are the bed washers.
 *
 * On TAZ:
 *   Z_MIN_PIN corresponds to the Z-Home push button.
 *   Z_MIN_PROBE_PIN are the bed washers.
 *
 * On TAZ or Mini with TMC2130 and Sensorless Homing on Z:
 *   Z_MIN_PIN corresponds to stallguard diag pin
 *   Z_MIN_PROBE_PIN are the bed washers.
 */
#define LULZBOT_SET_PIN_STATE(pin, enable) \
    if(enable) { \
        /* Set as inputs with pull-up resistor */ \
        SET_INPUT(pin); \
        WRITE(pin, HIGH); \
    } else { \
        /* Ground to prevent EMI */ \
        SET_OUTPUT(pin); \
        WRITE(pin, LOW); \
    }

#if defined(LULZBOT_USE_AUTOLEVELING) && defined(LULZBOT_MINI_BED)
    #define LULZBOT_ENABLE_PROBE_PINS(enable) { \
        endstops.enable_z_probe(enable); \
        LULZBOT_SET_PIN_STATE(Z_MIN_PIN, enable) \
        LULZBOT_EXTRUDER_MOTOR_SHUTOFF_ON_PROBE(enable) \
    }

#elif defined(LULZBOT_USE_AUTOLEVELING) && defined(LULZBOT_USE_HOME_BUTTON)
    #define LULZBOT_ENABLE_PROBE_PINS(enable) { \
        endstops.enable_z_probe(enable); \
        LULZBOT_SET_PIN_STATE(Z_MIN_PIN, enable) \
        LULZBOT_SET_PIN_STATE(LULZBOT_Z_MIN_PROBE_PIN, enable) \
    }
#else
    #define LULZBOT_ENABLE_PROBE_PINS(enable)
#endif

/* Make it so M42 S<state> controls the state of the
/* probe lines. This is useful for troubleshooting. */
#define LULZBOT_M42_TOGGLES_PROBE_PINS \
    if (!parser.seenval('P')) { \
      LULZBOT_ENABLE_PROBE_PINS(pin_status); \
      return; \
    }

#if defined(LULZBOT_USE_Z_BELT)
    #define LULZBOT_MENU_AXIS_LEVELING_GCODE \
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
        "M400\n"                 /* Finish moves */ \
        "G28\n"                  /* Rehome */ \
        "M117 Leveling done.\n"  /* Set LCD status */
#endif

#if defined(LULZBOT_USE_Z_SCREW)
    // The older Minis seem succeptible to noise in the probe lines.
    // This restores the sampling of endstops as it existed in previous
    // version of Marlin.
    #define LULZBOT_ENDSTOP_NOISE_FILTER
    #define LULZBOT_ENDSTOP_NOISE_FILTER_SAMPLES 2
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
        defined(TOOLHEAD_CecropiaSilk_SingleExtruderAeroV2) || \
        defined(TOOLHEAD_AchemonSphinx_SmallLayer) || \
        defined(TOOLHEAD_BandedTiger_HardenedSteel) || \
        defined(TOOLHEAD_DingyCutworm_HardenedSteelPlus)
        #define LULZBOT_MOTOR_CURRENT_E          960 // mA
    #else
        #define LULZBOT_MOTOR_CURRENT_E         1250 // mA
    #endif
#endif /* TOOLHEAD_Gladiola_SingleExtruder || TOOLHEAD_Albatross_Flexystruder || TOOLHEAD_Finch_Aerostruder */

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

/******************************** Mini 2 / TAZ 7 TOOLHEADS *********************/

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
#if defined(LULZBOT_MAY_USE_V2_ADAPTER) && defined(LULZBOT_USE_Z_SCREW)
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

/******************************** TAZ TOOLHEADS ********************************/

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
//          16 chars max                            ^^^^^^^^^^^^^^^
    #define LULZBOT_M115_EXTRUDER_TYPE         "SingleExtruder"
    #define LULZBOT_MOTOR_CURRENT_E            750 // mA
    #define LULZBOT_X_MAX_ENDSTOP_INVERTING    LULZBOT_NORMALLY_CLOSED_ENDSTOP
    #define LULZBOT_AO_Hexagon
    #define LULZBOT_E_STEPS                    830
#endif /* TOOLHEAD_Tilapia_SingleExtruder */

#if defined(TOOLHEAD_Angelfish_Aerostruder)
    // Prototype Aero for TAZ
    #define LULZBOT_LCD_TOOLHEAD_NAME          "Aerostruder"
//          16 chars max                       ^^^^^^^^^^^^^^^
    #define LULZBOT_M115_EXTRUDER_TYPE         "Aerostruder"
    #define LULZBOT_MOTOR_CURRENT_E            875 // mA
    #define LULZBOT_X_MAX_ENDSTOP_INVERTING    LULZBOT_NORMALLY_CLOSED_ENDSTOP
    #define LULZBOT_E3D_Titan_Aero_V6
    #define LULZBOT_E_STEPS                    420
#endif /* TOOLHEAD_Angelfish_Aerostruder */

#if defined(TOOLHEAD_Kanyu_Flexystruder)
    #define LULZBOT_LCD_TOOLHEAD_NAME              "Flexystruder"
//          16 chars max                            ^^^^^^^^^^^^^^^
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
    #define LULZBOT_LCD_TOOLHEAD_NAME              "MOARstruder"
//          16 chars max                            ^^^^^^^^^^^^^^^
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
    #define LULZBOT_TOOLHEAD_WIPE_X1_ADJ       0
    #define LULZBOT_TOOLHEAD_WIPE_X2_ADJ       0
    #define LULZBOT_TOOLHEAD_WIPE_Y1_ADJ       0
    #define LULZBOT_TOOLHEAD_WIPE_Y2_ADJ      48
    #define LULZBOT_X_MAX_ENDSTOP_INVERTING    LULZBOT_NORMALLY_OPEN_ENDSTOP
    #define LULZBOT_E_STEPS                    830
    #define LULZBOT_AO_Hexagon
#endif /* TOOLHEAD_Javelin_DualExtruderV2 */

#if defined(TOOLHEAD_Longfin_FlexyDually)
    #define LULZBOT_LCD_TOOLHEAD_NAME              "FlexyDually v2"
//          16 chars max                            ^^^^^^^^^^^^^^^
    #define LULZBOT_M115_EXTRUDER_TYPE         "FlexyDually v2"
    #define LULZBOT_TOOLHEAD_WIPE_X1_ADJ       0
    #define LULZBOT_TOOLHEAD_WIPE_X2_ADJ       0
    #define LULZBOT_TOOLHEAD_WIPE_Y1_ADJ       0
    #define LULZBOT_TOOLHEAD_WIPE_Y2_ADJ      48
    #define LULZBOT_X_MAX_ENDSTOP_INVERTING    LULZBOT_NORMALLY_OPEN_ENDSTOP
    #define LULZBOT_E_STEPS                  830
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
    #define LULZBOT_TOOLHEAD_X_MIN_ADJ             1
    #define LULZBOT_TOOLHEAD_X_MAX_ADJ            -7
    #define LULZBOT_TOOLHEAD_Y_MIN_ADJ            -7
    #define LULZBOT_TOOLHEAD_Y_MAX_ADJ            -7
    #define LULZBOT_TOOLHEAD_WIPE_X1_ADJ          -2
    #define LULZBOT_TOOLHEAD_WIPE_X2_ADJ          -2
    #define LULZBOT_TOOLHEAD_WIPE_Y1_ADJ           0
    #define LULZBOT_TOOLHEAD_WIPE_Y2_ADJ           0
    #undef  LULZBOT_Z_HOMING_HEIGHT
    #define LULZBOT_Z_HOMING_HEIGHT                10
    #undef  LULZBOT_Z_CLEARANCE_DEPLOY_PROBE
    #define LULZBOT_Z_CLEARANCE_DEPLOY_PROBE       10
    #undef  LULZBOT_Z_CLEARANCE_BETWEEN_PROBES
    #define LULZBOT_Z_CLEARANCE_BETWEEN_PROBES     10
    // Move the rear homing position back to avoid the Z homing adaptor
    #undef  LULZBOT_BACK_PROBE_BED_POSITION
    #define LULZBOT_BACK_PROBE_BED_POSITION       291
    // Adjust so left nozzle probes on the left washers;
    // right nozzles on the right nozzle.
    #undef  LULZBOT_LEFT_PROBE_BED_POSITION
    #define LULZBOT_LEFT_PROBE_BED_POSITION        -3
    #undef  LULZBOT_RIGHT_PROBE_BED_POSITION
    #define LULZBOT_RIGHT_PROBE_BED_POSITION      282
    #define LULZBOT_X_MAX_ENDSTOP_INVERTING       LULZBOT_NORMALLY_CLOSED_ENDSTOP
    #define LULZBOT_SWAP_EXTRUDERS
    #undef  LULZBOT_INVERT_E1_DIR
    #define LULZBOT_INVERT_E1_DIR                 false
    #define LULZBOT_E_STEPS                        760
    #define LULZBOT_E3D_SOMEstruder_x2
#endif /* TOOLHEAD_Yellowfin_DualExtruderV3 */

/************************ CUSTOMIZE VERSION STRING ***************************/

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

/*************************** TEMPERATURE SETTINGS *****************************/

#define LULZBOT_TEMP_SENSOR_0                 5
#define LULZBOT_TEMP_SENSOR_BED               7
#define LULZBOT_TEMP_RESIDENCY_TIME           1
#define LULZBOT_TEMP_HYSTERESIS               10
#define LULZBOT_TEMP_WINDOW                   10

#define LULZBOT_TEMP_BED_RESIDENCY_TIME       1
#define LULZBOT_TEMP_BED_HYSTERESIS           5
#define LULZBOT_TEMP_BED_WINDOW               5

#define LULZBOT_HEATER_MAXTEMP                305
#define LULZBOT_EXTRUDE_MINTEMP               120

#define LULZBOT_THERMAL_PROTECTION_PERIOD     15     // Seconds
#define LULZBOT_THERMAL_PROTECTION_HYSTERESIS 30     // Degrees Celsius

#define LULZBOT_THERMAL_PROTECTION_BED_PERIOD     15     // Seconds
#define LULZBOT_THERMAL_PROTECTION_BED_HYSTERESIS 10     // Degrees Celsius

#define LULZBOT_PREHEAT_1_TEMP_HOTEND 200            // PLA

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

// Modular two piece bed (TAZ 7+)
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

/****************************** FAN CONFIGURATION ******************************/

// For the Pelonis C4010L24BPLB1b-7 fan, we need a relative low
// PWM frequency of about 122Hz in order for the fan to turn.
// By default, FAST_PWM_FAN appears to PWM at ~31kHz, but if we
// set a prescale of 4, it divides this by 256 to get us down to
// the frequency we need.

#define LULZBOT_FAST_PWM_FAN
#define LULZBOT_FAST_PWM_SCALE                    4

#define LULZBOT_FAN_KICKSTART_TIME              100
#define LULZBOT_FAN_MIN_PWM                      70

#define LULZBOT_USE_CONTROLLER_FAN
#if defined(LULZBOT_IS_MINI) && defined(LULZBOT_USE_EINSY_RETRO)
    // The TMC drivers need a bit more cooling.
    #define LULZBOT_CONTROLLERFAN_SPEED                    255
    #define LULZBOT_CONTROLLERFAN_SPEED_WHEN_ONLY_Z_ACTIVE 120
#elif defined(LULZBOT_IS_MINI)
    // The Mini fan runs rather loud at full speed.
    #define LULZBOT_CONTROLLERFAN_SPEED         120
#elif defined(LULZBOT_IS_TAZ)
    #define LULZBOT_CONTROLLERFAN_SPEED         255
#endif

// As of Marlin 1.1.5, FAST_PWM_FAN adjusts the frequencies for
// all fans except the controller fan. This workaround allows
// the controller fan PWM freq to be adjusted to 122Hz (this
// may not be necessary, but since the Pelonis fan likes 122Hz,
// we are trying to keep all the fan frequencies at that).
#define LULZBOT_FAST_PWM_CONTROLLER_FAN_WORKAROUND

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

    #define LULZBOT_STANDARD_X_BED_SIZE         155
    #define LULZBOT_STANDARD_Y_BED_SIZE         155

#elif defined(LULZBOT_IS_MINI) && defined(LULZBOT_USE_Z_BELT)
    #define LULZBOT_STANDARD_X_MAX_POS         163
    #define LULZBOT_STANDARD_X_MIN_POS          -3
    #define LULZBOT_STANDARD_Y_MAX_POS         192
    #define LULZBOT_STANDARD_Y_MIN_POS          -5

    #define LULZBOT_STANDARD_X_BED_SIZE         157
    #define LULZBOT_STANDARD_Y_BED_SIZE         157

#elif defined(LULZBOT_Juniper_TAZ5)
    #define LULZBOT_STANDARD_X_MAX_POS         298
    #define LULZBOT_STANDARD_X_MIN_POS           0
    #define LULZBOT_STANDARD_Y_MAX_POS         276
    #define LULZBOT_STANDARD_Y_MIN_POS           0

    #define LULZBOT_STANDARD_X_BED_SIZE         288
    #define LULZBOT_STANDARD_Y_BED_SIZE         275

#elif defined(LULZBOT_IS_TAZ) && defined(LULZBOT_USE_Z_BELT)
    #define LULZBOT_STANDARD_X_MAX_POS         300
    #define LULZBOT_STANDARD_X_MIN_POS         -16
    #define LULZBOT_STANDARD_Y_MAX_POS         303
    #define LULZBOT_STANDARD_Y_MIN_POS         -20

    #define LULZBOT_STANDARD_X_BED_SIZE         280
    #define LULZBOT_STANDARD_Y_BED_SIZE         280

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
    #define LULZBOT_STANDARD_Z_MIN_POS           0
    #define LULZBOT_STANDARD_Z_MAX_POS         299
#endif

#define LULZBOT_X_MAX_POS  (LULZBOT_STANDARD_X_MAX_POS + LULZBOT_TOOLHEAD_X_MAX_ADJ)
#define LULZBOT_X_MIN_POS  (LULZBOT_STANDARD_X_MIN_POS + LULZBOT_TOOLHEAD_X_MIN_ADJ)
#define LULZBOT_Y_MAX_POS  (LULZBOT_STANDARD_Y_MAX_POS + LULZBOT_TOOLHEAD_Y_MAX_ADJ)
#define LULZBOT_Y_MIN_POS  (LULZBOT_STANDARD_Y_MIN_POS + LULZBOT_TOOLHEAD_Y_MIN_ADJ)
#define LULZBOT_Z_MAX_POS  (LULZBOT_STANDARD_Z_MAX_POS + LULZBOT_TOOLHEAD_Z_MAX_ADJ)
#define LULZBOT_Z_MIN_POS  (LULZBOT_STANDARD_Z_MIN_POS + LULZBOT_TOOLHEAD_Z_MIN_ADJ)

#define LULZBOT_X_BED_SIZE min(LULZBOT_X_MAX_POS, LULZBOT_STANDARD_X_BED_SIZE)
#define LULZBOT_Y_BED_SIZE min(LULZBOT_Y_MAX_POS, LULZBOT_STANDARD_Y_BED_SIZE)

/**************************** ENDSTOP CONFIGURATION ****************************/

#if defined(LULZBOT_USE_MIN_ENDSTOPS)
    #define LULZBOT_USE_XMIN_PLUG
    #define LULZBOT_USE_YMIN_PLUG
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

// The RAMBO does not support interrupts on all pins
// so leave the ENDSTOP_INTERRUPTS_FEATURE disabled

//#define LULZBOT_ENDSTOP_INTERRUPTS_FEATURE

/* Endstop settings are determined by printer model, except for the
 * X_MAX which varies by toolhead. */

#if defined(LULZBOT_USE_NORMALLY_CLOSED_ENDSTOPS)
    // TAZ 6+ and Hibiscus Mini onwards use normally closed endstops.
    // This is safer, as a loose connector or broken wire will halt
    // the axis
    #define LULZBOT_X_MIN_ENDSTOP_INVERTING       LULZBOT_NORMALLY_CLOSED_ENDSTOP
    #define LULZBOT_Y_MIN_ENDSTOP_INVERTING       LULZBOT_NORMALLY_CLOSED_ENDSTOP


    // LULZBOT_X_MAX_ENDSTOP_INVERTING varies by toolhead
    #define LULZBOT_Y_MAX_ENDSTOP_INVERTING       LULZBOT_NORMALLY_CLOSED_ENDSTOP
    #define LULZBOT_Z_MAX_ENDSTOP_INVERTING       LULZBOT_NORMALLY_CLOSED_ENDSTOP

#elif defined(LULZBOT_USE_NORMALLY_OPEN_ENDSTOPS)
    #define LULZBOT_X_MIN_ENDSTOP_INVERTING       LULZBOT_NORMALLY_OPEN_ENDSTOP
    #define LULZBOT_Y_MIN_ENDSTOP_INVERTING       LULZBOT_NORMALLY_OPEN_ENDSTOP

    // LULZBOT_X_MAX_ENDSTOP_INVERTING varies by toolhead
    #define LULZBOT_Y_MAX_ENDSTOP_INVERTING       LULZBOT_NORMALLY_OPEN_ENDSTOP
    #define LULZBOT_Z_MAX_ENDSTOP_INVERTING       LULZBOT_NORMALLY_OPEN_ENDSTOP
#endif

// Electrical probing pins are always open until contact is made
#define LULZBOT_Z_MIN_ENDSTOP_INVERTING       LULZBOT_NORMALLY_OPEN_ENDSTOP
#define LULZBOT_Z_MIN_PROBE_ENDSTOP_INVERTING LULZBOT_NORMALLY_OPEN_ENDSTOP

/******************************* MOTOR DRIVER TYPE ******************************/

#if defined(LULZBOT_USE_EINSY_RETRO) || defined(LULZBOT_USE_ARCHIM2)
    #define LULZBOT_X_DRIVER_TYPE  TMC2130
    #define LULZBOT_Y_DRIVER_TYPE  TMC2130
    #define LULZBOT_Z_DRIVER_TYPE  TMC2130
    #define LULZBOT_E0_DRIVER_TYPE TMC2130
    #if LULZBOT_EXTRUDERS > 1
        #define LULZBOT_E1_DRIVER_TYPE TMC2130
    #else
        #define LULZBOT_E1_DRIVER_TYPE A4988
    #endif

    #define LULZBOT_TMC_DEBUG
#else
    #define LULZBOT_X_DRIVER_TYPE  A4988
    #define LULZBOT_Y_DRIVER_TYPE  A4988
    #define LULZBOT_Z_DRIVER_TYPE  A4988
    #define LULZBOT_E0_DRIVER_TYPE A4988
    #define LULZBOT_E1_DRIVER_TYPE A4988
#endif

/******************************* SENSORLESS HOMING ******************************/

#if defined(LULZBOT_SENSORLESS_HOMING)
    #define LULZBOT_X_HOMING_SENSITIVITY 4
    #define LULZBOT_Y_HOMING_SENSITIVITY 4
    #define LULZBOT_Z_HOMING_SENSITIVITY_DISABLED
#endif

#if defined(LULZBOT_SENSORLESS_HOMING)
    #define LULZBOT_X_HOME_BUMP_MM                0
    #define LULZBOT_Y_HOME_BUMP_MM                0
#else
    #define LULZBOT_X_HOME_BUMP_MM                5
    #define LULZBOT_Y_HOME_BUMP_MM                5
#endif

#define LULZBOT_Z_HOME_BUMP_MM                2

#if defined(LULZBOT_USE_EINSY_RETRO)
    #define LULZBOT_HAVE_TMC2130
    #define LULZBOT_R_SENSE         0.12
    #define LULZBOT_HOLD_MULTIPLIER 0.5

    // According to Jason at UltiMachine, setting the lower the
    // stealth freq the cooler the motor drivers will operate.
    #define LULZBOT_STEALTH_FREQ 0

    // If true, use STEALTHCHOP, otherwise use COOLSTEP
    #if defined(LULZBOT_USE_TMC_STEALTHCHOP_XY) || defined(LULZBOT_USE_TMC_STEALTHCHOP_Z)
        #define LULZBOT_STEALTHCHOP LULZBOT_USE_TMC_STEALTHCHOP
        #define LULZBOT_HYBRID_THRESHOLD
    #endif

    #define LULZBOT_Y_HYBRID_THRESHOLD 72
    #define LULZBOT_X_HYBRID_THRESHOLD 72

    #define LULZBOT_TMC_INIT(st) \
        /* The EinsyRambo connects both diag pins to the same */ \
        /* microcontroller pin and provides a pull up resistor, */ \
        /* so configure the pin as active low. */ \
        st.diag0_active_high(0); \
        st.diag1_active_high(0); \
        st.diag1_stall(1); \
        /* Reverse the motor direction so it matches the Rambo */ \
        st.shaft_dir(1); \
        st.external_ref(0);     /* I_scale_analog  = 0 */ \
        st.internal_sense_R(0); /* internal_Rsense = 0 */ \

    #define LULZBOT_SIGN_EXTEND_SGT(sgt) int8_t(sgt | ((sgt << 1) & 0x80))

    #define TMC_CS_TO_mA(cs,vsense) (float(cs)+1)/32 * (vsense?0.180:0.325)/(LULZBOT_R_SENSE+0.02) * 1/sqrt(2) * 1000

    #define LULZBOT_ENABLE_COOLSTEP_WITH_STALLGUARD(st) \
        /* Disable steathchop */ \
        st.stealthChop(0); \
        /* Enable coolstep for all velocities */ \
        st.coolstep_min_speed(1024UL * 1024UL - 1UL); \
        st.sg_min(1); \
        st.sg_max(3);

    #define LULZBOT_ENABLE_STEALTHCHOP(st) \
        /* Enable steathchop */ \
        st.stealthChop(1); \
        /* Disable coolstep */ \
        st.coolstep_min_speed(0); \
        st.sg_min(0); \
        st.sg_max(0);

    /* X axis driver temperature tuning notes over a 10 minute print:
     *  - TOFF caused the greatest effect on driver temperature (~40C) and needs
     *    to be at 1 to keep the drivers from overheating (was tested at 1-3)
     *  - TBL can be either 0, 1 or 2, but 3 will cause overheating and 1 appears
     *    to run the coolest.
     *  - Setting HSTRT higher than 5 cause the drivers to warm up.
     *  - Setting HSTRT lower than 3 causes the motor to stall.
     *  - Setting HEND higher than 1 causes drivers to overheat.
     */

    #define LULZBOT_XY_TOFF           1
    #define LULZBOT_XY_HSTRT          4
    #define LULZBOT_XY_HEND           0
    #define LULZBOT_XY_TBL            1

    #define LULZBOT_MOTOR_INIT_XY \
        stepperX.tbl(LULZBOT_XY_TBL);         /* TBL    = [0..3]   */ \
        stepperY.tbl(LULZBOT_XY_TBL);         /* TBL    = [0..3]   */ \
        stepperX.toff(LULZBOT_XY_TOFF);       /* TOFF   = [1..15]  */ \
        stepperY.toff(LULZBOT_XY_TOFF);       /* TOFF   = [1..15]  */ \
        stepperX.hstrt(LULZBOT_XY_HSTRT);     /* HSTART = [0..7]   */ \
        stepperY.hstrt(LULZBOT_XY_HSTRT);     /* HSTART = [0..7]   */ \
        stepperX.hend(LULZBOT_XY_HEND);       /* HEND   = [0..15]  */ \
        stepperY.hend(LULZBOT_XY_HEND);       /* HEND   = [0..15]  */

    /* Once homing is finished, return to the normal operating mode: */
    /* Either stealthchop or coolstep, as previously configured */
    #if defined(LULZBOT_USE_TMC_STEALTHCHOP_Z)
        #define LULZBOT_DEFAULT_OPERATING_MODE_Z(st) LULZBOT_ENABLE_STEALTHCHOP(st)
    #else
        #define LULZBOT_DEFAULT_OPERATING_MODE_Z(st) LULZBOT_ENABLE_COOLSTEP_WITH_STALLGUARD(st)
    #endif

    #define LULZBOT_Z_TOFF           1
    #define LULZBOT_Z_HSTRT          0
    #define LULZBOT_Z_HEND           0
    #define LULZBOT_Z_TBL            1

    #define LULZBOT_MOTOR_INIT_Z \
        /* Set TOFF to reduce audible chopping noise */ \
        stepperZ.toff(LULZBOT_Z_TOFF);       /* TOFF   = [1..15]  */ \
        stepperZ.hstrt(LULZBOT_Z_HSTRT);     /* HSTART = [0..7]   */ \
        stepperZ.hend(LULZBOT_Z_HEND);       /* HEND   = [0..15]  */ \
        stepperZ.tbl(LULZBOT_Z_TBL);         /* TBL    = [0..3]   */ \
        LULZBOT_DEFAULT_OPERATING_MODE_Z(stepperZ);

    #define LULZBOT_E_TOFF           1
    #define LULZBOT_E_HSTRT          0
    #define LULZBOT_E_HEND           0
    #define LULZBOT_E_TBL            1

    #define LULZBOT_MOTOR_INIT_E \
        /* Set TOFF to reduce audible chopping noise */ \
        stepperZ.toff(LULZBOT_E_TOFF);       /* TOFF   = [1..15]  */ \
        stepperZ.hstrt(LULZBOT_E_HSTRT);     /* HSTART = [0..7]   */ \
        stepperZ.hend(LULZBOT_E_HEND);       /* HEND   = [0..15]  */ \
        stepperZ.tbl(LULZBOT_E_TBL);         /* TBL    = [0..3]   */ \
        /* Always use COOLSTEP on E0 */ \
        LULZBOT_ENABLE_COOLSTEP_WITH_STALLGUARD(stepperE0); \

    #define LULZBOT_TMC_ADV { \
            LULZBOT_MOTOR_INIT_XY \
            LULZBOT_MOTOR_INIT_Z \
            LULZBOT_MOTOR_INIT_E \
        }

    /* Once homing is finished, return to the normal operating mode: */
    /* Either stealthchop or coolstep, as previously configured */
    #if defined(LULZBOT_USE_TMC_STEALTHCHOP_XY)
        #define LULZBOT_DEFAULT_OPERATING_MODE_XY(st) LULZBOT_ENABLE_STEALTHCHOP(st)
    #else
        #define LULZBOT_DEFAULT_OPERATING_MODE_XY(st) LULZBOT_ENABLE_COOLSTEP_WITH_STALLGUARD(st)
    #endif

    /* Sensorless homing requires stallguard, which is not available with
     * stealhchop, so switch to COOLSTEP prior to doing a homing move. */
    #define LULZBOT_SENSORLESS_HOMING_TOGGLE(st, enable) \
        if (enable) { \
            /* Sometimes the X axis refuses to move at the start of G28, */ \
            /* because the stallguard is triggered. Toggling in and out */ \
            /* of STEALHCHOP mode seems to resolve this. */ \
            LULZBOT_ENABLE_STEALTHCHOP(st) \
            LULZBOT_ENABLE_COOLSTEP_WITH_STALLGUARD(st) \
        } else { \
            LULZBOT_DEFAULT_OPERATING_MODE_XY(st) \
        }
#endif

#if defined(LULZBOT_SENSORLESS_HOMING)
    #define LULZBOT_USE_XMIN_PLUG // Uses Stallguard
    //#define LULZBOT_USE_XMAX_PLUG // Uses Stallguard
    //#define LULZBOT_USE_YMIN_PLUG // Uses Stallguard
    #define LULZBOT_USE_YMAX_PLUG // Uses Stallguard
    #define LULZBOT_USE_ZMIN_PLUG
    #define LULZBOT_USE_ZMAX_PLUG

    #define LULZBOT_X_MIN_ENDSTOP_INVERTING   LULZBOT_NORMALLY_OPEN_ENDSTOP
    //#define LULZBOT_X_MAX_ENDSTOP_INVERTING LULZBOT_NORMALLY_OPEN_ENDSTOP
    #define LULZBOT_Y_MAX_ENDSTOP_INVERTING   LULZBOT_NORMALLY_OPEN_ENDSTOP
    //#define LULZBOT_Y_MIN_ENDSTOP_INVERTING LULZBOT_NORMALLY_OPEN_ENDSTOP

    // The following does not seem to work when both
    // MAX and MIN are using Stallguard.
    // It also appears that when this is enabled
    // stallguard is never cleared.
    //#define LULZBOT_ENDSTOP_INTERRUPTS_FEATURE

    // Quickhome does not work with sensorless homing
    #undef LULZBOT_QUICKHOME
#endif

/**************************** ADVANCED PAUSE FEATURE ****************************/

#if defined(LULZBOT_USE_LCD_DISPLAY)
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
    #define LULZBOT_EXTRUDER_STR "Extruder"

    // The following shows up both while pausing and changing filament, better to not be specific.
    #define MSG_FILAMENT_CHANGE_RESUME_1      _UxGT("")
    #define MSG_FILAMENT_CHANGE_RESUME_2      _UxGT("Please wait.")
    #define LULZBOT_FILAMENT_CHANGE_MSG_BACK  MSG_MAIN

    // The following shows up even when no print is happening, better to leave blank.
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

    // Hide the status message after a resume
    #define LULZBOT_PAUSED_MESSAGE_WORKAROUND
#endif

#if defined(LULZBOT_IS_MINI)
    #define LULZBOT_NOZZLE_PARK_POINT {  10, (LULZBOT_Y_MAX_POS - 10), 20 }
#else
    // Match the purge location of the v3 dual so a single tray can be used.
    #define LULZBOT_NOZZLE_PARK_POINT { 100, LULZBOT_Y_MAX_POS-1, 20 }
#endif

#define LULZBOT_ACTION_ON_PAUSE_AND_RESUME

#if defined(LULZBOT_IS_MINI)
    #define LULZBOT_AFTER_ABORT_PRINT_ACTION execute_commands_immediate_P(PSTR("G28 Z\nG0 X80 Y190 F3000\nM117 Print aborted."));

#elif defined(LULZBOT_Juniper_TAZ5)
    #define LULZBOT_AFTER_ABORT_PRINT_ACTION execute_commands_immediate_P(PSTR("G0 X170 Y290 F3000\nM117 Print aborted."));

#elif defined(LULZBOT_IS_TAZ)
    #define LULZBOT_AFTER_ABORT_PRINT_ACTION execute_commands_immediate_P(PSTR("G91\nG0 Z15 F600\nG90\nG0 X170 Y290 F3000\nM117 Print aborted."));
#endif

/*********************************** WIPER PAD **********************************/

// Nozzle wiping points (varies by toolhead, as the nozzle position varies)
#if defined(LULZBOT_USE_AUTOLEVELING) && defined(LULZBOT_MINI_BED) && defined(LULZBOT_USE_Z_BELT)
    // Mini has a horizontal wiping pad on the back of the bed
    #define LULZBOT_STANDARD_WIPE_X1                       45
    #define LULZBOT_STANDARD_WIPE_X2                       115
    #define LULZBOT_STANDARD_WIPE_Y1                       176
    #define LULZBOT_STANDARD_WIPE_Y2                       176
    #define LULZBOT_STANDARD_WIPE_Z                        1

#elif defined(LULZBOT_USE_AUTOLEVELING) && defined(LULZBOT_MINI_BED) && defined(LULZBOT_USE_Z_SCREW)
    // Mini has a horizontal wiping pad on the back of the bed
    #define LULZBOT_STANDARD_WIPE_X1                       45
    #define LULZBOT_STANDARD_WIPE_X2                       115
    #define LULZBOT_STANDARD_WIPE_Y1                       172
    #define LULZBOT_STANDARD_WIPE_Y2                       172
    #define LULZBOT_STANDARD_WIPE_Z                       -0.5

#elif defined(LULZBOT_USE_AUTOLEVELING) && defined(LULZBOT_TAZ_BED)
    // TAZ has a vertical wiping pad on the left side of the bed
    #define LULZBOT_STANDARD_WIPE_X1                      -16
    #define LULZBOT_STANDARD_WIPE_X2                      -16
    #define LULZBOT_STANDARD_WIPE_Y1                       95
    #define LULZBOT_STANDARD_WIPE_Y2                       25
    #define LULZBOT_STANDARD_WIPE_Z                        1
#endif

#define LULZBOT_NOZZLE_CLEAN_START_POINT { \
    LULZBOT_STANDARD_WIPE_X1 + LULZBOT_TOOLHEAD_WIPE_X1_ADJ, \
    LULZBOT_STANDARD_WIPE_Y1 + LULZBOT_TOOLHEAD_WIPE_Y1_ADJ, \
    LULZBOT_STANDARD_WIPE_Z \
}

#define LULZBOT_NOZZLE_CLEAN_END_POINT   { \
    LULZBOT_STANDARD_WIPE_X2 + LULZBOT_TOOLHEAD_WIPE_X2_ADJ, \
    LULZBOT_STANDARD_WIPE_Y2 + LULZBOT_TOOLHEAD_WIPE_Y2_ADJ, \
    LULZBOT_STANDARD_WIPE_Z \
}

/*************************** REWIPE FUNCTIONALITY *******************************/

#if defined(LULZBOT_USE_AUTOLEVELING)
    #define LULZBOT_NUM_REWIPES      3

    #if defined(LULZBOT_IS_TAZ)
        #define LULZBOT_Z_PROBE_LOW_POINT    0 // Limit on pushing into the bed
    #else defined(LULZBOT_IS_MINI)
        #define LULZBOT_Z_PROBE_LOW_POINT   -4 // Limit on pushing into the bed
    #endif

    #define LULZBOT_WIPE_SEQUENCE_GCODE \
        "M109 R170\n"                         /* Wait for wipe temp */ \
        "M117 Rewiping nozzle\n"              /* Status message */ \
        "G12 P0 S12 T0\n"                     /* Wipe nozzle */ \
        "M104 S160\n"                         /* Drop to probe temp */

    #if defined(LULZBOT_USE_Z_BELT)
        #define LULZBOT_REWIPE_RECOVER_GCODE \
            "M104 S170\n"                         /* Begin heating to wipe temp */ \
            LULZBOT_MENU_AXIS_LEVELING_GCODE      /* Level X axis */ \
            LULZBOT_WIPE_SEQUENCE_GCODE           /* Perform wipe sequence */ \
            "M117 Probing bed"                    /* Status message */

    #elif defined(LULZBOT_USE_PRE_GLADIOLA_G29_WORKAROUND)
        #define LULZBOT_REWIPE_RECOVER_GCODE \
            "M104 S170\n"                         /* Begin heating to wipe temp */ \
            "M117 Rewiping nozzle\n"              /* Status message */ \
            "M121\n"                              /* Turn off endstops so we can move */ \
            "G0 Z10\n"                            /* Raise nozzle */ \
            "G28 X0 Y0\n"                         /* G29 on older minis shifts the coordinate system */ \
            LULZBOT_WIPE_SEQUENCE_GCODE           /* Perform wipe sequence */ \
            "M120\n"                              /* Restore endstops */ \
            "M117 Probing bed"                    /* Status message */

    #else
        #define LULZBOT_REWIPE_RECOVER_GCODE \
            "M109 R170\n"                         /* Setting wipe temperature and wait */ \
            "M117 Rewiping nozzle\n"              /* Status message */ \
            "G0 Z10\n"                            /* Raise nozzle */ \
            LULZBOT_WIPE_SEQUENCE_GCODE           /* Perform wipe sequence */ \
            "M117 Probing bed"                    /* Status message */
    #endif

    #define LULZBOT_REWIPE_FAILED_GCODE \
        "M117 Bed leveling failed.\n"             /* Status message */ \
        "G0 Z10\n"                                /* Raise head */ \
        "G0 E0\n"                                 /* Prime filament */ \
        "M300 P25 S880\n"                         /* Play tone */ \
        "M300 P50 S0\n"                           /* Silence */ \
        "M300 P25 S880\n"                         /* Play tone */ \
        "M300 P50 S0\n"                           /* Silence */ \
        "M300 P25 S880\n"                         /* Play tone */ \
        "G4 S1"                                   /* Dwell to allow sound to end */

    #define LULZBOT_REWIPE_SUCCESS_GCODE \
        "M117 Probe successful\n"                 /* Status message */

    #define LULZBOT_DO_PROBE_MOVE(speed) if (do_probe_move(LULZBOT_Z_PROBE_LOW_POINT, MMM_TO_MMS(speed))) return NAN;

    #define LULZBOT_G29_WITH_RETRY_DECL \
        void gcode_G29_with_retry();

    #define LULZBOT_G29_WITH_RETRY_IMPL \
        void gcode_G29_with_retry() { \
            set_bed_leveling_enabled(false); \
            for(uint8_t i = 0; i < LULZBOT_NUM_REWIPES; i++) { \
                LULZBOT_BACKLASH_MEASUREMENT_START \
                LULZBOT_ENABLE_PROBE_PINS(true); \
                gcode_G29(); \
                LULZBOT_ENABLE_PROBE_PINS(false); \
                if(planner.leveling_active) break; \
                execute_commands_immediate_P(PSTR(LULZBOT_REWIPE_RECOVER_GCODE)); \
            } \
            if(planner.leveling_active) { \
                execute_commands_immediate_P(PSTR(LULZBOT_REWIPE_SUCCESS_GCODE)); \
            } else { \
                SERIAL_ERRORLNPGM("PROBE FAIL CLEAN NOZZLE"); \
                execute_commands_immediate_P(PSTR(LULZBOT_REWIPE_FAILED_GCODE)); \
                kill(PSTR(MSG_ERR_PROBING_FAILED)); \
            } \
        }

    #define LULZBOT_G29_COMMAND gcode_G29_with_retry();
#else
    #define LULZBOT_G29_WITH_RETRY_DECL
    #define LULZBOT_G29_WITH_RETRY_IMPL
#endif

/******************************* EXECUTE IMMEDIATE *************************/

#define LULZBOT_EXECUTE_IMMEDIATE_DECL \
    void execute_commands_immediate_P(const char *pgcode);

#define LULZBOT_EXECUTE_IMMEDIATE_IMPL \
    void execute_commands_immediate_P(const char *pgcode) { \
        /* Save the parser state */ \
        char *saved_cmd = parser.command_ptr; \
        /* Process individual commands in string */ \
        while(pgm_read_byte_near(pgcode) != '\0') { \
            /* Break up string at '\n' delimiters */ \
            const char *delim = strchr_P(pgcode, '\n'); \
            size_t len = delim ? delim - pgcode : strlen_P(pgcode); \
            char cmd[len+1]; \
            strncpy_P(cmd, pgcode, len); \
            cmd[len] = '\0'; \
            pgcode += len; \
            if(delim) pgcode++; \
            /* Parse the next command in the string */ \
            parser.parse(cmd); \
            process_parsed_command(false); \
        } \
        /* Restore the parser state */ \
        parser.parse(saved_cmd); \
    }

/******************************** PROBE QUALITY CHECK *************************/

#if defined(LULZBOT_USE_AUTOLEVELING)
    #define LULZBOT_BED_LEVELING_DECL vector_3 bp[4];
    #define LULZBOT_BED_LEVELING_POINT(i,x,y,z) bp[i] = vector_3(x,y,z);
    #define LULZBOT_BED_LEVELING_SUMMARY \
        { \
            vector_3 norm = vector_3::cross(bp[0]-bp[1],bp[1]-bp[2]); \
            float a = norm.x, b = norm.y, c = norm.z, d = -bp[0].x*a -bp[0].y*b -bp[0].z*c; \
            float dist = abs(a * bp[3].x + b * bp[3].y + c * bp[3].z + d)/sqrt( a*a + b*b + c*c ); \
            SERIAL_PROTOCOLLNPAIR("4th probe point, distance from plane: ", dist); \
            SERIAL_EOL(); \
        }
#else
    #define LULZBOT_BED_LEVELING_DECL
    #define LULZBOT_BED_LEVELING_POINT(i,x,y,z)
    #define LULZBOT_BED_LEVELING_SUMMARY
#endif

/****************************** BACKLASH COMPENSATION **************************/

#if defined(LULZBOT_USE_Z_BACKLASH_COMPENSATION)

    #define LULZBOT_BACKLASH_MEASUREMENT_RESOLUTION 0.005
    #define LULZBOT_BACKLASH_MEASUREMENT_LIMIT      0.5
    #define LULZBOT_BACKLASH_SMOOTHING_DISTANCE     3

    #if ENABLED(LULZBOT_Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN)
        #if defined(LULZBOT_Z_MIN_ENDSTOP_INVERTING)
            #define LULZBOT_TEST_PROBE_PIN !READ(Z_MIN_PIN)
        #else
            #define LULZBOT_TEST_PROBE_PIN  READ(Z_MIN_PIN)
        #endif
    #else
        #if defined(LULZBOT_Z_MIN_PROBE_ENDSTOP_INVERTING)
            #define LULZBOT_TEST_PROBE_PIN !READ(Z_MIN_PROBE_PIN)
        #else
            #define LULZBOT_TEST_PROBE_PIN  READ(Z_MIN_PROBE_PIN)
        #endif
    #endif

    #define LULZBOT_BACKLASH_MEASUREMENT_DECL \
        float z_backlash_measured_mm; \
        float z_backlash_distance_mm = 0; \
        float z_backlash_smoothing_mm  = LULZBOT_BACKLASH_SMOOTHING_DISTANCE; \
        float z_backlash_fade  = 0;
    #define LULZBOT_BACKLASH_MEASUREMENT_EXTERN extern float z_backlash_fade, z_backlash_measured_mm, z_backlash_distance_mm , z_backlash_smoothing_mm;
    #define LULZBOT_BACKLASH_MEASUREMENT_START  z_backlash_measured_mm = 0;

    #define LULZBOT_BACKLASH_MEASUREMENT \
        { \
            /* Measure Z backlash by raising nozzle in increments until probe breaks contact with washer */ \
            float start_height = current_position[Z_AXIS]; \
            while(current_position[Z_AXIS] < (start_height + LULZBOT_BACKLASH_MEASUREMENT_LIMIT) && LULZBOT_TEST_PROBE_PIN) { \
                do_blocking_move_to_z(current_position[Z_AXIS] + LULZBOT_BACKLASH_MEASUREMENT_RESOLUTION, MMM_TO_MMS(Z_PROBE_SPEED_SLOW)); \
            } \
            /* Average the backlash from all four corners */ \
            z_backlash_measured_mm += 0.25 * (current_position[Z_AXIS] - start_height); \
        }

    #define LULZBOT_BACKLASH_COMPENSATION_DECL \
        static void backlash_compensation(const int32_t dm, const int32_t dc, block_t*block, float delta_mm[]);

    #define LULZBOT_BACKLASH_COMPENSATION_IMPL \
        void Planner::backlash_compensation(const int32_t dm, const int32_t dc, block_t*block, float delta_mm[]) { \
            static bool    last_z_direction; \
            static int32_t residual_z_error = 0; \
            if(!planner.leveling_active || z_backlash_fade == 0 || z_backlash_distance_mm == 0) \
                return; \
            if(dc != 0) { \
                const float sign = dc < 0 ? -1 : 1; \
                const bool new_z_direction = TEST(dm, Z_AXIS); \
                /* When Z changes direction, add backlash correction to residual_z_error, \
                 * to be compensated over one or more subsequent segments */ \
                if(last_z_direction != new_z_direction) { \
                    last_z_direction = new_z_direction; \
                    residual_z_error += sign * z_backlash_distance_mm * z_backlash_fade * planner.axis_steps_per_mm[Z_AXIS]; \
                } \
                /* Take up a portion of the residual_z_error in this segment, but \
                 * only when the current segment travels along Z in the same \
                 * direction as the residual error */ \
                if((dc > 0 && residual_z_error > 0) || (dc < 0 && residual_z_error < 0)) { \
                    const int32_t z_adj = residual_z_error * min(1.0, block->millimeters / z_backlash_smoothing_mm); \
                    block->steps[Z_AXIS] += labs(z_adj); \
                    residual_z_error     -= z_adj; \
                    /* Update derived values */ \
                    delta_mm[Z_AXIS] = sign * block->steps[Z_AXIS] * steps_to_mm[Z_AXIS]; \
                    block->millimeters = SQRT(sq(delta_mm[X_AXIS]) + sq(delta_mm[Y_AXIS]) + sq(delta_mm[Z_AXIS])); \
                } \
            } \
        }

    #define LULZBOT_BACKLASH_COMPENSATION_GCODE \
        inline void gcode_M425() { \
            if (parser.seen('Z')) { \
                z_backlash_distance_mm = parser.has_value() ? parser.value_float() : z_backlash_measured_mm; \
                z_backlash_fade  = 1.0; \
            } \
            if (parser.seen('F')) z_backlash_fade = max(0, min(1.0, parser.value_float())); \
            if (parser.seen('S')) z_backlash_smoothing_mm = parser.value_float(); \
            if (!parser.seen('Z') && !parser.seen('F') && !parser.seen('S')) { \
                SERIAL_ECHOLNPAIR("Backlash measurement (mm):  ", z_backlash_measured_mm); \
                SERIAL_EOL(); \
                if(z_backlash_distance_mm > 0 && z_backlash_fade > 0) \
                    SERIAL_ECHOLNPGM("Backlash correction is active:"); \
                else \
                    SERIAL_ECHOLNPGM("Backlash correction is inactive:"); \
                SERIAL_ECHOLNPAIR("  Distance (mm):        Z", z_backlash_distance_mm); \
                SERIAL_ECHOLNPAIR("  Smoothing (mm):       S", z_backlash_smoothing_mm); \
                SERIAL_ECHOLNPAIR("  Fade:                 F", z_backlash_fade); \
                SERIAL_ECHOLNPGM("     (F1.0 = full correction, F0.0 = no correction)"); \
            } \
        }

#else
    #define LULZBOT_BACKLASH_MEASUREMENT
    #define LULZBOT_BACKLASH_MEASUREMENT_DECL
    #define LULZBOT_BACKLASH_MEASUREMENT_IMPL
    #define LULZBOT_BACKLASH_MEASUREMENT_EXTERN
    #define LULZBOT_BACKLASH_MEASUREMENT_START
    #define LULZBOT_BACKLASH_MEASUREMENT_SUMMARY
    #define LULZBOT_BACKLASH_COMPENSATION_DECL
    #define LULZBOT_BACKLASH_COMPENSATION_IMPL
#endif

/******************************** MOTOR CURRENTS *******************************/

// Values for XYZ vary by printer model, values for E vary by toolhead.

#if defined(LULZBOT_USE_EINSY_RETRO)
    // These values specify the maximum current, but actual
    // currents may be lower when used with COOLCONF
    #if defined(LULZBOT_USE_TMC_STEALTHCHOP_XY)
        // Stealth mode seems to cause the drivers to overheat
        // at considerably less current.
        #define LULZBOT_MOTOR_CURRENT_XY          900    // mA
    #else
        #define LULZBOT_MOTOR_CURRENT_XY          920    // mA
    #endif
    #define LULZBOT_MOTOR_CURRENT_Z               960    // mA

#elif defined(LULZBOT_IS_MINI) && defined(LULZBOT_USE_Z_SCREW)
    #define LULZBOT_MOTOR_CURRENT_XY              1300   // mA
    #define LULZBOT_MOTOR_CURRENT_Z               1630   // mA

#elif defined(LULZBOT_IS_MINI) && defined(LULZBOT_USE_Z_BELT)
    #define LULZBOT_MOTOR_CURRENT_XY              1300   // mA
    #define LULZBOT_MOTOR_CURRENT_Z               1630   // mA

#elif defined(LULZBOT_Juniper_TAZ5)
    #define LULZBOT_MOTOR_CURRENT_XY              950    // mA
    #define LULZBOT_MOTOR_CURRENT_Z               1275   // mA

#elif defined(LULZBOT_IS_TAZ) && defined(LULZBOT_USE_Z_SCREW)
    #define LULZBOT_MOTOR_CURRENT_XY              950    // mA
    #define LULZBOT_MOTOR_CURRENT_Z               1075   // mA

#elif defined(LULZBOT_IS_TAZ) && defined(LULZBOT_USE_Z_BELT)
    #define LULZBOT_MOTOR_CURRENT_XY              950    // mA
    #define LULZBOT_MOTOR_CURRENT_Z               950    // mA
#endif

/************ ACCELERATION, FEEDRATES AND XYZ MOTOR STEPS *******************/

// Values for XYZ vary by printer model, values for E vary by toolhead.

#if defined(LULZBOT_USE_Z_SCREW)
    // Older printers had a fudge factor for ABS shrinkage.
    #define LULZBOT_XY_STEPS                      100.5
#else
    // In the Mini 2 and TAZ 7 going forward, use true XY steps.
    #define LULZBOT_XY_STEPS                      100
#endif

#if defined(LULZBOT_IS_MINI)

    #if defined(LULZBOT_USE_TMC_STEALTHCHOP)
        #define LULZBOT_DEFAULT_ACCELERATION          500
        #define LULZBOT_DEFAULT_TRAVEL_ACCELERATION   500
    #else
        #define LULZBOT_DEFAULT_ACCELERATION          1000
        #define LULZBOT_DEFAULT_TRAVEL_ACCELERATION   1000
    #endif

    #define LULZBOT_DEFAULT_XJERK                 12.0
    #define LULZBOT_DEFAULT_YJERK                 12.0
    #define LULZBOT_DEFAULT_ZJERK                  0.4

    #if not defined(LULZBOT_Z_PROBE_OFFSET_FROM_EXTRUDER)
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
    #if not defined(LULZBOT_DEFAULT_ACCELERATION)
        #define LULZBOT_DEFAULT_ACCELERATION      500
    #endif
    #if not defined(LULZBOT_DEFAULT_TRAVEL_ACCELERATION)
        #define LULZBOT_DEFAULT_TRAVEL_ACCELERATION 500
    #endif

    #if not defined(LULZBOT_Z_PROBE_OFFSET_FROM_EXTRUDER)
        #define LULZBOT_Z_PROBE_OFFSET_FROM_EXTRUDER -1.200
    #endif
#endif

#if defined(LULZBOT_IS_MINI) && defined(LULZBOT_USE_Z_SCREW)
    #define LULZBOT_Z_STEPS                       1600
    #define LULZBOT_Z_MICROSTEPS                  16

    #define LULZBOT_DEFAULT_MAX_FEEDRATE          {300, 300, 8, 40}      // (mm/sec)
    #define LULZBOT_DEFAULT_MAX_ACCELERATION      {9000,9000,100,1000}

#elif defined(LULZBOT_IS_MINI) && defined(LULZBOT_USE_Z_BELT)
    #if defined(LULZBOT_USE_32_MICROSTEPS_ON_Z)
        #define LULZBOT_Z_STEPS                       200
        #define LULZBOT_Z_MICROSTEPS                  32
    #else
        #define LULZBOT_Z_STEPS                       100
        #define LULZBOT_Z_MICROSTEPS                  16
    #endif
    #define LULZBOT_DEFAULT_MAX_FEEDRATE          {300, 300, 300, 40}      // (mm/sec)

    #if defined(LULZBOT_USE_TMC_STEALTHCHOP_XY)
        /* Stealthchop can skip steps if the acceleration is too high */
        #define LULZBOT_DEFAULT_MAX_ACCELERATION      {3000,3000,200,1000}
    #else
        #define LULZBOT_DEFAULT_MAX_ACCELERATION      {9000,9000,200,1000}
    #endif

#elif defined(LULZBOT_IS_TAZ) && defined(LULZBOT_USE_Z_SCREW)
    #define LULZBOT_DEFAULT_MAX_FEEDRATE          {300, 300, 3, 25}      // (mm/sec)
    #define LULZBOT_DEFAULT_MAX_ACCELERATION      {9000,9000,100,10000}
    #define LULZBOT_Z_STEPS                       1600
    #define LULZBOT_Z_MICROSTEPS                  16

#elif defined(LULZBOT_IS_TAZ) && defined(LULZBOT_USE_Z_BELT)
    // Prototype Z-belt driven TAZ 7
    #define LULZBOT_DEFAULT_MAX_FEEDRATE          {300, 300, 10, 25}     // (mm/sec)
    #define LULZBOT_DEFAULT_MAX_ACCELERATION      {9000,9000,10,10000}
    #define LULZBOT_Z_STEPS                       1790.08264463
    #define LULZBOT_Z_MICROSTEPS                  16
#endif

#if defined(LULZBOT_USE_Z_BELT)
    #define LULZBOT_MANUAL_FEEDRATE_Z 40*60
#else
    #define LULZBOT_MANUAL_FEEDRATE_Z  4*60
#endif

#if LULZBOT_EXTRUDERS == 2
    #define LULZBOT_MANUAL_FEEDRATE       {50*60, 50*60, LULZBOT_MANUAL_FEEDRATE_Z, LULZBOT_MANUAL_FEEDRATE_E * 60, LULZBOT_MANUAL_FEEDRATE_E *  60} // (mm/min)
#else
    #define LULZBOT_MANUAL_FEEDRATE       {50*60, 50*60, LULZBOT_MANUAL_FEEDRATE_Z, LULZBOT_MANUAL_FEEDRATE_E * 60} // (mm/min)
#endif

#if defined(LULZBOT_USE_EINSY_RETRO)
    // Neither define LULZBOT_PWM_MOTOR_CURRENT nor LULZBOT_DIGIPOT_MOTOR_CURRENT,
    // as the current is set in Configuration_adv.h under the HAVE_TMC2130 block

    #define LULZBOT_X_CURRENT  LULZBOT_MOTOR_CURRENT_XY
    #define LULZBOT_Y_CURRENT  LULZBOT_MOTOR_CURRENT_XY
    #define LULZBOT_Z_CURRENT  LULZBOT_MOTOR_CURRENT_Z
    #define LULZBOT_E0_CURRENT LULZBOT_MOTOR_CURRENT_E

#elif defined(LULZBOT_IS_MINI)
        #define LULZBOT_PWM_MOTOR_CURRENT { \
            LULZBOT_MOTOR_CURRENT_XY, \
            LULZBOT_MOTOR_CURRENT_Z, \
            LULZBOT_MOTOR_CURRENT_E \
        } // Values in milliamps

#elif defined(LULZBOT_IS_TAZ)
    // Values 0-255 (RAMBO 135 = ~0.75A, 185 = ~1A)
    #define DIGIPOT_CURRENT(mA) ((mA-750)/5+135)

    #if LULZBOT_EXTRUDERS == 2
        #define LULZBOT_DIGIPOT_MOTOR_CURRENT { \
            DIGIPOT_CURRENT(LULZBOT_MOTOR_CURRENT_XY), \
            DIGIPOT_CURRENT(LULZBOT_MOTOR_CURRENT_XY), \
            DIGIPOT_CURRENT(LULZBOT_MOTOR_CURRENT_Z), \
            DIGIPOT_CURRENT(LULZBOT_MOTOR_CURRENT_E0), \
            DIGIPOT_CURRENT(LULZBOT_MOTOR_CURRENT_E1) \
        } // Values 0-255 (RAMBO 135 = ~0.75A, 185 = ~1A)
    #else
        #define LULZBOT_DIGIPOT_MOTOR_CURRENT { \
            DIGIPOT_CURRENT(LULZBOT_MOTOR_CURRENT_XY), \
            DIGIPOT_CURRENT(LULZBOT_MOTOR_CURRENT_XY), \
            DIGIPOT_CURRENT(LULZBOT_MOTOR_CURRENT_Z), \
            DIGIPOT_CURRENT(LULZBOT_MOTOR_CURRENT_E), \
            DIGIPOT_CURRENT(LULZBOT_MOTOR_CURRENT_E) \
        } // Values 0-255 (RAMBO 135 = ~0.75A, 185 = ~1A)
    #endif
#else
    #error Motor currents not defined
#endif

#if defined(LULZBOT_DISTINCT_E_FACTORS) && LULZBOT_EXTRUDERS == 2 && !defined(LULZBOT_SWITCHING_EXTRUDER)
    #define LULZBOT_DEFAULT_AXIS_STEPS_PER_UNIT   {LULZBOT_XY_STEPS,LULZBOT_XY_STEPS,LULZBOT_Z_STEPS,LULZBOT_E_STEPS,LULZBOT_E_STEPS}
#else
    #define LULZBOT_DEFAULT_AXIS_STEPS_PER_UNIT   {LULZBOT_XY_STEPS,LULZBOT_XY_STEPS,LULZBOT_Z_STEPS,LULZBOT_E_STEPS}
#endif

/*********************************** LCD OPTIONS *******************************/

#if defined(LULZBOT_USE_LCD_DISPLAY)
    #define LULZBOT_REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER
    #define LULZBOT_SDSUPPORT
    #define LULZBOT_XYZ_HOLLOW_FRAME_DISABLE
    #define LULZBOT_MENU_HOLLOW_FRAME_DISABLE
    #define LULZBOT_USE_SMALL_INFOFONT
    #if defined(LULZBOT_USE_AUTOLEVELING)
        #define LULZBOT_BABYSTEPPING
        #define LULZBOT_BABYSTEP_ZPROBE_OFFSET
        //#define LULZBOT_MENU_BED_LEVELING_GCODE "G28 XY\nM109 S175\nG28 Z\nM109 R145\nG12\nG29\nM104 S0"
    #endif
    #define LULZBOT_SHOW_CUSTOM_BOOTSCREEN
    #define LULZBOT_ENCODER_PULSES_PER_STEP 2
    #define LULZBOT_ENCODER_STEPS_PER_MENU_ITEM 1
    #if defined(LULZBOT_IS_MINI)
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
    #define LULZBOT_REORDERED_MENUS
    #define LULZBOT_ESTEP_REDUCED_LCD_PRECISION
    #define LULZBOT_LCD_SET_PROGRESS_MANUALLY
    #define LULZBOT_SCROLL_LONG_FILENAMES
    #define LULZBOT_BABYSTEP_ZPROBE_GFX_OVERLAY
    #define LULZBOT_DISABLE_KILL_BUTTON
    #define LULZBOT_ZOFFSET_PRECISION ftostr52
    #define LULZBOT_RESET_SELECTION_TO_FIRST_ON_MENU_BACK
    #define LULZBOT_SHOW_TEMPERATURE_ADJUSTMENT_IN_PREHEAT_MENU
    #define LULZBOT_CPU_ST7920_DELAY_2 300 // Increase delay from 250 to 300 to avoid display lock ups
    #define LULZBOT_LCD_MANUAL_EXTRUDE_RELATIVE
    #define MSG_MOVE_E _UxGT("Extruder ") // Add space to extruder string
#endif

#if defined(LULZBOT_LIGHTWEIGHT_UI)
    #define WELCOME_MSG _UxGT(LULZBOT_LCD_MACHINE_NAME " ready.")
    #define LULZBOT_STATUS_EXPIRE_SECONDS 0
#else
    #define WELCOME_MSG _UxGT("LulzBot " LULZBOT_LCD_MACHINE_NAME " ready.")
#endif

#if defined(LULZBOT_PRINTCOUNTER)
    #define LULZBOT_LCD_INFO_MENU
#endif

/***************************** CUSTOM SPLASH SCREEN *****************************/

#define LULZBOT_CUSTOM_BOOTSCREEN() \
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
    u8g.setFont(FONT_MENU_NAME);

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
#if defined(LULZBOT_USE_AUTOLEVELING)
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
