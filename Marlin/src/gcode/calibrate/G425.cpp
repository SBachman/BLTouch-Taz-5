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

#include "../../Marlin.h"

#include "../../lcd/ultralcd.h"
#include "../../module/motion.h"
#include "../../module/planner.h"
#include "../../module/tool_change.h"

#if ENABLED(LULZBOT_CALIBRATION_GCODE)

#define CALIBRATION_MEASUREMENT_RESOLUTION       0.01                    // mm
#define CALIBRATION_MEASUREMENT_LIMIT            5.0                     // mm

#define CALIBRATION_SLOW_FEEDRATE                Z_PROBE_SPEED_SLOW      // mm/m
#define CALIBRATION_FAST_FEEDRATE                Z_PROBE_SPEED_FAST      // mm/m
#define CALIBRATION_TRAVEL_FEEDRATE              HOMING_FEEDRATE_XY      // mm/m

#define CALIBRATION_NOZZLE_TIP_PLUNGE            0.5                     // mm
#define CALIBRATION_NOZZLE_TIP_DIAMETER          2.0                     // mm

// We keep a confidence interval of how close we are to the true measurement:
//
//   UNKNOWN   - No real notion on where the cube is on the bed
//   UNCERTAIN - Measurement may be uncertain due to backlash
//   CERTAIN   - Measurement obtained with backlash compensation
//
#define CALIBRATION_MEASUREMENT_UNKNOWN           5.0                    // mm
#define CALIBRATION_MEASUREMENT_UNCERTAIN         1.0                    // mm
#define CALIBRATION_MEASUREMENT_CERTAIN           0.5                    // mm

#include "../gcode.h"

#if ENABLED(BACKLASH_GCODE)
  extern float backlash_distance_mm[];
  extern float backlash_correction, backlash_smoothing_mm;
#endif

struct measurements_t {
  float cube_top;
  float cube_left;
  float cube_right;
  float cube_front;
  float cube_back;

  float center[XYZ];

  float backlash_xl;
  float backlash_xr;
  float backlash_yf;
  float backlash_yb;
  float backlash_zt;

  float nozzle_x_width;
  float nozzle_y_depth;
  float nozzle_tip_radius;

  float error[XYZ];
  float confidence[XYZ];

  measurements_t() {
    constexpr float _center[XYZ] = CALIBRATION_CUBE_CENTER;
    confidence[X_AXIS] = CALIBRATION_MEASUREMENT_UNKNOWN;
    confidence[Y_AXIS] = CALIBRATION_MEASUREMENT_UNKNOWN;
    confidence[Z_AXIS] = CALIBRATION_MEASUREMENT_UNKNOWN;

    center[X_AXIS] = _center[X_AXIS];
    center[Y_AXIS] = _center[Y_AXIS];
    center[Z_AXIS] = _center[Z_AXIS];

    nozzle_tip_radius = CALIBRATION_NOZZLE_TIP_DIAMETER/2;
  }
};

#define HAS_X_CENTER defined(CALIBRATION_CUBE_LEFT_SIDE_MEASUREMENT)  && defined(CALIBRATION_CUBE_RIGHT_SIDE_MEASUREMENT)
#define HAS_Y_CENTER defined(CALIBRATION_CUBE_FRONT_SIDE_MEASUREMENT) && defined(CALIBRATION_CUBE_BACK_SIDE_MEASUREMENT)

static void calibrate_all();
static void calibrate_backlash(measurements_t &m);
static void calibrate_toolhead_offset(measurements_t &m, uint8_t extruder);

static void report_measured_faces(const measurements_t &m);
static void report_measured_center(const measurements_t &m);
static void report_measured_backlash(const measurements_t &m);
static void report_measured_nozzle_error(const measurements_t &m);
static void report_measured_nozzle_dimensions(const measurements_t &m);
#if HOTENDS > 1
static void report_relative_nozzle_offsets();
static void normalize_nozzle_offsets();
static void park_above_cube(measurements_t &m);
#endif
static void probe_cube(measurements_t &m, bool fast = false);
static void move_to(
  const AxisEnum a1,           const float p1,
  const AxisEnum a2 = NO_AXIS, const float p2 = 0,
  const AxisEnum a3 = NO_AXIS, const float p3 = 0
);
static float measure(const AxisEnum axis, const int dir, const bool stopping_state, float *backlash_ptr, bool fast);

/**
 * G425: Perform calibration with calibration cube.
 *
 *   B           - Perform automatic calibration of backlash only.
 *   T<extruder> - Perform automatic calibration of extruder nozzle only.
 *   V           - Measure and print all calibration data.
 *   no args     - Perform fully automatic calibration sequence.
 */
void GcodeSuite::G425() {
  if (axis_unhomed_error()) return;

  measurements_t m;

  if(parser.seen('B')) calibrate_backlash(m);
  if(parser.seen('T')) calibrate_toolhead_offset(m, parser.value_int());

  if(parser.seen('V')) {
    ui.set_status_P(PSTR("Finding calibration cube"));
    probe_cube(m, true);
    ui.set_status_P(PSTR("Measuring"));
    probe_cube(m);
    SERIAL_ECHOLNPGM("Calibration cube results: ");
    SERIAL_EOL();
    report_measured_faces(m);
    report_measured_center(m);
    SERIAL_EOL();
    report_measured_backlash(m);
    SERIAL_EOL();
    report_measured_nozzle_error(m);
    SERIAL_EOL();
    report_measured_nozzle_dimensions(m);
  }

  if(!parser.seen('B') && !parser.seen('T') && !parser.seen('V')) calibrate_all();
}

static void set_nozzle(measurements_t &m, uint8_t extruder) {
  #if HOTENDS > 1
    if(extruder != active_extruder) {
      park_above_cube(m);
      tool_change(extruder);
    }
  #endif
}

static void reset_nozzle_offsets() {
  #if HOTENDS > 1
    hotend_offset[X_AXIS][0] = 0;
    hotend_offset[Y_AXIS][0] = 0;
    hotend_offset[Z_AXIS][0] = 0;
    constexpr float tmp4[XYZ][HOTENDS] = { HOTEND_OFFSET_X, HOTEND_OFFSET_Y, HOTEND_OFFSET_Z };
    LOOP_XYZ(i) HOTEND_LOOP() hotend_offset[i][e] = tmp4[i][e];
  #endif
}

static void calibrate_all() {
  #if ENABLED(BACKLASH_GCODE)
    float saved_backlash_correction   = backlash_correction;
    #ifdef BACKLASH_SMOOTHING_MM
      float saved_backlash_smoothing_mm = backlash_smoothing_mm;
    #endif
  #endif

  measurements_t m;

  SERIAL_EOL();

  set_nozzle(m, 0);

  #if HOTENDS > 1
    reset_nozzle_offsets(); // Start up with default nozzle offsets
  #endif

  ui.set_status_P(PSTR("Finding calibration cube"));
  probe_cube(m, true);

  SERIAL_ECHOLNPGM("Approximate center of cube:");
  report_measured_center(m);
  SERIAL_EOL();

  #if ENABLED(BACKLASH_GCODE)
    ui.set_status_P(PSTR("Measuring backlash"));
    calibrate_backlash(m);
    SERIAL_ECHOLNPGM("Backlash before correction:");
    report_measured_backlash(m);

    #if ENABLED(BACKLASH_GCODE)
      backlash_correction   = 1;
      #ifdef BACKLASH_SMOOTHING_MM
        backlash_smoothing_mm = 0;
      #endif
    #endif
  #endif

  SERIAL_EOL();

  // Apply offset to toolhead one
  ui.set_status_P(PSTR("Centering nozzle"));
  calibrate_toolhead_offset(m, 0);
  #if ENABLED(BACKLASH_GCODE)
    SERIAL_ECHOLNPGM("Backlash after correction (T0):");
    report_measured_backlash(m);
  #endif

  SERIAL_EOL();

  SERIAL_ECHOLNPGM("Positional error before correction (T0):");
  report_measured_nozzle_error(m);
  SERIAL_EOL();

  for (uint8_t e = 1; e < HOTENDS; e++) {
    m.confidence[X_AXIS] = CALIBRATION_MEASUREMENT_UNKNOWN;
    m.confidence[Y_AXIS] = CALIBRATION_MEASUREMENT_UNKNOWN;
    m.confidence[Z_AXIS] = CALIBRATION_MEASUREMENT_UNKNOWN;

    set_nozzle(m, e);

    ui.set_status_P(PSTR("Finding calibration cube"));
    probe_cube(m, true);

    // Apply offset to toolhead two
    ui.set_status_P(PSTR("Centering nozzle"));
    calibrate_toolhead_offset(m, 1);
    #if ENABLED(BACKLASH_GCODE)
      SERIAL_ECHOPAIR("Backlash after correction (T", e);
      SERIAL_ECHOLNPGM("):");
      report_measured_backlash(m);
    #endif
  }

  SERIAL_EOL();

  #if HOTENDS > 1
    set_nozzle(m, 0);
    normalize_nozzle_offsets();
    report_relative_nozzle_offsets();
  #endif

  ui.set_status_P(PSTR("Calibration done."));
  move_to(X_AXIS, 150); // Park nozzle away from cube

  #if ENABLED(BACKLASH_GCODE)
    backlash_correction   = saved_backlash_correction;
    #ifdef BACKLASH_SMOOTHING_MM
      backlash_smoothing_mm = saved_backlash_smoothing_mm;
    #endif
  #endif
}

static void calibrate_backlash(measurements_t &m) {
  #if ENABLED(BACKLASH_GCODE)
    float saved_backlash_correction   = backlash_correction;
    #ifdef BACKLASH_SMOOTHING_MM
      float saved_backlash_smoothing_mm = backlash_smoothing_mm;
    #endif

    backlash_correction   = 0; // Turn off backlash compensation
    #ifdef BACKLASH_SMOOTHING_MM
      backlash_smoothing_mm = 0; // Turn off smoothing
    #endif
    probe_cube(m);

    #if HAS_X_CENTER
      backlash_distance_mm[X_AXIS] = (m.backlash_xl + m.backlash_xr)/2;
    #elif defined(CALIBRATION_CUBE_LEFT_SIDE_MEASUREMENT)
      backlash_distance_mm[X_AXIS] = m.backlash_xl;
    #elif defined(CALIBRATION_CUBE_RIGHT_SIDE_MEASUREMENT)
      backlash_distance_mm[X_AXIS] = m.backlash_xr;
    #endif

    #if HAS_Y_CENTER
      backlash_distance_mm[Y_AXIS] = (m.backlash_yf + m.backlash_yb)/2;
    #elif defined(CALIBRATION_CUBE_FRONT_SIDE_MEASUREMENT)
      backlash_distance_mm[Y_AXIS] = m.backlash_yf;
    #elif defined(CALIBRATION_CUBE_BACK_SIDE_MEASUREMENT)
      backlash_distance_mm[Y_AXIS] = m.backlash_yb;
    #endif

    backlash_distance_mm[Z_AXIS] = m.backlash_zt;

    backlash_correction   = saved_backlash_correction;
    #ifdef BACKLASH_SMOOTHING_MM
      backlash_smoothing_mm = saved_backlash_smoothing_mm;
    #endif
  #endif
}

static void calibrate_toolhead_offset(measurements_t &m, uint8_t extruder) {
  #if HAS_X_CENTER && HAS_Y_CENTER
    set_nozzle(m, extruder);
    probe_cube(m);

    #if HOTENDS > 1
      hotend_offset[X_AXIS][extruder] += m.error[X_AXIS];
      hotend_offset[Y_AXIS][extruder] += m.error[Y_AXIS];
      hotend_offset[Z_AXIS][extruder] += m.error[Z_AXIS];
    #endif

    planner.synchronize();

    // Adjust the current position
    current_position[X_AXIS] += m.error[X_AXIS];
    current_position[Y_AXIS] += m.error[Y_AXIS];
    current_position[Z_AXIS] += m.error[Z_AXIS];

    // Tell the planner the new "current position"
    sync_plan_position();
  #endif
}

static void report_measured_faces(const measurements_t &m) {
    SERIAL_ECHOLNPAIR("  Top   Z:            ", m.cube_top);
  #if defined(CALIBRATION_CUBE_LEFT_SIDE_MEASUREMENT)
    SERIAL_ECHOLNPAIR("  Left  X:            ", m.cube_left);
  #endif
  #if defined(CALIBRATION_CUBE_RIGHT_SIDE_MEASUREMENT)
    SERIAL_ECHOLNPAIR("  Right X:            ", m.cube_right);
  #endif
  #if defined(CALIBRATION_CUBE_FRONT_SIDE_MEASUREMENT)
    SERIAL_ECHOLNPAIR("  Front Y:            ", m.cube_front);
  #endif
  #if defined(CALIBRATION_CUBE_BACK_SIDE_MEASUREMENT)
    SERIAL_ECHOLNPAIR("  Back  Y:            ", m.cube_back);
  #endif
}

static void report_measured_center(const measurements_t &m) {
  #if HAS_X_CENTER
    SERIAL_ECHOLNPAIR("  Center X:           ", m.center[X_AXIS]);
  #endif
  #if HAS_Y_CENTER
    SERIAL_ECHOLNPAIR("  Center Y:           ", m.center[Y_AXIS]);
  #endif
  SERIAL_ECHOLNPAIR("  Center Z:           ", m.center[Z_AXIS]);
}

static void report_measured_backlash(const measurements_t &m) {
  #if defined(CALIBRATION_CUBE_LEFT_SIDE_MEASUREMENT)
    SERIAL_ECHOLNPAIR("  Backlash X (left):  ", m.backlash_xl);
  #endif
  #if defined(CALIBRATION_CUBE_RIGHT_SIDE_MEASUREMENT)
    SERIAL_ECHOLNPAIR("  Backlash X (right): ", m.backlash_xr);
  #endif
  #if defined(CALIBRATION_CUBE_FRONT_SIDE_MEASUREMENT)
    SERIAL_ECHOLNPAIR("  Backlash Y (front): ", m.backlash_yf);
  #endif
  #if defined(CALIBRATION_CUBE_BACK_SIDE_MEASUREMENT)
    SERIAL_ECHOLNPAIR("  Backlash Y (back):  ", m.backlash_yb);
  #endif
  SERIAL_ECHOLNPAIR(  "  Backlash Z (top):   ", m.backlash_zt);
}

static void report_measured_nozzle_error(const measurements_t &m) {
  SERIAL_ECHOLNPAIR(  "  Active Nozzle:      ", active_extruder);
  #if HAS_X_CENTER
    SERIAL_ECHOLNPAIR("  Positional Error X: ", m.error[X_AXIS]);
  #endif
  #if HAS_Y_CENTER
    SERIAL_ECHOLNPAIR("  Positional Error Y: ", m.error[Y_AXIS]);
  #endif
  SERIAL_ECHOLNPAIR(  "  Positional Error Z: ", m.error[Z_AXIS]);
}

#if HOTENDS > 1
static void normalize_nozzle_offsets() {
  for (uint8_t e = 1; e < HOTENDS; e++) {
    hotend_offset[X_AXIS][e] -= hotend_offset[X_AXIS][0];
    hotend_offset[Y_AXIS][e] -= hotend_offset[Y_AXIS][0];
    hotend_offset[Z_AXIS][e] -= hotend_offset[Z_AXIS][0];
  }
  hotend_offset[X_AXIS][0] = 0;
  hotend_offset[Y_AXIS][0] = 0;
  hotend_offset[Z_AXIS][0] = 0;
}

static void report_relative_nozzle_offsets() {
  for (uint8_t e = 1; e < HOTENDS; e++) {
    SERIAL_ECHOPAIR("Nozzle offset for T", e);
    SERIAL_ECHOLNPGM(":");
    SERIAL_ECHOLNPAIR("  Offset X:       ", hotend_offset[X_AXIS][e] - hotend_offset[X_AXIS][0]);
    SERIAL_ECHOLNPAIR("  Offset Y:       ", hotend_offset[Y_AXIS][e] - hotend_offset[Y_AXIS][0]);
    SERIAL_ECHOLNPAIR("  Offset Z:       ", hotend_offset[Z_AXIS][e] - hotend_offset[Z_AXIS][0]);
  }
}
#endif

static void report_measured_nozzle_dimensions(const measurements_t &m) {
  #if HAS_X_CENTER
    SERIAL_ECHOLNPAIR("  Nozzle Width:       ", m.nozzle_x_width);
  #endif
  #if HAS_Y_CENTER
    SERIAL_ECHOLNPAIR("  Nozzle Depth:       ", m.nozzle_y_depth);
  #endif
}

#if HOTENDS > 1
static void park_above_cube(measurements_t &m) {
  constexpr float dimensions[XYZ] = CALIBRATION_CUBE_DIMENSIONS;

  // Move to safe distance above cube/washer
  move_to(Z_AXIS, m.center[Z_AXIS] + dimensions[Z_AXIS]/2 + m.confidence[Z_AXIS]);

  // Move to center of cube in XY
  move_to(X_AXIS, m.center[X_AXIS], Y_AXIS, m.center[Y_AXIS]);
}
#endif

inline void adjust_confidence(measurements_t &m, AxisEnum axis) {
    m.confidence[axis] = (backlash_correction != 0)
      ? CALIBRATION_MEASUREMENT_CERTAIN
      : CALIBRATION_MEASUREMENT_UNCERTAIN;
}

// Probes sides of the cube gathering measurements.
static void probe_cube(measurements_t &m, bool fast) {
  constexpr float dimensions[XYZ]  = CALIBRATION_CUBE_DIMENSIONS;
  #ifndef CALIBRATION_CUBE_TOP_CENTER_MEASUREMENT
  constexpr float inset            = 2;
  #endif
  float plunge                     = fast ? 1 : CALIBRATION_NOZZLE_TIP_PLUNGE;

  const bool saved_soft_endstops_enabled = soft_endstops_enabled;
  soft_endstops_enabled = false;

  #ifdef CALIBRATION_CUBE_TOP_CENTER_MEASUREMENT
    // Move to safe distance above cube/washer
    move_to(Z_AXIS, m.center[Z_AXIS] + dimensions[Z_AXIS]/2 + m.confidence[Z_AXIS]);

    // Move to center of cube in XY
    move_to(X_AXIS, m.center[X_AXIS], Y_AXIS, m.center[Y_AXIS]);

    // Probe exact top of cube
    m.cube_top = measure(Z_AXIS, -1, true, &m.backlash_zt, fast);
    m.center    [Z_AXIS] = m.cube_top - dimensions[Z_AXIS]/2;
    adjust_confidence(m, Z_AXIS);
  #endif

  #ifdef CALIBRATION_CUBE_RIGHT_SIDE_MEASUREMENT
    // Move to safe distance above cube/washer
    move_to(Z_AXIS, m.center[Z_AXIS] + dimensions[Z_AXIS]/2 + m.confidence[Z_AXIS]);

    #ifdef CALIBRATION_CUBE_TOP_CENTER_MEASUREMENT
      // Move to center of cube in XY
      move_to(X_AXIS, m.center[X_AXIS], Y_AXIS, m.center[Y_AXIS]);
    #else
      // Probe top nearest right side
      move_to(X_AXIS, m.center[X_AXIS] + dimensions[X_AXIS]/2 - inset, Y_AXIS, m.center[Y_AXIS]);
      m.cube_top       = measure(Z_AXIS, -1, true, &m.backlash_zt, fast);
      m.center[Z_AXIS] = m.cube_top - dimensions[Z_AXIS]/2;
      adjust_confidence(m, Z_AXIS);
    #endif

    // Move to safe distance to the size of the cube
    move_to(X_AXIS, m.center[X_AXIS] + dimensions[X_AXIS]/2 + m.nozzle_tip_radius + m.confidence[X_AXIS]);

    // Plunge below the side of the cube and measure
    move_to(Z_AXIS, m.cube_top - plunge);
    m.cube_right = measure(X_AXIS, -1, true, &m.backlash_xr, fast);
    m.center    [X_AXIS] = m.cube_right - dimensions[X_AXIS]/2 - m.nozzle_tip_radius;
    adjust_confidence(m, X_AXIS);
  #endif

  #ifdef CALIBRATION_CUBE_FRONT_SIDE_MEASUREMENT
    // Move to safe distance above cube/washer
    move_to(Z_AXIS, m.center[Z_AXIS] + dimensions[Z_AXIS]/2 + m.confidence[Z_AXIS]);

    #ifdef CALIBRATION_CUBE_TOP_CENTER_MEASUREMENT
      // Move to center of cube in XY
      move_to(X_AXIS, m.center[X_AXIS], Y_AXIS, m.center[Y_AXIS]);
    #else // Probe top nearest front side
      move_to(X_AXIS, m.center[X_AXIS], Y_AXIS, m.center[Y_AXIS] - dimensions[Y_AXIS]/2 + inset);
      m.cube_top = measure(Z_AXIS, -1, true, &m.backlash_zt, fast);
      m.center[Z_AXIS] = m.cube_top - dimensions[Z_AXIS]/2;
      adjust_confidence(m, Z_AXIS);
    #endif

    // Move to safe distance to the size of the cube
    move_to(Y_AXIS, m.center[Y_AXIS] - dimensions[Y_AXIS]/2 - m.nozzle_tip_radius - m.confidence[Y_AXIS]);

    // Plunge below the side of the cube and measure
    move_to(Z_AXIS, m.cube_top - plunge);
    m.cube_front = measure(Y_AXIS, 1, true, &m.backlash_yf, fast);
    m.center    [Y_AXIS] = m.cube_front + dimensions[Y_AXIS]/2 + m.nozzle_tip_radius;
    adjust_confidence(m, Y_AXIS);
  #endif

  #ifdef CALIBRATION_CUBE_LEFT_SIDE_MEASUREMENT
    // Move to safe distance above cube/washer
    move_to(Z_AXIS, m.center[Z_AXIS] + dimensions[Z_AXIS]/2 + m.confidence[Z_AXIS]);

    #ifdef CALIBRATION_CUBE_TOP_CENTER_MEASUREMENT
      // Move to center of cube in XY
      move_to(X_AXIS, m.center[X_AXIS], Y_AXIS, m.center[Y_AXIS]);
    #else
      // Probe top nearest left side
      move_to(X_AXIS, m.center[X_AXIS] - dimensions[X_AXIS]/2 + inset, Y_AXIS, m.center[Y_AXIS]);
      m.cube_top = measure(Z_AXIS, -1, true, &m.backlash_zt, fast);
      m.center[Z_AXIS] = m.cube_top - dimensions[Z_AXIS]/2;
      adjust_confidence(m, Z_AXIS);
    #endif

    // Move to safe distance to the size of the cube
    move_to(X_AXIS, m.center[X_AXIS] - dimensions[X_AXIS]/2 - m.nozzle_tip_radius - m.confidence[X_AXIS]);

    // Plunge below the side of the cube and measure
    move_to(Z_AXIS, m.cube_top - plunge);
    m.cube_left = measure(X_AXIS, 1, true, &m.backlash_xl, fast);
    m.center    [X_AXIS] = m.cube_left + dimensions[X_AXIS]/2 + m.nozzle_tip_radius;
    adjust_confidence(m, X_AXIS);
  #endif

  #ifdef CALIBRATION_CUBE_BACK_SIDE_MEASUREMENT
    // Move to safe distance above cube/washer
    move_to(Z_AXIS, m.center[Z_AXIS] + dimensions[Z_AXIS]/2 + m.confidence[Z_AXIS]);

    #ifdef CALIBRATION_CUBE_TOP_CENTER_MEASUREMENT
      // Move to center of cube in XY
      move_to(X_AXIS, m.center[X_AXIS], Y_AXIS, m.center[Y_AXIS]);
    #else
      // Probe top nearest back side
      move_to(X_AXIS, m.center[X_AXIS], Y_AXIS, m.center[Y_AXIS] + dimensions[Y_AXIS]/2 - inset);
      m.cube_top = measure(Z_AXIS, -1, true, &m.backlash_zt, fast);
      m.center[Z_AXIS] = m.cube_top - dimensions[Z_AXIS]/2;
      adjust_confidence(m, Z_AXIS);
    #endif

    // Move to safe distance to the size of the cube
    move_to(Y_AXIS, m.center[Y_AXIS] + dimensions[Y_AXIS]/2 + m.nozzle_tip_radius + m.confidence[Y_AXIS]);

    // Plunge below the side of the cube and measure
    move_to(Z_AXIS, m.cube_top - plunge);
    m.cube_back = measure(Y_AXIS, -1, true, &m.backlash_yb, fast);
    m.center    [Y_AXIS] = m.cube_back - dimensions[Y_AXIS]/2 - m.nozzle_tip_radius;
    adjust_confidence(m, Y_AXIS);
  #endif

  #if HAS_X_CENTER
    m.center[X_AXIS] = (m.cube_left+m.cube_right)/2;
  #endif
  #if HAS_Y_CENTER
    m.center[Y_AXIS] = (m.cube_front+m.cube_back)/2;
  #endif

  #if defined(CALIBRATION_CUBE_LEFT_SIDE_MEASUREMENT) && defined(CALIBRATION_CUBE_RIGHT_SIDE_MEASUREMENT)
    m.nozzle_x_width = (m.cube_right - m.cube_left)  - m.backlash_xl - m.backlash_xr - dimensions[X_AXIS];
  #endif
  #if defined(CALIBRATION_CUBE_FRONT_SIDE_MEASUREMENT) && defined(CALIBRATION_CUBE_BACK_SIDE_MEASUREMENT)
    m.nozzle_y_depth = (m.cube_back  - m.cube_front) - m.backlash_yf - m.backlash_yb - dimensions[Y_AXIS];
  #endif

  constexpr float assumed_center[XYZ] = CALIBRATION_CUBE_CENTER;
  m.error[X_AXIS] = 0;
  m.error[Y_AXIS] = 0;
  m.error[Z_AXIS] = 0;

  #if HAS_X_CENTER
    m.error[X_AXIS] = assumed_center[X_AXIS] - m.center[X_AXIS];
  #endif
  #if HAS_Y_CENTER
    m.error[Y_AXIS] = assumed_center[Y_AXIS] - m.center[Y_AXIS];
  #endif
  m.error[Z_AXIS] = assumed_center[Z_AXIS] - m.center[Z_AXIS];

  soft_endstops_enabled = saved_soft_endstops_enabled;
}

static bool read_probe_value() {
  #if ENABLED(Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN)
    return ((READ(Z_MIN_PIN))       != Z_MIN_ENDSTOP_INVERTING);
  #else
    return ((READ(Z_MIN_PROBE_PIN)) != Z_MIN_PROBE_ENDSTOP_INVERTING);
  #endif
}

// Moves along axis in the specified dir, until the probe value becomes stopping_state,
// then return the axis value.
float measuring_movement(const AxisEnum axis, const int dir, const bool stopping_state, float resolution, float feedrate, float limit) {
  float travel = 0;
  while (travel < limit) {
    set_destination_from_current();
    destination[axis] += float(dir) * resolution;
    do_blocking_move_to(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], MMM_TO_MMS(feedrate));
    planner.synchronize();

    if(read_probe_value() != stopping_state)
      travel += resolution;
    else
      break;
  }
  return destination[axis] - resolution;
}

// Moves along axis in the specified dir, until the probe value becomes stopping_state.
// Returns the axis value at that point and reverses to starting point. When
// backlash_ptr != 0, then the axis backlash is recorded while reversing.
float measure(const AxisEnum axis, const int dir, const bool stopping_state, float *backlash_ptr, bool fast) {
  float resolution, feedrate, limit;

  if(fast) {
    resolution = 0.20;
    feedrate   = CALIBRATION_FAST_FEEDRATE;
    limit      = 50;
  } else {
    resolution = CALIBRATION_MEASUREMENT_RESOLUTION;
    feedrate   = CALIBRATION_SLOW_FEEDRATE;
    limit      = CALIBRATION_MEASUREMENT_LIMIT;
  }

  // Save position
  set_destination_from_current();
  float start_pos = destination[axis];
  const float measured_position = measuring_movement(axis, dir, stopping_state, resolution, feedrate, limit);
  // Measure backlash
  if(backlash_ptr && !fast) {
    const float release_pos = measuring_movement(axis, -1 * dir, !stopping_state, resolution, feedrate, limit);
    *backlash_ptr = ABS(release_pos - measured_position);
  }
  // Return to starting position
  destination[axis] = start_pos;
  do_blocking_move_to(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], MMM_TO_MMS(CALIBRATION_TRAVEL_FEEDRATE));
  return measured_position;
}

// Move to a particular location. Up to three individual axis
// and their destinations can be specified, in any order.
void move_to(
  const AxisEnum a1, const float p1,
  const AxisEnum a2, const float p2,
  const AxisEnum a3, const float p3
) {
  set_destination_from_current();

  // Note: The order of p1, p2, p3 may not correspond to X, Y, Z
  if(a1 != NO_AXIS) destination[a1] = p1;
  if(a2 != NO_AXIS) destination[a2] = p2;
  if(a3 != NO_AXIS) destination[a3] = p3;

  // Make sure coordinates are within bounds
  destination[X_AXIS] = MAX(MIN(destination[X_AXIS],X_MAX_POS),X_MIN_POS);
  destination[Y_AXIS] = MAX(MIN(destination[Y_AXIS],Y_MAX_POS),Y_MIN_POS);
  destination[Z_AXIS] = MAX(MIN(destination[Z_AXIS],Z_MAX_POS),Z_MIN_POS);

  // Move to commanded destination
  do_blocking_move_to(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], MMM_TO_MMS(CALIBRATION_TRAVEL_FEEDRATE));
}

#endif // CALIBRATION_GCODE
