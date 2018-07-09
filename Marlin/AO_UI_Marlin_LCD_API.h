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

#ifndef __MARLIN_LCD_API_H__
#define __MARLIN_LCD_API_H__

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

    static float getActualTemp_celsius(const uint8_t extruder);
    static float getTargetTemp_celsius(const uint8_t extruder);
    static float getFan_percent(const uint8_t fan);

    static float getAxisPosition_mm(const axis_t axis);
    static float getAxisSteps_per_mm(const axis_t axis);
    static uint8_t getProgress_percent();
    static uint32_t getProgress_seconds_elapsed();
    static float getFeedRate_percent();
    static float getZOffset_mm();
    static bool isAxisPositionKnown(const axis_t axis);
    static bool isMoving();

    static progmem_str getFirmwareName();

    static void setTargetTemp_celsius(const uint8_t extruder, float temp);
    static void setFan_percent(const uint8_t fan, float percent);
    static void setAxisPosition_mm(const axis_t axis, float position, float _feedrate_mm_s);
    static void setAxisSteps_per_mm(const axis_t axis, float steps_per_mm);
    static void incrementZOffset_mm(const float z_offset);
    static void setFeedrate_percent(const float percent);

    static void runGCode(progmem_str gcode);

    static float clamp(float value, float minimum, float maximum) {return max(min(value,maximum),minimum);};

    static void initMedia();
    static void checkMedia();
    static bool isPrintingFromMedia();
    static bool isPrinting();
    static bool isMediaInserted();
    static void stopPrint();
    static void pausePrint();
    static void resumePrint();

    static void onCardInserted();
    static void onCardRemoved();
    static void onPrinterKilled(const char* lcd_msg);
    static void onPlayTone(const uint16_t frequency, const uint16_t duration);

    static uint16_t getMediaFileCount();

    static void printFromSDCard(const char *filename);
    static void changeDir(const char *dirname);
    static void upDir();
    static bool isAtRootDir();

    class Media_Iterator;
};

class Marlin_LCD_API::Media_Iterator {
  private:
    uint16_t index;
    uint16_t num_files;
  public:
    Media_Iterator(uint16_t start_index /* = 0*/);
    bool hasMore();
    void seek(uint16_t);
    void next();
    const char *longFilename();
    const char *shortFilename();
    const char *filename();
    uint16_t value() {return index;}
    uint16_t count() {return num_files;}
    bool isDirectory();
};

#if defined(MSG_MARLIN)
const uint8_t Marlin_LCD_API::extruderCount = EXTRUDERS;
const uint8_t Marlin_LCD_API::fanCount      = 1;

float Marlin_LCD_API::getActualTemp_celsius(const uint8_t extruder) {
  if(extruder) {
    return thermalManager.degHotend(extruder-1);
  } else {
    return thermalManager.degBed();
  }
}

float Marlin_LCD_API::getTargetTemp_celsius(const uint8_t extruder) {
  if(extruder) {
    return thermalManager.degTargetHotend(extruder-1);
  } else {
    return thermalManager.degTargetBed();
  }
}

float Marlin_LCD_API::getFan_percent(const uint8_t fan) {
 return ((float(fanSpeeds[fan]) + 1) * 100) / 256;
}

float Marlin_LCD_API::getAxisPosition_mm(const Marlin_LCD_API::axis_t axis) {
  switch(axis) {
    case X:  return current_position[X_AXIS];  break;
    case Y:  return current_position[Y_AXIS];  break;
    case Z:  return current_position[Z_AXIS];  break;
    case E0: return current_position[E_AXIS]; break;
    case E1: return current_position[E_AXIS+1]; break;
  }
}

void Marlin_LCD_API::setAxisPosition_mm(const Marlin_LCD_API::axis_t axis, float position, float _feedrate_mm_s) {
  set_destination_from_current();
  switch(axis) {
    case X:  destination[X_AXIS]   = position; break;
    case Y:  destination[Y_AXIS]   = position; break;
    case Z:  destination[Z_AXIS]   = position; break;
    case E0: destination[E_AXIS]   = position; break;
    case E1: destination[E_AXIS+1] = position; break;
  }

  float old_feedrate = feedrate_mm_s;
  feedrate_mm_s = _feedrate_mm_s;
  prepare_move_to_destination();
  feedrate_mm_s = old_feedrate;
}

bool Marlin_LCD_API::isMoving() {
  return planner.blocks_queued();
}

float Marlin_LCD_API::getAxisSteps_per_mm(const Marlin_LCD_API::axis_t axis) {
  switch(axis) {
    case X:  return planner.axis_steps_per_mm[X_AXIS];
    case Y:  return planner.axis_steps_per_mm[Y_AXIS];
    case Z:  return planner.axis_steps_per_mm[Z_AXIS];
    case E0: return planner.axis_steps_per_mm[E_AXIS];
    case E1: return planner.axis_steps_per_mm[E_AXIS+1];
  }
}

void Marlin_LCD_API::setAxisSteps_per_mm(const Marlin_LCD_API::axis_t axis, float steps_per_mm) {
  switch(axis) {
    case X:  planner.axis_steps_per_mm[X_AXIS]   = steps_per_mm; break;
    case Y:  planner.axis_steps_per_mm[Y_AXIS]   = steps_per_mm; break;
    case Z:  planner.axis_steps_per_mm[Z_AXIS]   = steps_per_mm; break;
    case E0: planner.axis_steps_per_mm[E_AXIS]   = steps_per_mm; break;
    case E1: planner.axis_steps_per_mm[E_AXIS+1] = steps_per_mm; break;
  }
}


void Marlin_LCD_API::incrementZOffset_mm(float babystep_increment) {
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

uint8_t Marlin_LCD_API::getProgress_percent() {
  #if ENABLED(SDSUPPORT)
    return card.percentDone();
  #endif
}

uint32_t Marlin_LCD_API::getProgress_seconds_elapsed() {
  const duration_t elapsed = print_job_timer.duration();
  return elapsed.value;
}

float Marlin_LCD_API::getFeedRate_percent() {
  return feedrate_percentage;
}

float Marlin_LCD_API::getZOffset_mm() {
  return zprobe_zoffset;
}

void Marlin_LCD_API::runGCode(progmem_str gcode) {
  enqueue_and_echo_commands_P((const char*)gcode);
}

bool Marlin_LCD_API::isAxisPositionKnown(const axis_t axis) {
  switch(axis) {
    case X:  return axis_known_position[X_AXIS];  break;
    case Y:  return axis_known_position[Y_AXIS];  break;
    case Z:  return axis_known_position[Z_AXIS];  break;
  }
}

Marlin_LCD_API::progmem_str Marlin_LCD_API::getFirmwareName() {
  return F("Marlin " SHORT_BUILD_VERSION LULZBOT_FW_VERSION);
}

void Marlin_LCD_API::setTargetTemp_celsius(const uint8_t extruder, float temp) {
  if(extruder) {
    thermalManager.setTargetHotend(clamp(temp,0,500), extruder-1);
  } else {
    thermalManager.setTargetBed(clamp(temp,0,200));
  }
}

void Marlin_LCD_API::setFan_percent(const uint8_t fan, float percent) {
  if (fan < FAN_COUNT) {
    fanSpeeds[fan] = clamp(round(percent*256/100-1), 0, 255);
  }
}

void Marlin_LCD_API::setFeedrate_percent(const float percent) {
  feedrate_percentage = clamp(percent, 10, 500);
}

void Marlin_LCD_API::printFromSDCard(const char *filename) {
  card.openAndPrintFile(filename);
}

void Marlin_LCD_API::changeDir(const char *dirname) {
  card.chdir(dirname);
}

void Marlin_LCD_API::upDir() {
  card.updir();
}

bool Marlin_LCD_API::isAtRootDir() {
  card.getWorkDirName();
  return card.filename[0] == '/';
}

void lcd_setstatusPGM(const char * const message, int8_t level /* = 0 */);

uint8_t lcd_sd_status;

void Marlin_LCD_API::initMedia() {
  #if ENABLED(SDSUPPORT) && PIN_EXISTS(SD_DETECT)
    SET_INPUT_PULLUP(SD_DETECT_PIN);
    lcd_sd_status = 2; // UNKNOWN
  #endif
}

void Marlin_LCD_API::checkMedia() {
  #if (ENABLED(SDSUPPORT) && PIN_EXISTS(SD_DETECT)) || defined(LULZBOT_USE_USB_STICK)
    const bool sd_status = isMediaInserted();
    if (sd_status != lcd_sd_status) {

      SERIAL_PROTOCOLLNPAIR("SD Status: ", sd_status);

      if (sd_status) {
        card.initsd();
        if (lcd_sd_status != 2) onCardInserted();
        SERIAL_PROTOCOLLNPAIR("Card: ",1);
      }
      else {
        card.release();
        if (lcd_sd_status != 2) onCardRemoved();
        SERIAL_PROTOCOLLNPAIR("Card: ",0);
      }

      lcd_sd_status = sd_status;
    }
  #endif // SDSUPPORT && SD_DETECT_PIN
}

bool Marlin_LCD_API::isPrintingFromMedia() {
  #if ENABLED(SDSUPPORT)
    return card.cardOK && card.isFileOpen() && card.sdprinting;
  #else
    return false;
  #endif
}

bool Marlin_LCD_API::isPrinting() {
  return (planner.movesplanned() || IS_SD_PRINTING ||
  #if ENABLED(SDSUPPORT)
    (card.cardOK && card.isFileOpen()));
  #else
    false;
  #endif
}

bool Marlin_LCD_API::isMediaInserted() {
  #if (ENABLED(SDSUPPORT) && PIN_EXISTS(SD_DETECT)) || defined(LULZBOT_USE_USB_STICK)
    #if defined(LULZBOT_USE_USB_STICK)
      return Sd2Card::isInserted();
    #else
      return IS_SD_INSERTED;
    #endif
  #else
    return false;
  #endif
}

void Marlin_LCD_API::pausePrint() {
  #if ENABLED(SDSUPPORT)
    card.pauseSDPrint();
    print_job_timer.pause();
    #if ENABLED(PARK_HEAD_ON_PAUSE)
      enqueue_and_echo_commands_P(PSTR("M125"));
    #endif
    lcd_setstatusPGM(PSTR(MSG_PRINT_PAUSED), -1);
  #endif
}

void Marlin_LCD_API::resumePrint() {
  #if ENABLED(SDSUPPORT)
    #if ENABLED(PARK_HEAD_ON_PAUSE)
      enqueue_and_echo_commands_P(PSTR("M24"));
    #else
      card.startFileprint();
      print_job_timer.start();
    #endif
  #endif
}

void Marlin_LCD_API::stopPrint() {
  #if ENABLED(SDSUPPORT)
    card.stopSDPrint();
    clear_command_queue();
    quickstop_stepper();
    print_job_timer.stop();
    thermalManager.disable_all_heaters();
    #if FAN_COUNT > 0
      for (uint8_t i = 0; i < FAN_COUNT; i++) fanSpeeds[i] = 0;
    #endif
    wait_for_heatup = false;
    lcd_setstatusPGM(PSTR(MSG_PRINT_ABORTED), -1);
  #endif
}

Marlin_LCD_API::Media_Iterator::Media_Iterator(uint16_t start_index /* = 0*/) {
  num_files = card.get_num_Files();
  index     = min(start_index, num_files-1);
  seek(index);
}

bool Marlin_LCD_API::Media_Iterator::hasMore() {
  return (index < (num_files - 1)) && (num_files > 0);
}

void Marlin_LCD_API::Media_Iterator::next() {
  if(hasMore()) {
    index++;
    seek(index);
  }
}

void Marlin_LCD_API::Media_Iterator::seek(uint16_t index) {
  const uint16_t nr =
    #if ENABLED(SDCARD_RATHERRECENTFIRST) && DISABLED(SDCARD_SORT_ALPHA)
      num_files - 1 -
    #endif
  index;

  #if ENABLED(SDCARD_SORT_ALPHA)
    card.getfilename_sorted(nr);
  #else
    card.getfilename(nr);
  #endif
}

const char *Marlin_LCD_API::Media_Iterator::filename() {
  return (card.longFilename && card.longFilename[0]) ? card.longFilename : card.filename;
}

const char *Marlin_LCD_API::Media_Iterator::shortFilename() {
  return card.filename;
}

const char *Marlin_LCD_API::Media_Iterator::longFilename() {
  return card.longFilename;
}

bool Marlin_LCD_API::Media_Iterator::isDirectory() {
  return card.filenameIsDir;
}
#endif

#endif MARLIN_LCD_API__MARLIN_LCD_API_H__