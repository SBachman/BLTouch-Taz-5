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

class Marlin_LCD_API {
  public:
    typedef const __FlashStringHelper *progmem_str;

    enum axis_t {
      X,
      Y,
      Z,
      E0,
      E1
    };

    static const uint8_t extruderCount;
    static const uint8_t fanCount;

    static const float getActualTemp_celsius(const uint8_t extruder);
    static const float getTargetTemp_celsius(const uint8_t extruder);
    static const float getFan_percent(const uint8_t fan);

    static const float getAxisPosition_mm(const axis_t axis);
    static const float getAxisSteps_per_mm(const axis_t axis);
    static const uint8_t getProgress_percent();
    static const uint32_t getProgress_seconds_elapsed();
    static const uint8_t getFeedRate_percent();
    static const float getZOffset_mm();
    static const bool isAxisPositionKnown(const axis_t axis);
    static const bool isMoving();

    static const progmem_str getFirmwareName();

    static const void setTargetTemp_celsius(const uint8_t extruder, float temp);
    static const void setFan_percent(const uint8_t fan, float percent);
    static const void setAxisPosition_mm(const axis_t axis, float position, float _feedrate_mm_s);
    static const void setAxisSteps_per_mm(const axis_t axis, float steps_per_mm);
    static const void incrementZOffset_mm(const float z_offset);

    static const void runGCode(progmem_str gcode);

    static float clamp(float value, float minimum, float maximum) {return max(min(value,maximum),minimum);};

    static void showKillScreen(const char* lcd_msg);
};

#if defined(MSG_MARLIN)
const uint8_t Marlin_LCD_API::extruderCount = EXTRUDERS;
const uint8_t Marlin_LCD_API::fanCount      = 1;

const float Marlin_LCD_API::getActualTemp_celsius(const uint8_t extruder) {
  if(extruder) {
    return thermalManager.degHotend(extruder-1);
  } else {
    return thermalManager.degBed();
  }
}

const float Marlin_LCD_API::getTargetTemp_celsius(const uint8_t extruder) {
  if(extruder) {
    return thermalManager.degTargetHotend(extruder-1);
  } else {
    return thermalManager.degTargetBed();
  }
}

const float Marlin_LCD_API::getFan_percent(const uint8_t fan) {
 return ((float(fanSpeeds[fan]) + 1) * 100) / 256;
}

const float Marlin_LCD_API::getAxisPosition_mm(const Marlin_LCD_API::axis_t axis) {
  switch(axis) {
    case X:  return current_position[X_AXIS];  break;
    case Y:  return current_position[Y_AXIS];  break;
    case Z:  return current_position[Z_AXIS];  break;
    case E0: return current_position[E_AXIS]; break;
    case E1: return current_position[E_AXIS+1]; break;
  }
}

const void Marlin_LCD_API::setAxisPosition_mm(const Marlin_LCD_API::axis_t axis, float position, float _feedrate_mm_s) {
  set_destination_from_current();
  switch(axis) {
    case X:  destination[X_AXIS]   = position; break;
    case Y:  destination[Y_AXIS]   = position; break;
    case Z:  destination[Z_AXIS]   = position; break;
    case E0: destination[E_AXIS]   = position; break;
    case E1: destination[E_AXIS+1] = position; break;
  }

  const float old_feedrate = feedrate_mm_s;
  feedrate_mm_s = _feedrate_mm_s;
  prepare_move_to_destination();
  feedrate_mm_s = old_feedrate;
}

const bool Marlin_LCD_API::isMoving() {
  return planner.blocks_queued();
}

const float Marlin_LCD_API::getAxisSteps_per_mm(const Marlin_LCD_API::axis_t axis) {
  switch(axis) {
    case X:  return planner.axis_steps_per_mm[X_AXIS];
    case Y:  return planner.axis_steps_per_mm[Y_AXIS];
    case Z:  return planner.axis_steps_per_mm[Z_AXIS];
    case E0: return planner.axis_steps_per_mm[E_AXIS];
    case E1: return planner.axis_steps_per_mm[E_AXIS+1];
  }
}

const void Marlin_LCD_API::setAxisSteps_per_mm(const Marlin_LCD_API::axis_t axis, float steps_per_mm) {
  switch(axis) {
    case X:  planner.axis_steps_per_mm[X_AXIS]   = steps_per_mm; break;
    case Y:  planner.axis_steps_per_mm[Y_AXIS]   = steps_per_mm; break;
    case Z:  planner.axis_steps_per_mm[Z_AXIS]   = steps_per_mm; break;
    case E0: planner.axis_steps_per_mm[E_AXIS]   = steps_per_mm; break;
    case E1: planner.axis_steps_per_mm[E_AXIS+1] = steps_per_mm; break;
  }
}


const void Marlin_LCD_API::incrementZOffset_mm(float babystep_increment) {
  const float new_zoffset = zprobe_zoffset + babystep_increment;
  if (WITHIN(new_zoffset, Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX)) {
    #if ENABLED(BABYSTEP_ZPROBE_OFFSET)
      if (planner.leveling_active) {
        thermalManager.babystep_axis(Z_AXIS, babystep_increment);
      }
    #endif
    zprobe_zoffset = new_zoffset;
  }
}

const uint8_t Marlin_LCD_API::getProgress_percent() {
  #if ENABLED(SDSUPPORT)
    return card.percentDone();
  #endif
}

const uint32_t Marlin_LCD_API::getProgress_seconds_elapsed() {
  const duration_t elapsed = print_job_timer.duration();
  return elapsed.value;
}

const uint8_t Marlin_LCD_API::getFeedRate_percent() {
  return feedrate_percentage;
}

const float Marlin_LCD_API::getZOffset_mm() {
  return zprobe_zoffset;
}

const void Marlin_LCD_API::runGCode(progmem_str gcode) {
  enqueue_and_echo_commands_P((const char*)gcode);
}

const bool Marlin_LCD_API::isAxisPositionKnown(const axis_t axis) {
  switch(axis) {
    case X:  return axis_known_position[X_AXIS];  break;
    case Y:  return axis_known_position[Y_AXIS];  break;
    case Z:  return axis_known_position[Z_AXIS];  break;
  }
}

const Marlin_LCD_API::progmem_str Marlin_LCD_API::getFirmwareName() {
  return F("Marlin " SHORT_BUILD_VERSION LULZBOT_FW_VERSION);
}

const void Marlin_LCD_API::setTargetTemp_celsius(const uint8_t extruder, float temp) {
  if(extruder) {
    thermalManager.setTargetHotend(clamp(temp,0,500), extruder-1);
  } else {
    thermalManager.setTargetBed(clamp(temp,0,200));
  }
}

const void Marlin_LCD_API::setFan_percent(const uint8_t fan, float percent) {
  if (fan < FAN_COUNT) {
    fanSpeeds[fan] = clamp(round(percent*256/100-1), 0, 255);
  }
}
#endif