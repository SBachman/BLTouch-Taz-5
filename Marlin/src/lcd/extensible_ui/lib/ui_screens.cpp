/******************
 * ui_screens.cpp *
 ******************/

/****************************************************************************
 *   Written By Mark Pelletier  2017 - Aleph Objects, Inc.                  *
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

#include "ui.h"

#if ENABLED(EXTENSIBLE_UI)

#include "ftdi_eve_constants.h"
#include "ftdi_eve_functions.h"
#include "ftdi_eve_panels.h"
#include "ftdi_eve_dl.h"

#include "ui_builder.h"
#include "ui_sounds.h"
#include "ui_bitmaps.h"
#include "ui_screens.h"
#include "ui_theme.h"
#include "ui_event_loop.h"
#include "ui_storage.h"
#include "ui_filereader.h"

#if ENABLED(LULZBOT_PRINTCOUNTER)
  #include "../../../module/printcounter.h"
#endif

using namespace FTDI;

#define N_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

static tiny_timer_t refresh_timer;

/****************************** SCREEN STATIC DATA *************************/

// To save RAM, store state information related to a particular screen
// in a union. The values should be initialized in the onEntry method.

static union {
  struct {uint8_t increment;}                  ValueAdjusters;
  struct {uint8_t volume; uint8_t brightness;} InterfaceSettingsScreen;
  struct {char passcode[5];}                   LockScreen;
  struct {bool isError;}                       AlertBoxScreen;
  struct {uint8_t e_tag, t_tag, repeat_tag;}   ChangeFilamentScreen;
  struct {
    struct {
      uint8_t is_dir  : 1;
      uint8_t is_root : 1;
    } flags;
    uint8_t  selected_tag;
    uint8_t  num_page;
    uint8_t  cur_page;
  } FilesScreen;
  struct {
    struct ValueAdjusters placeholder;
    float e_rel[ExtUI::extruderCount];
  } MoveAxisScreen;
#if ENABLED(BABYSTEPPING)
  struct {
    struct ValueAdjusters placeholder;
    int16_t rel[XYZ];
    #if EXTRUDERS > 1
      bool  link_nozzles;
    #endif
    bool show_offsets;
  } AdjustOffsetsScreen;
#endif
} screen_data;

/******************************* MENU SCREEN TABLE ******************************/

SCREEN_TABLE {
  DECL_SCREEN(BootScreen),
  DECL_SCREEN(AboutScreen),
  DECL_SCREEN(StatisticsScreen),
  DECL_SCREEN(AlertBoxScreen),
  DECL_SCREEN(RestoreFailsafeScreen),
  DECL_SCREEN(SaveSettingsScreen),
  DECL_SCREEN(ConfirmAbortPrint),
#if ENABLED(LULZBOT_CALIBRATION_GCODE)
  DECL_SCREEN(ConfirmAutoCalibration),
#endif
  DECL_SCREEN(SpinnerScreen),
  DECL_SCREEN(CalibrationScreen),
  DECL_SCREEN(StatusScreen),
  DECL_SCREEN(MenuScreen),
  DECL_SCREEN(TuneScreen),
#if ENABLED(BABYSTEPPING)
  DECL_SCREEN(AdjustOffsetsScreen),
#endif
  DECL_SCREEN(MoveAxisScreen),
  DECL_SCREEN(AdvancedSettingsScreen),
  DECL_SCREEN(StepsScreen),
#if HAS_BED_PROBE
  DECL_SCREEN(ZOffsetScreen),
#endif
#if HOTENDS > 1
  DECL_SCREEN(NozzleOffsetScreen),
#endif
#if ENABLED(BACKLASH_GCODE)
  DECL_SCREEN(BacklashCompensationScreen),
#endif
  DECL_SCREEN(MaxFeedrateScreen),
  DECL_SCREEN(MaxVelocityScreen),
  DECL_SCREEN(MaxAccelerationScreen),
  DECL_SCREEN(DefaultAccelerationScreen),
#if ENABLED(JUNCTION_DEVIATION)
  DECL_SCREEN(JunctionDeviationScreen),
#else
  DECL_SCREEN(JerkScreen),
#endif
#if ENABLED(LIN_ADVANCE) || ENABLED(FILAMENT_RUNOUT_SENSOR)
  DECL_SCREEN(FilamentOptionsScreen),
#endif
  DECL_SCREEN(TemperatureScreen),
  DECL_SCREEN(ChangeFilamentScreen),
  DECL_SCREEN(InterfaceSettingsScreen),
  DECL_SCREEN(InterfaceSoundsScreen),
  DECL_SCREEN(LockScreen),
  DECL_SCREEN(FilesScreen),
#if ENABLED(DEVELOPER_SCREENS)
  DECL_SCREEN(DeveloperScreen),
  DECL_SCREEN(EraseSPIFlashScreen),
  DECL_SCREEN(WidgetsScreen),
  DECL_SCREEN(CalibrationRegistersScreen),
  DECL_SCREEN(DiagnosticsScreen),
#endif
  DECL_SCREEN(MediaPlayerScreen),
};

SCREEN_TABLE_POST

/****************** COLOR SCALES ***********************/

uint32_t Theme::getWarmColor(uint16_t temp, uint16_t cool, uint16_t low, uint16_t med, uint16_t high) {
  FTDI::rgb_t R0, R1, mix;

  float t;
  if(temp < cool) {
    R0 = cool_rgb;
    R1 = low_rgb;
    t  = 0;
  }
  else if(temp < low) {
    R0 = cool_rgb;
    R1 = low_rgb;
    t = (float(temp)-cool)/(low-cool);
  }
  else if(temp < med) {
    R0 = low_rgb;
    R1 = med_rgb;
    t = (float(temp)-low)/(med-low);
  }
  else if(temp < high) {
    R0 = med_rgb;
    R1 = high_rgb;
    t = (float(temp)-med)/(high-med);
  }
  else if(temp >= high) {
    R0 = med_rgb;
    R1 = high_rgb;
    t = 1;
  }
  FTDI::rgb_t::lerp(t, R0, R1, mix);
  return mix;
}

/****************** BUTTON HIGHTLIGHTING ROUTINE ***********************/

enum {
  STYLE_DEFAULT   = 0x00,
  STYLE_DISABLED  = 0x01,
  STYLE_RED_BTN   = 0x02,
  STYLE_LIGHT_BTN = 0x04
};

void BaseScreen::onEntry() {
  CommandProcessor cmd;
  cmd.set_button_style_callback(buttonStyleCallback);
  UIScreen::onEntry();
}

bool BaseScreen::buttonStyleCallback(uint8_t tag, uint8_t &style, uint16_t &options, bool post) {
  using namespace FTDI;
  CommandProcessor cmd;

  if(post) {
    default_button_colors();
    return false;
  }

  #if defined(MENU_TIMEOUT)
  if(get_pressed_tag() != 0) {
    reset_menu_timeout();
  }
  #endif

  if(tag != 0 && get_pressed_tag() == tag) {
    options = OPT_FLAT;
  }

  if(style & STYLE_RED_BTN) {
    if(style & STYLE_DISABLED) {
      cmd.cmd(COLOR_RGB(Theme::red_btn::rgb_disabled))
         .fgcolor(Theme::red_btn::fg_disabled)
         .tag(0);
      style &= ~STYLE_DISABLED; // Clear the disabled flag
    } else {
      cmd.cmd(COLOR_RGB(Theme::red_btn::rgb_enabled))
         .fgcolor(Theme::red_btn::fg_enabled);
    }
    return true;              // Call me again to reset the colors
  }

  if(style & STYLE_LIGHT_BTN) {
    if(style & STYLE_DISABLED) {
      cmd.cmd(COLOR_RGB(Theme::light_btn::rgb_disabled))
         .fgcolor(Theme::light_btn::fg_disabled)
         .gradcolor(0xFFFFFF)
         .tag(0);
      style &= ~STYLE_DISABLED; // Clear the disabled flag
    } else {
      cmd.cmd(COLOR_RGB(Theme::light_btn::rgb_enabled))
         .gradcolor(Theme::light_btn::grad_enabled)
         .fgcolor(Theme::light_btn::fg_enabled);
    }
    return true;              // Call me again to reset the colors
  }

  if(style & STYLE_DISABLED) {
    cmd.cmd(COLOR_RGB(Theme::default_btn::rgb_disabled))
       .fgcolor(Theme::default_btn::fg_disabled)
       .tag(0);
    style &= ~STYLE_DISABLED; // Clear the disabled flag
    return true;              // Call me again to reset the colors
  }
  return false;
}

void BaseScreen::default_button_colors() {
  using namespace FTDI;
  CommandProcessor cmd;
  cmd.cmd(COLOR_RGB(Theme::default_btn::rgb_enabled))
     .gradcolor(Theme::default_btn::grad_enabled)
     .fgcolor(Theme::default_btn::fg_enabled);
}

void BaseScreen::onIdle() {
  #if defined(MENU_TIMEOUT)
    const uint32_t elapsed = millis() - last_interaction;
    if(elapsed > uint32_t(MENU_TIMEOUT) * 1000) {
      GOTO_SCREEN(StatusScreen);
      reset_menu_timeout();
    }
  #endif
}

void BaseScreen::reset_menu_timeout() {
  #if defined(MENU_TIMEOUT)
    last_interaction = millis();
  #endif
}

#if defined(MENU_TIMEOUT)
  uint32_t BaseScreen::last_interaction;
#endif

/******************************** BOOT SCREEN ****************************/

void BootScreen::onRedraw(draw_mode_t what) {
  CommandProcessor cmd;
  cmd.cmd(CLEAR_COLOR_RGB(0x000000));
  cmd.cmd(CLEAR(true,true,true));

  CLCD::turn_on_backlight();
  FTDI::SoundPlayer::set_volume(255);
}

void BootScreen::onIdle() {
  if(CLCD::is_touching()) {
    // If the user is touching the screen at startup, then
    // assume the user wants to re-calibrate the screen.
    // This gives the user the ability to recover a
    // miscalibration that has been stored to EEPROM.
    GOTO_SCREEN(CalibrationScreen);
  } else {
    if(UIData::animations_enabled()) {
      MediaPlayerScreen::playBootMedia();
    }
    GOTO_SCREEN(StatusScreen);
  }
}

/******************************** ABOUT SCREEN ****************************/

void AboutScreen::onEntry() {
  BaseScreen::onEntry();
  sound.play(chimes, PLAY_ASYNCHRONOUS);
}

void AboutScreen::onRedraw(draw_mode_t what) {
  using namespace ExtUI;
  CommandProcessor cmd;
  cmd.cmd(CLEAR_COLOR_RGB(Theme::background));
  cmd.cmd(CLEAR(true,true,true));

  #define GRID_COLS 4
  #define GRID_ROWS 9

  #if defined(LULZBOT_LCD_MACHINE_NAME) && defined(LULZBOT_LCD_TOOLHEAD_NAME)
  cmd.tag(0).font(Theme::font_medium).text(  BTN_POS(1,2), BTN_SIZE(4,1), F(LULZBOT_LCD_MACHINE_NAME "  (" LULZBOT_LCD_TOOLHEAD_NAME ")"))
  #else
  cmd       .font(Theme::font_large) .text(  BTN_POS(1,2), BTN_SIZE(4,1), F("Color Touch Panel"))
  #endif
     .tag(2).font(Theme::font_medium).text(  BTN_POS(1,3), BTN_SIZE(4,1), F("(C) 2018 Aleph Objects, Inc."))
                                     .text(  BTN_POS(1,4), BTN_SIZE(4,1), F("www.lulzbot.com"))

     .tag(0)                         .text(  BTN_POS(1,6), BTN_SIZE(4,1), progmem_str(getFirmwareName_str()))
     .tag(1)                         .button(BTN_POS(2,8), BTN_SIZE(2,1), F("Okay"));

  #undef GRID_COLS
  #undef GRID_ROWS
}

bool AboutScreen::onTouchEnd(uint8_t tag) {
  switch(tag) {
    case 1:        GOTO_PREVIOUS();              return true;
#if ENABLED(DEVELOPER_SCREENS)
    case 2:        GOTO_SCREEN(DeveloperScreen); return true;
#endif
    default:                                     return false;
  }
}

/******************************** STATISTICS SCREEN ****************************/

#define GRID_COLS 4
#define GRID_ROWS 7

void StatisticsScreen::onRedraw(draw_mode_t what) {
  using namespace ExtUI;
  CommandProcessor cmd;

  if(what & BACKGROUND) {
    char buffer[21];

    cmd.cmd(CLEAR_COLOR_RGB(Theme::background))
       .cmd(CLEAR(true,true,true))

       .font(Theme::font_medium)
       .text(BTN_POS(1,1), BTN_SIZE(4,1), F("Printer Statistics"))
       .font(Theme::font_small)
       .tag(0)
       .text      (BTN_POS(1,2), BTN_SIZE(2,1), F("Total Prints:"),     OPT_RIGHTX | OPT_CENTERY)
       .text      (BTN_POS(1,3), BTN_SIZE(2,1), F("Finished Prints:"),  OPT_RIGHTX | OPT_CENTERY)
       .text      (BTN_POS(1,4), BTN_SIZE(2,1), F("Total Print Time:"), OPT_RIGHTX | OPT_CENTERY)
       .text      (BTN_POS(1,5), BTN_SIZE(2,1), F("Longest Print:"),    OPT_RIGHTX | OPT_CENTERY)
       .text      (BTN_POS(1,6), BTN_SIZE(2,1), F("Filament Used:"),    OPT_RIGHTX | OPT_CENTERY);
    cmd.text      (BTN_POS(3,2), BTN_SIZE(2,1), getTotalPrints_str(buffer));
    cmd.text      (BTN_POS(3,3), BTN_SIZE(2,1), getFinishedPrints_str(buffer));
    cmd.text      (BTN_POS(3,4), BTN_SIZE(2,1), getTotalPrintTime_str(buffer));
    cmd.text      (BTN_POS(3,5), BTN_SIZE(2,1), getLongestPrint_str(buffer));
    cmd.text      (BTN_POS(3,6), BTN_SIZE(2,1), getFilamentUsed_str(buffer));
  }

  if(what & FOREGROUND) {
    cmd.font(Theme::font_medium)
       .style(STYLE_LIGHT_BTN)
       .tag(1).button    (BTN_POS(1,7), BTN_SIZE(4,1), F("Back"));
  }
}

bool StatisticsScreen::onTouchEnd(uint8_t tag) {
  switch(tag) {
    case 1:        GOTO_PREVIOUS();              return true;
    default:                                     return false;
  }
}

#undef  GRID_COLS
#undef  GRID_ROWS

/**************************** GENERIC DIALOG BOX SCREEN ****************************/

#define GRID_COLS 2
#define GRID_ROWS 8

void DialogBoxBaseClass::drawMessage(const progmem_str line1, const progmem_str line2, const progmem_str line3, int16_t font) {
  progmem_str lines[] = {line1, line2, line3};
  const uint8_t n_lines = line3 ? 3 : line2 ? 2 : 1;
  CommandProcessor cmd;
  cmd.cmd(CMD_DLSTART)
     .cmd(CLEAR_COLOR_RGB(Theme::background))
     .cmd(CLEAR(true,true,true));
  cmd.font(font ? font : Theme::font_large);
  for(uint8_t line = 0; line < n_lines; line++) {
    cmd.text  ( BTN_POS(1,3-n_lines/2+line), BTN_SIZE(2,1), lines[line]);
  }
}

void DialogBoxBaseClass::drawYesNoButtons() {
  CommandProcessor cmd;
  cmd.font(Theme::font_medium)
     .tag(1).button( BTN_POS(1,8), BTN_SIZE(1,1), F("Yes"))
     .tag(2).button( BTN_POS(2,8), BTN_SIZE(1,1), F("No"));
}

void DialogBoxBaseClass::drawOkayButton() {
  CommandProcessor cmd;
  cmd.font(Theme::font_medium)
     .tag(1).button( BTN_POS(1,8), BTN_SIZE(2,1), F("Okay"));
}

void DialogBoxBaseClass::drawSpinner() {
  CommandProcessor cmd;
  cmd.spinner(BTN_POS(1,5), BTN_SIZE(2,2)).execute();
}

#undef GRID_COLS
#undef GRID_ROWS

bool DialogBoxBaseClass::onTouchEnd(uint8_t tag) {
  switch(tag) {
    case 1: GOTO_PREVIOUS(); return true;
    case 2: GOTO_PREVIOUS(); return true;
    default:                 return false;
  }
}

/****************************** ALERT BOX SCREEN *****************************/

void AlertBoxScreen::onEntry() {
  BaseScreen::onEntry();
  sound.play(screen_data.AlertBoxScreen.isError ? sad_trombone : twinkle, PLAY_ASYNCHRONOUS);
}

void AlertBoxScreen::onRedraw(draw_mode_t what) {
  if(what & FOREGROUND) {
    drawOkayButton();
  }
}

void AlertBoxScreen::show(const progmem_str line1, const progmem_str line2, const progmem_str line3) {
  drawMessage(line1, line2, line3);
  storeBackground();
  screen_data.AlertBoxScreen.isError = false;
  GOTO_SCREEN(AlertBoxScreen);
}

void AlertBoxScreen::showError(const progmem_str line1, const progmem_str line2, const progmem_str line3) {
  drawMessage(line1, line2, line3);
  storeBackground();
  screen_data.AlertBoxScreen.isError = true;
  GOTO_SCREEN(AlertBoxScreen);
}

/**************************** RESTORE FAILSAFE SCREEN ***************************/

void RestoreFailsafeScreen::onRedraw(draw_mode_t what) {
  drawMessage(
    F("Are you sure?"),
    F("Customizations will be lost.")
  );
  drawYesNoButtons();
}

bool RestoreFailsafeScreen::onTouchEnd(uint8_t tag) {
  switch(tag) {
    case 1:
      ExtUI::enqueueCommands_P(PSTR("M502"));
      AlertBoxScreen::show(F("Factory settings restored."));
      // Remove RestoreFailsafeScreen from the stack
      // so the alert box doesn't return to it.
      current_screen.forget();
      return true;
    default:
      return DialogBoxBaseClass::onTouchEnd(tag);
  }
}

/**************************** SAVE CHANGES SCREEN ***************************/

void SaveSettingsScreen::onRedraw(draw_mode_t what) {
  drawMessage(
    F("Do you wish to save these"),
    F("settings as power on defaults?")
  );
  drawYesNoButtons();
}

bool SaveSettingsScreen::onTouchEnd(uint8_t tag) {
  switch(tag) {
    case 1:
      ExtUI::enqueueCommands_P(PSTR("M500"));
      AlertBoxScreen::show(F("Settings saved!"));
      // Remove SaveSettingsScreen from the stack
      // so the alert box doesn't return to me.
      current_screen.forget();
      return true;
    default:
      return DialogBoxBaseClass::onTouchEnd(tag);
  }
}

void SaveSettingsScreen::promptToSaveSettings() {
   // Remove current screen from the stack
   // so SaveSettingsScreen doesn't return here.
   GOTO_SCREEN(SaveSettingsScreen);
   current_screen.forget();
}

/**************************** CONFIRM ABORT SCREEN ***************************/

void ConfirmAbortPrint::onRedraw(draw_mode_t what) {
  drawMessage(
    F("Are you sure you want"),
    F("to cancel the print?")
  );
  drawYesNoButtons();
}

bool ConfirmAbortPrint::onTouchEnd(uint8_t tag) {
  switch(tag) {
    case 1:
      GOTO_SCREEN(StatusScreen);
      ExtUI::stopPrint();
      return true;
    default:
      return DialogBoxBaseClass::onTouchEnd(tag);
  }
}

/**************** CONFIRM AUTO-CALIBRATION SCREEN *******************/

#if ENABLED(LULZBOT_CALIBRATION_GCODE)
void ConfirmAutoCalibration::onRedraw(draw_mode_t what) {
  drawMessage(
    F("This process will adjust the"),
    F("backlash and nozzle offsets."),
    F("Continue with auto-calibration?"),
    Theme::font_small
  );
  drawYesNoButtons();
}

bool ConfirmAutoCalibration::onTouchEnd(uint8_t tag) {
  using namespace ExtUI;

  switch(tag) {
    case 1:
      GOTO_SCREEN(StatusScreen);
      enqueueCommands_P(PSTR(LULZBOT_CALIBRATION_SCRIPT));
      return true;
    default:
      return DialogBoxBaseClass::onTouchEnd(tag);
  }
}
#endif

/****************************** SPINNER SCREEN *****************************/

void SpinnerScreen::onRedraw(draw_mode_t what) {
}

void SpinnerScreen::show(const progmem_str line1, const progmem_str line2, const progmem_str line3) {
  drawMessage(line1, line2, line3);
  drawSpinner();
  storeBackground();
}

void SpinnerScreen::hide() {
  CommandProcessor cmd;
  cmd.stop().execute();
}


/************************************ KILL SCREEN *******************************/

// The kill screen is an oddball that happens after Marlin has killed the events
// loop. So we only have a show() method rather than onRedraw(). The KillScreen
// should not be used as a model for other UI screens as it is an exception.

void KillScreen::show(progmem_str message) {
  CommandProcessor cmd;

  cmd.cmd(CMD_DLSTART)
     .cmd(CLEAR_COLOR_RGB(Theme::background))
     .cmd(CLEAR(true,true,true));

  #define GRID_COLS 4
  #define GRID_ROWS 8

  cmd.font(Theme::font_large)
     .text(BTN_POS(1,2), BTN_SIZE(4,1), message)
     .text(BTN_POS(1,3), BTN_SIZE(4,1), F("PRINTER HALTED"))
     .text(BTN_POS(1,6), BTN_SIZE(4,1), F("Please reset"));

  #undef GRID_COLS
  #undef GRID_ROWS

  cmd.cmd(DL::DL_DISPLAY)
     .cmd(CMD_SWAP)
     .execute();

  InterfaceSoundsScreen::playEventSound(InterfaceSoundsScreen::PRINTING_FAILED, PLAY_SYNCHRONOUS);
}

/*********************************** STATUS SCREEN ******************************/
#if defined(USE_PORTRAIT_ORIENTATION)
  #define GRID_ROWS 8
#else
  #define GRID_ROWS 8
#endif

void StatusScreen::draw_axis_position(draw_mode_t what) {
  CommandProcessor cmd;

  #define GRID_COLS 3

  if(what & BACKGROUND) {
    cmd.tag(6)
    #if defined(USE_PORTRAIT_ORIENTATION)
      .fgcolor(Theme::axis_label)
        .font(Theme::font_large)
                         .button( BTN_POS(1,5), BTN_SIZE(2,1), F(""), OPT_FLAT)
                         .button( BTN_POS(1,6), BTN_SIZE(2,1), F(""), OPT_FLAT)
                         .button( BTN_POS(1,7), BTN_SIZE(2,1), F(""), OPT_FLAT)

        .font(Theme::font_small)
                         .text  ( BTN_POS(1,5), BTN_SIZE(1,1), F("X"))
                         .text  ( BTN_POS(1,6), BTN_SIZE(1,1), F("Y"))
                         .text  ( BTN_POS(1,7), BTN_SIZE(1,1), F("Z"))

        .font(Theme::font_medium)
        .fgcolor(Theme::x_axis) .button( BTN_POS(2,5), BTN_SIZE(2,1), F(""), OPT_FLAT)
        .fgcolor(Theme::y_axis) .button( BTN_POS(2,6), BTN_SIZE(2,1), F(""), OPT_FLAT)
        .fgcolor(Theme::z_axis) .button( BTN_POS(2,7), BTN_SIZE(2,1), F(""), OPT_FLAT);
    #else
      .fgcolor(Theme::axis_label)
        .font(Theme::font_large)
                         .button( BTN_POS(1,5), BTN_SIZE(1,2), F(""),  OPT_FLAT)
                         .button( BTN_POS(2,5), BTN_SIZE(1,2), F(""),  OPT_FLAT)
                         .button( BTN_POS(3,5), BTN_SIZE(1,2), F(""),  OPT_FLAT)

        .font(Theme::font_small)
                         .text  ( BTN_POS(1,5), BTN_SIZE(1,1), F("X"))
                         .text  ( BTN_POS(2,5), BTN_SIZE(1,1), F("Y"))
                         .text  ( BTN_POS(3,5), BTN_SIZE(1,1), F("Z"))
                         .font(Theme::font_medium)

        .fgcolor(Theme::x_axis) .button( BTN_POS(1,6), BTN_SIZE(1,1), F(""), OPT_FLAT)
        .fgcolor(Theme::y_axis) .button( BTN_POS(2,6), BTN_SIZE(1,1), F(""), OPT_FLAT)
        .fgcolor(Theme::z_axis) .button( BTN_POS(3,6), BTN_SIZE(1,1), F(""), OPT_FLAT);
    #endif
  }

  if(what & FOREGROUND) {
    using namespace ExtUI;
    char x_str[15];
    char y_str[15];
    char z_str[15];

    if(isAxisPositionKnown(X)) {
      dtostrf(getAxisPosition_mm(X), 5, 1, x_str);
      strcat_P(x_str, PSTR(" mm"));
    } else {
      strcpy_P(x_str, PSTR("?"));
    }

    if(isAxisPositionKnown(Y)) {
      dtostrf(getAxisPosition_mm(Y), 5, 1, y_str);
      strcat_P(y_str, PSTR(" mm"));
    } else {
      strcpy_P(y_str, PSTR("?"));
    }

    if(isAxisPositionKnown(Z)) {
      dtostrf(getAxisPosition_mm(Z), 5, 1, z_str);
      strcat_P(z_str, PSTR(" mm"));
    } else {
      strcpy_P(z_str, PSTR("?"));
    }

    cmd.tag(6).font(Theme::font_medium)
    #if defined(USE_PORTRAIT_ORIENTATION)
         .text  ( BTN_POS(2,5), BTN_SIZE(2,1), x_str)
         .text  ( BTN_POS(2,6), BTN_SIZE(2,1), y_str)
         .text  ( BTN_POS(2,7), BTN_SIZE(2,1), z_str);
    #else
         .text  ( BTN_POS(1,6), BTN_SIZE(1,1), x_str)
         .text  ( BTN_POS(2,6), BTN_SIZE(1,1), y_str)
         .text  ( BTN_POS(3,6), BTN_SIZE(1,1), z_str);
    #endif
  }

  #undef GRID_COLS
}

#if defined(USE_PORTRAIT_ORIENTATION)
  #define GRID_COLS 8
#else
  #define GRID_COLS 12
#endif

#define ROUND(val) uint16_t((val)+0.5)

void StatusScreen::draw_temperature(draw_mode_t what) {
  CommandProcessor cmd;

  if(what & BACKGROUND) {
    cmd.font(Theme::font_small)
    #if defined(USE_PORTRAIT_ORIENTATION)
       .tag(5)
       .fgcolor(Theme::temp)      .button( BTN_POS(1,1), BTN_SIZE(4,2), F(""), OPT_FLAT)
                                  .button( BTN_POS(1,1), BTN_SIZE(8,1), F(""), OPT_FLAT)
       .fgcolor(Theme::fan_speed) .button( BTN_POS(5,2), BTN_SIZE(4,1), F(""), OPT_FLAT)
       .tag(0)
       .fgcolor(Theme::progress)  .button( BTN_POS(1,3), BTN_SIZE(4,1), F(""), OPT_FLAT)
                                  .button( BTN_POS(5,3), BTN_SIZE(4,1), F(""), OPT_FLAT);
    #else
       .tag(5)
       .fgcolor(Theme::temp)      .button( BTN_POS(1,1), BTN_SIZE(4,2), F(""), OPT_FLAT)
                                  .button( BTN_POS(1,1), BTN_SIZE(8,1), F(""), OPT_FLAT)
       .fgcolor(Theme::fan_speed) .button( BTN_POS(5,2), BTN_SIZE(4,1), F(""), OPT_FLAT)
       .tag(0)
       .fgcolor(Theme::progress)  .button( BTN_POS(9,1), BTN_SIZE(4,1), F(""), OPT_FLAT)
                                  .button( BTN_POS(9,2), BTN_SIZE(4,1), F(""), OPT_FLAT);
    #endif

    // Draw Extruder Bitmap on Extruder Temperature Button

    cmd.tag(5)
       .cmd(BITMAP_SOURCE(Extruder_Icon_Info))
       .cmd(BITMAP_LAYOUT(Extruder_Icon_Info))
       .cmd(BITMAP_SIZE  (Extruder_Icon_Info))
       .icon (BTN_POS(1,1), BTN_SIZE(1,1),  Extruder_Icon_Info, Theme::icon_scale)
       .icon (BTN_POS(5,1), BTN_SIZE(1,1),  Extruder_Icon_Info, Theme::icon_scale);

    // Draw Bed Heat Bitmap on Bed Heat Button
    cmd.cmd(BITMAP_SOURCE(Bed_Heat_Icon_Info))
       .cmd(BITMAP_LAYOUT(Bed_Heat_Icon_Info))
       .cmd(BITMAP_SIZE  (Bed_Heat_Icon_Info))
       .icon (BTN_POS(1,2), BTN_SIZE(1,1), Bed_Heat_Icon_Info, Theme::icon_scale);

    // Draw Fan Percent Bitmap on Bed Heat Button

    cmd.cmd(BITMAP_SOURCE(Fan_Icon_Info))
       .cmd(BITMAP_LAYOUT(Fan_Icon_Info))
       .cmd(BITMAP_SIZE  (Fan_Icon_Info))
       .icon  (BTN_POS(5,2), BTN_SIZE(1,1), Fan_Icon_Info, Theme::icon_scale);
  }

  if(what & FOREGROUND) {
    using namespace ExtUI;
    char e0_str[15];
    char e1_str[15];
    char bed_str[15];
    char fan_str[15];

    sprintf_P(
      fan_str,
      PSTR("%-3d %%"),
      int8_t(getActualFan_percent(FAN0))
    );

    sprintf_P(
      bed_str,
      PSTR("%-3d / %-3d C" ),
      ROUND(getActualTemp_celsius(BED)),
      ROUND(getTargetTemp_celsius(BED))
    );

    sprintf_P(
      e0_str,
      PSTR("%-3d / %-3d C"),
      ROUND(getActualTemp_celsius(H0)),
      ROUND(getTargetTemp_celsius(H0))
    );

    #if EXTRUDERS == 2
      sprintf_P(
        e1_str,
        PSTR("%-3d / %-3d C"),
        ROUND(getActualTemp_celsius(H1)),
        ROUND(getTargetTemp_celsius(H1))
      );
    #else
      strcpy_P(
        e1_str,
        PSTR("-")
      );
    #endif

      cmd.tag(5)
         .font(Theme::font_medium)
         .text(BTN_POS(2,1), BTN_SIZE(3,1), e0_str)
         .text(BTN_POS(6,1), BTN_SIZE(3,1), e1_str)
         .text(BTN_POS(2,2), BTN_SIZE(3,1), bed_str)
         .text(BTN_POS(6,2), BTN_SIZE(3,1), fan_str);
  }
}

void StatusScreen::draw_progress(draw_mode_t what) {
  using namespace ExtUI;
  CommandProcessor cmd;

  if(what & BACKGROUND) {
    cmd.tag(0).font(Theme::font_medium)
    #if defined(USE_PORTRAIT_ORIENTATION)
       .fgcolor(Theme::progress) .button(BTN_POS(1,3), BTN_SIZE(4,1), F(""), OPT_FLAT)
                                 .button(BTN_POS(5,3), BTN_SIZE(4,1), F(""), OPT_FLAT);
    #else
       .fgcolor(Theme::progress) .button(BTN_POS(9,1), BTN_SIZE(4,1), F(""), OPT_FLAT)
                                 .button(BTN_POS(9,2), BTN_SIZE(4,1), F(""), OPT_FLAT);
    #endif
  }

  if(what & FOREGROUND) {
    const uint32_t elapsed = getProgress_seconds_elapsed();
    const uint8_t hrs = elapsed/3600;
    const uint8_t min = (elapsed/60)%60;

    char time_str[10];
    char progress_str[10];

    sprintf_P(time_str,     PSTR(" %02d : %02d"), hrs, min);
    sprintf_P(progress_str, PSTR("%-3d %%"),      getProgress_percent() );

    cmd.font(Theme::font_medium)
    #if defined(USE_PORTRAIT_ORIENTATION)
       .tag(0).text(BTN_POS(1,3), BTN_SIZE(4,1), time_str)
              .text(BTN_POS(5,3), BTN_SIZE(4,1), progress_str);
    #else
       .tag(0).text(BTN_POS(9,1), BTN_SIZE(4,1), time_str)
              .text(BTN_POS(9,2), BTN_SIZE(4,1), progress_str);
    #endif
  }
}

#undef GRID_COLS


void StatusScreen::draw_interaction_buttons(draw_mode_t what) {
  CommandProcessor cmd;
  default_button_colors();

  #define GRID_COLS 4

  if(what & FOREGROUND) {
    using namespace ExtUI;
    CommandProcessor cmd;
    cmd.font(Theme::font_medium)
       .enabled(isMediaInserted() && !isPrintingFromMedia())
      #if defined(USE_PORTRAIT_ORIENTATION)
         .tag(3).button( BTN_POS(1,8), BTN_SIZE(2,1),
      #else
         .tag(3).button( BTN_POS(1,7), BTN_SIZE(2,2),
      #endif
      isPrintingFromMedia() ? F("Printing") :
      #if ENABLED(USB_FLASH_DRIVE_SUPPORT)
        F("USB Drive"))
      #else
        F("SD Card"))
      #endif
      .style(STYLE_LIGHT_BTN)
      #if defined(USE_PORTRAIT_ORIENTATION)
       .tag(4).button( BTN_POS(3,8), BTN_SIZE(2,1), F("MENU"));
      #else
       .tag(4).button( BTN_POS(3,7), BTN_SIZE(2,2), F("MENU"));
    #endif
  }

  #undef  GRID_COLS
}

void StatusScreen::draw_status_message(draw_mode_t what, const char * const message) {
  #define GRID_COLS 1
  if(what & BACKGROUND) {
    CommandProcessor cmd;
    cmd.fgcolor(Theme::status_msg)
       .tag(0)
       .font(Theme::font_large)
    #if defined(USE_PORTRAIT_ORIENTATION)
       .button( BTN_POS(1,4), BTN_SIZE(1,1), message, OPT_FLAT);
    #else
       .button( BTN_POS(1,3), BTN_SIZE(1,2), message, OPT_FLAT);
    #endif
  }
  #undef  GRID_COLS
}

void StatusScreen::setStatusMessage(progmem_str message) {
  char buff[strlen_P((const char * const)message)+1];
  strcpy_P(buff, (const char * const) message);
  setStatusMessage(buff);
}

void StatusScreen::setStatusMessage(const char * const message) {
  CommandProcessor cmd;
  cmd.cmd(CMD_DLSTART)
     .cmd(CLEAR_COLOR_RGB(Theme::background))
     .cmd(CLEAR(true,true,true));

  draw_temperature(BACKGROUND);
  draw_progress(BACKGROUND);
  draw_axis_position(BACKGROUND);
  draw_status_message(BACKGROUND, message);
  draw_interaction_buttons(BACKGROUND);
  storeBackground();

  #if defined(UI_FRAMEWORK_DEBUG)
    SERIAL_ECHO_START();
    SERIAL_ECHOLNPAIR("New status message: ", message);
  #endif

  if(AT_SCREEN(StatusScreen)) {
    current_screen.onRefresh();
  }
}

void StatusScreen::onStartup() {
  // Load the bitmaps for the status screen

  CLCD::mem_write_pgm(TD_Icon_Info.RAMG_addr,       TD_Icon,       sizeof(TD_Icon));
  CLCD::mem_write_pgm(Extruder_Icon_Info.RAMG_addr, Extruder_Icon, sizeof(Extruder_Icon));
  CLCD::mem_write_pgm(Bed_Heat_Icon_Info.RAMG_addr, Bed_Heat_Icon, sizeof(Bed_Heat_Icon));
  CLCD::mem_write_pgm(Fan_Icon_Info.RAMG_addr,      Fan_Icon,      sizeof(Fan_Icon));

  setStatusMessage(F(WELCOME_MSG));

  UIFlashStorage::initialize();
}

void StatusScreen::onRedraw(draw_mode_t what) {
  if(what & FOREGROUND) {
    draw_temperature(FOREGROUND);
    draw_progress(FOREGROUND);
    draw_axis_position(FOREGROUND);
    draw_interaction_buttons(FOREGROUND);
  }
}

void StatusScreen::onEntry() {
  onRefresh();
}

void StatusScreen::onIdle() {
  if(refresh_timer.elapsed(STATUS_UPDATE_INTERVAL)) {
    onRefresh();
    refresh_timer.start();
  }
  BaseScreen::onIdle();
}

bool StatusScreen::onTouchEnd(uint8_t tag) {
  using namespace ExtUI;

  switch(tag) {
    case 3: GOTO_SCREEN(FilesScreen); break;
    case 4:
      if(isPrinting()) {
        GOTO_SCREEN(TuneScreen);
      } else {
        GOTO_SCREEN(MenuScreen);
      }
      break;
    case 5:  GOTO_SCREEN(TemperatureScreen); break;
    case 6:
      if(!isPrinting()) {
        GOTO_SCREEN(MoveAxisScreen);
      }
      break;
    default:
      return true;
  }
  // If a passcode is enabled, the LockScreen will prevent the
  // user from proceeding.
  LockScreen::check_passcode();
  return true;
}

#undef GRID_ROWS

/************************************ MENU SCREEN *******************************/

void MenuScreen::onRedraw(draw_mode_t what) {
  if(what & BACKGROUND) {
    CommandProcessor cmd;
    cmd.cmd(CLEAR_COLOR_RGB(Theme::background))
       .cmd(CLEAR(true,true,true));
  }

  if(what & FOREGROUND) {
    CommandProcessor cmd;
    default_button_colors();
    cmd.font(Theme::font_medium)
    #if defined(USE_PORTRAIT_ORIENTATION)
      #define GRID_ROWS 8
      #define GRID_COLS 2
        .tag(2).button( BTN_POS(1,1), BTN_SIZE(1,1), F("Auto Home"))
        #if defined(NOZZLE_CLEAN_FEATURE)
         .enabled(1)
        #else
         .enabled(0)
        #endif
        .tag(3).button( BTN_POS(2,1), BTN_SIZE(1,1), F("Clean Nozzle"))
        .tag(4).button( BTN_POS(1,2), BTN_SIZE(1,1), F("Move Axis"))
        .tag(5).button( BTN_POS(2,2), BTN_SIZE(1,1), F("Motors Off"))
        .tag(6).button( BTN_POS(1,3), BTN_SIZE(2,1), F("Temperature"))
        .tag(7).button( BTN_POS(1,4), BTN_SIZE(2,1), F("Change Filament"))
        .tag(8).button( BTN_POS(1,5), BTN_SIZE(2,1), F("Advanced Settings"))
        #if defined(PRINTCOUNTER)
         .enabled(1)
        #else
         .enabled(0)
        #endif
        .tag(9).button( BTN_POS(1,7), BTN_SIZE(2,1), F("Printer Statistics"))
        .tag(10).button( BTN_POS(1,6), BTN_SIZE(2,1), F("About Printer"))
        .style(STYLE_LIGHT_BTN)
        .tag(1).button( BTN_POS(1,8), BTN_SIZE(2,1), F("Back"));
      #undef GRID_COLS
      #undef GRID_ROWS
    #else
      #define GRID_ROWS 5
      #define GRID_COLS 2
        .tag(2).button( BTN_POS(1,1), BTN_SIZE(1,1), F("Auto Home"))
        #if ENABLED(NOZZLE_CLEAN_FEATURE)
         .enabled(1)
        #else
         .enabled(0)
        #endif
        .tag(3).button( BTN_POS(2,1), BTN_SIZE(1,1), F("Clean Nozzle"))
        .tag(4).button( BTN_POS(1,2), BTN_SIZE(1,1), F("Move Axis"))
        .tag(5).button( BTN_POS(2,2), BTN_SIZE(1,1), F("Motors Off"))
        .tag(6).button( BTN_POS(1,3), BTN_SIZE(1,1), F("Temperature"))
        .tag(7).button( BTN_POS(2,3), BTN_SIZE(1,1), F("Change Filament"))
        .tag(8).button( BTN_POS(1,4), BTN_SIZE(1,1), F("Advanced Settings"))
        #if defined(PRINTCOUNTER)
         .enabled(1)
        #else
         .enabled(0)
        #endif
        .tag(9).button( BTN_POS(2,4), BTN_SIZE(1,1), F("Printer Statistics"))
        .tag(10).button( BTN_POS(1,5), BTN_SIZE(1,1), F("About Printer"))
        .style(STYLE_LIGHT_BTN)
        .tag(1).button( BTN_POS(2,5), BTN_SIZE(1,1), F("Back"));
      #undef GRID_COLS
      #undef GRID_ROWS
    #endif
  }
}

bool MenuScreen::onTouchEnd(uint8_t tag) {
  using namespace ExtUI;

  switch(tag) {
    case 1:  GOTO_PREVIOUS();                                         break;
    case 2:  enqueueCommands_P(PSTR("G28"));                          break;
    #if ENABLED(NOZZLE_CLEAN_FEATURE)
    case 3:  enqueueCommands_P(PSTR("G12")); GOTO_SCREEN(StatusScreen); break;
    #endif
    case 4:  GOTO_SCREEN(MoveAxisScreen);                             break;
    case 5:  enqueueCommands_P(PSTR("M84"));                          break;
    case 6:  GOTO_SCREEN(TemperatureScreen);                          break;
    case 7:  GOTO_SCREEN(ChangeFilamentScreen);                       break;
    case 8:  GOTO_SCREEN(AdvancedSettingsScreen);                     break;
#if ENABLED(PRINTCOUNTER)
    case 9:  GOTO_SCREEN(StatisticsScreen);                           break;
#endif
    case 10: GOTO_SCREEN(AboutScreen);                                break;
    default:
      return false;
  }
  return true;
}

/************************************ TUNE SCREEN *******************************/

void TuneScreen::onRedraw(draw_mode_t what) {
  if(what & BACKGROUND) {
    CommandProcessor cmd;
    cmd.cmd(CLEAR_COLOR_RGB(Theme::background))
       .cmd(CLEAR(true,true,true))
       .font(Theme::font_medium);
  }

  #if defined(USE_PORTRAIT_ORIENTATION)
    #define GRID_ROWS 8
    #define GRID_COLS 2
  #else
    #define GRID_ROWS 4
    #define GRID_COLS 2
  #endif

  if(what & FOREGROUND) {
    using namespace ExtUI;

    CommandProcessor cmd;
    default_button_colors();
    cmd.font(Theme::font_medium)
    #if defined(USE_PORTRAIT_ORIENTATION)
       .tag(2).enabled(1)      .button( BTN_POS(1,1), BTN_SIZE(2,1), F("Temperature"))
       .tag(3).enabled(1)      .button( BTN_POS(1,2), BTN_SIZE(2,1), F("Change Filament"))
       #if ENABLED(LIN_ADVANCE) || ENABLED(FILAMENT_RUNOUT_SENSOR)
          .enabled(1)
        #else
          .enabled(0)
        #endif
       .tag(9).button( BTN_POS(1,3), BTN_SIZE(2,1), F("Filament Options"))
      #if ENABLED(BABYSTEPPING)
       .tag(4).enabled(1)      .button( BTN_POS(1,4), BTN_SIZE(2,1), F("Nudge Nozzle"))
      #else
        #if ENABLED(HAS_BED_PROBE)
          .enabled(1)
        #else
          .enabled(0)
        #endif
       .tag(4)                 .button( BTN_POS(1,4), BTN_SIZE(2,1), F("Adjust Z-Offset"))
      #endif
       .tag(5).enabled(1)      .button( BTN_POS(1,5), BTN_SIZE(2,1), F("Print Speed"))
       .tag(isPrintingFromMediaPaused() ? 7 : 6)
      #if ENABLED(SDSUPPORT)
        .enabled(isPrintingFromMedia())
      #else
        .enabled(0)
      #endif
        .button( BTN_POS(1,6), BTN_SIZE(2,1), isPrintingFromMediaPaused() ? F("Resume Print") : F("Pause Print"))
      #if ENABLED(SDSUPPORT)
        .enabled(isPrintingFromMedia())
      #else
        .enabled(0)
      #endif
      .tag(8)             .button( BTN_POS(1,7), BTN_SIZE(2,1), F("Cancel Print"))
      .tag(1).style(STYLE_LIGHT_BTN)
                          .button( BTN_POS(1,8), BTN_SIZE(2,1), F("Back"));
    #else // USE_PORTRAIT_ORIENTATION
       .tag(2).enabled(1) .button( BTN_POS(1,1), BTN_SIZE(1,1), F("Temperature"))
       .tag(3).enabled(1) .button( BTN_POS(1,2), BTN_SIZE(1,1), F("Change Filament"))
      #if ENABLED(BABYSTEPPING)
       .enabled(1)
      #else
       .enabled(0)
      #endif
        #if ENABLED(BABYSTEPPING)
          .tag(4)         .button( BTN_POS(2,1), BTN_SIZE(1,1), F("Nudge Nozzle"))
        #else
          #if ENABLED(HAS_BED_PROBE)
            .enabled(1)
          #else
            .enabled(0)
          #endif
          .tag(4)         .button( BTN_POS(1,4), BTN_SIZE(2,1), F("Adjust Z-Offset"))
        #endif
       .tag(5).enabled(1) .button( BTN_POS(2,2), BTN_SIZE(1,1), F("Print Speed"))
       .tag(isPrintingFromMediaPaused() ? 7 : 6)
      #if ENABLED(SDSUPPORT)
        .enabled(isPrintingFromMedia())
      #else
        .enabled(0)
      #endif
                          .button( BTN_POS(1,3), BTN_SIZE(1,1), isPrintingFromMediaPaused() ? F("Resume Print") : F("Pause Print"))
      #if ENABLED(SDSUPPORT)
        .enabled(isPrintingFromMedia())
      #else
        .enabled(0)
      #endif
       .tag(8).           button( BTN_POS(2,3), BTN_SIZE(1,1), F("Cancel Print"))
       #if ENABLED(LIN_ADVANCE) || ENABLED(FILAMENT_RUNOUT_SENSOR)
          .enabled(1)
        #else
          .enabled(0)
        #endif
       .tag(9).button( BTN_POS(1,4), BTN_SIZE(1,1), F("Filament Options"))
       .tag(1).style(STYLE_LIGHT_BTN) .button( BTN_POS(2,4), BTN_SIZE(1,1), F("Back"));
    #endif
  }
  #undef GRID_COLS
  #undef GRID_ROWS
}

bool TuneScreen::onTouchEnd(uint8_t tag) {
  using namespace ExtUI;
  switch(tag) {
    case 1:  GOTO_PREVIOUS();                    break;
    case 2:  GOTO_SCREEN(TemperatureScreen);     break;
    case 3:  GOTO_SCREEN(ChangeFilamentScreen);  break;
    case 4:
      #if ENABLED(BABYSTEPPING)
        GOTO_SCREEN(AdjustOffsetsScreen);
      #else
        #if ENABLED(HAS_BED_PROBE)
          GOTO_SCREEN(ZOffsetScreen);
        #endif
      #endif
      break;
    case 5:  GOTO_SCREEN(MaxFeedrateScreen);     break;
    case 6:  sound.play(twinkle, PLAY_ASYNCHRONOUS); ExtUI::pausePrint();  GOTO_SCREEN(StatusScreen); break;
    case 7:  sound.play(twinkle, PLAY_ASYNCHRONOUS); ExtUI::resumePrint(); GOTO_SCREEN(StatusScreen); break;
    case 8:  GOTO_SCREEN(ConfirmAbortPrint);     break;
    #if ENABLED(LIN_ADVANCE) || ENABLED(FILAMENT_RUNOUT_SENSOR)
    case 9:  GOTO_SCREEN(FilamentOptionsScreen); break;
    #endif
    default:
      return false;
  }
  return true;
}

/******************************* CONFIGURATION SCREEN ****************************/

void AdvancedSettingsScreen::onRedraw(draw_mode_t what) {
  if(what & BACKGROUND) {
    CommandProcessor cmd;
    cmd.cmd(CLEAR_COLOR_RGB(Theme::background))
       .cmd(CLEAR(true,true,true));
  }

  if(what & FOREGROUND) {
    CommandProcessor cmd;
    default_button_colors();
    cmd.font(Theme::font_medium)
    #if defined(USE_PORTRAIT_ORIENTATION)
      #define GRID_ROWS 7
      #define GRID_COLS 2
      #if HAS_BED_PROBE
        .enabled(1)
      #else
        .enabled(0)
      #endif
      .tag(2) .button( BTN_POS(1,1), BTN_SIZE(1,1), F("Z Offset "))
      .enabled(1)
      .tag(3) .button( BTN_POS(1,2), BTN_SIZE(1,1), F("Steps/mm"))
      #if HOTENDS > 1
      .enabled(1)
      #else
      .enabled(0)
      #endif
      .tag(4) .button( BTN_POS(1,4), BTN_SIZE(1,1), F("Nozzle Offsets"))
      #if ENABLED(LIN_ADVANCE) || ENABLED(FILAMENT_RUNOUT_SENSOR)
      .enabled(1)
      #else
      .enabled(0)
      #endif
      .tag(11).button( BTN_POS(1,3), BTN_SIZE(1,1), F("Filament"))
      .tag(9) .button( BTN_POS(1,5), BTN_SIZE(2,1), F("Interface Settings"))
      .tag(10).button( BTN_POS(1,6), BTN_SIZE(2,1), F("Restore Factory Defaults"))
      .tag(5) .button( BTN_POS(2,1), BTN_SIZE(1,1), F("Velocity "))
      .tag(6) .button( BTN_POS(2,2), BTN_SIZE(1,1), F("Acceleration"))
      #if ENABLED(JUNCTION_DEVIATION)
      .tag(7) .button( BTN_POS(2,3), BTN_SIZE(1,1), F("Junc Dev"))
      #else
      .tag(7) .button( BTN_POS(2,3), BTN_SIZE(1,1), F("Jerk"))
      #endif
      #if ENABLED(BACKLASH_GCODE)
      .enabled(1)
      #else
      .enabled(0)
      #endif
      .tag(8).button( BTN_POS(2,4), BTN_SIZE(1,1), F("Axis Backlash"))
      .style(STYLE_LIGHT_BTN)
      .tag(1) .button( BTN_POS(1,7), BTN_SIZE(2,1), F("Back"));
      #undef GRID_COLS
      #undef GRID_ROWS
    #else
      #define GRID_ROWS 6
      #define GRID_COLS 2
      #if HAS_BED_PROBE
        .enabled(1)
      #else
        .enabled(0)
      #endif
      .tag(2) .button( BTN_POS(1,1),  BTN_SIZE(1,1), F("Z Offset "))
      .enabled(1)
      .tag(3) .button( BTN_POS(1,2),  BTN_SIZE(1,1), F("Steps/mm"))
      #if ENABLED(BACKLASH_GCODE)
      .enabled(1)
      #else
      .enabled(0)
      #endif
      .tag(8).button( BTN_POS(1,3),  BTN_SIZE(1,1), F("Axis Backlash"))
      #if HOTENDS > 1
      .enabled(1)
      #else
      .enabled(0)
      #endif
      .tag(4) .button( BTN_POS(1,4),  BTN_SIZE(1,1), F("Nozzle Offsets"))
      .tag(5) .button( BTN_POS(2,1),  BTN_SIZE(1,1), F("Velocity "))
      .tag(6) .button( BTN_POS(2,2),  BTN_SIZE(1,1), F("Acceleration"))
      #if ENABLED(JUNCTION_DEVIATION)
      .tag(7) .button( BTN_POS(2,3),  BTN_SIZE(1,1), F("Junc Dev"))
      #else
      .tag(7) .button( BTN_POS(2,3),  BTN_SIZE(1,1), F("Jerk"))
      #endif
      .tag(11).button( BTN_POS(2,4), BTN_SIZE(1,1),  F("Filament"))
      .tag(9).button(  BTN_POS(2,5),  BTN_SIZE(1,1), F("Interface Settings"))
      .tag(10).button( BTN_POS(1,5),  BTN_SIZE(1,1), F("Restore Defaults"))
      .style(STYLE_LIGHT_BTN)
      .tag(1) .button( BTN_POS(1,6),  BTN_SIZE(2,1), F("Back"));
      #undef GRID_COLS
      #undef GRID_ROWS
    #endif
  }
}

bool AdvancedSettingsScreen::onTouchEnd(uint8_t tag) {
  using namespace ExtUI;

  switch(tag) {
    case 1: SaveSettingsScreen::promptToSaveSettings(); break;
    #if HAS_BED_PROBE
    case 2:  GOTO_SCREEN(ZOffsetScreen);              break;
    #endif
    case 3:  GOTO_SCREEN(StepsScreen);                break;
    #if HOTENDS > 1
    case 4:  GOTO_SCREEN(NozzleOffsetScreen);         break;
    #endif
    case 5:  GOTO_SCREEN(MaxVelocityScreen);          break;
    case 6:  GOTO_SCREEN(DefaultAccelerationScreen);  break;
    case 7:
      #if ENABLED(JUNCTION_DEVIATION)
        GOTO_SCREEN(JunctionDeviationScreen);
      #else
        GOTO_SCREEN(JerkScreen);
      #endif
      break;
    #if ENABLED(BACKLASH_GCODE)
    case 8:  GOTO_SCREEN(BacklashCompensationScreen); break;
    #endif
    #if ENABLED(LIN_ADVANCE) || ENABLED(FILAMENT_RUNOUT_SENSOR)
    case 11: GOTO_SCREEN(FilamentOptionsScreen); break;
    #endif
    case 9:  GOTO_SCREEN(InterfaceSettingsScreen); LockScreen::check_passcode(); break;
    case 10: GOTO_SCREEN(RestoreFailsafeScreen);   LockScreen::check_passcode(); break;
    default: return false;
  }
  return true;
}

/************************* CHANGE FILAMENT SCREEN *****************************/

#define COOL_TEMP  40
#define LOW_TEMP  180
#define MED_TEMP  200
#define HIGH_TEMP 220

//#define _STRINGIFY(v) #v
//#define STRINGIFY(v) _STRINGIFY(v)

void ChangeFilamentScreen::drawTempGradient(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
  CommandProcessor cmd;
  cmd.cmd(SCISSOR_XY   (x, y))
     .cmd(SCISSOR_SIZE (w, h/2))
     .gradient         (x, y,     Theme::high_rgb, x, y+h/2, Theme::med_rgb)
     .cmd(SCISSOR_XY   (x, y+h/2))
     .cmd(SCISSOR_SIZE (w, h/2))
     .gradient         (x, y+h/2, Theme::med_rgb,  x, y+h, Theme::low_rgb)
     .cmd(SCISSOR_XY   ())
     .cmd(SCISSOR_SIZE ());
}

void ChangeFilamentScreen::onEntry() {
  screen_data.ChangeFilamentScreen.e_tag = ExtUI::getActiveTool() + 10;
  screen_data.ChangeFilamentScreen.t_tag = 0;
  screen_data.ChangeFilamentScreen.repeat_tag = 0;
}

void ChangeFilamentScreen::onRedraw(draw_mode_t what) {
  using namespace ExtUI;
  CommandProcessor cmd;

  #if defined(USE_PORTRAIT_ORIENTATION)
    #define GRID_COLS 2
    #define GRID_ROWS 11
  #else
    #define GRID_COLS 4
    #define GRID_ROWS 6
  #endif

  if(what & BACKGROUND) {
    cmd.cmd(CLEAR_COLOR_RGB(Theme::background))
       .cmd(CLEAR(true,true,true))
    #if defined(USE_PORTRAIT_ORIENTATION)
       .font(Theme::font_large)
    #else
       .font(Theme::font_medium)
    #endif
       .text(BTN_POS(1,1), BTN_SIZE(2,1), F("Extruder Selection:"))
    #if defined(USE_PORTRAIT_ORIENTATION)
       .text(BTN_POS(1,7), BTN_SIZE(1,1), F("Current Temp:"))
    #else
       .text(BTN_POS(3,1), BTN_SIZE(2,1), F("Current Temp:"))
       .font(Theme::font_small)
    #endif
       .text(BTN_POS(1,3), BTN_SIZE(2,1), F("Removal Temp:"));
    drawTempGradient(BTN_POS(1,4), BTN_SIZE(1,3));
  }

  if(what & FOREGROUND) {
    char e_str[15];

    sprintf_P(
      e_str,
      PSTR("%-3d / %-3d C"),
      ROUND(getActualTemp_celsius(getExtruder())),
      ROUND(getTargetTemp_celsius(getExtruder()))
    );

    const rgb_t tcol = Theme::getWarmColor(getActualTemp_celsius(getExtruder()), COOL_TEMP, LOW_TEMP, MED_TEMP, HIGH_TEMP);
    cmd.cmd(COLOR_RGB(tcol))
       .tag(15)
    #if defined(USE_PORTRAIT_ORIENTATION)
       .rectangle(BTN_POS(2,7), BTN_SIZE(1,1))
    #else
       .rectangle(BTN_POS(3,2), BTN_SIZE(2,1))
    #endif
       .cmd(COLOR_RGB(tcol.luminance() > 128 ? 0x000000 : 0xFFFFFF))
       .font(Theme::font_medium)
    #if defined(USE_PORTRAIT_ORIENTATION)
       .text   (BTN_POS(2,7), BTN_SIZE(1,1), e_str);
    #else
       .text   (BTN_POS(3,2), BTN_SIZE(2,1), e_str);
    #endif

    default_button_colors();

    const bool t_ok = getActualTemp_celsius(getExtruder()) > getSoftenTemp() - 10;

    if(screen_data.ChangeFilamentScreen.t_tag) {
      if(t_ok) {
        cmd.cmd(COLOR_RGB(0xFF0000)).text(BTN_POS(1,4), BTN_SIZE(1,1), F("Caution:"));
        default_button_colors();
        cmd.text(BTN_POS(1,6), BTN_SIZE(1,1), F("Hot!"));
      } else {
        cmd.text(BTN_POS(1,6), BTN_SIZE(1,1), F("Heating..."));
      }
    }

    const uint32_t tog2  = screen_data.ChangeFilamentScreen.t_tag == 2  ? STYLE_LIGHT_BTN : 0;
    const uint32_t tog3  = screen_data.ChangeFilamentScreen.t_tag == 3  ? STYLE_LIGHT_BTN : 0;
    const uint32_t tog4  = screen_data.ChangeFilamentScreen.t_tag == 4  ? STYLE_LIGHT_BTN : 0;
    const uint32_t tog10 = screen_data.ChangeFilamentScreen.e_tag == 10 ? STYLE_LIGHT_BTN : 0;
    #if HOTENDS > 1
    const uint32_t tog11 = screen_data.ChangeFilamentScreen.e_tag == 11 ? STYLE_LIGHT_BTN : 0;
    #endif

    #if defined(USE_PORTRAIT_ORIENTATION)
      cmd.font(Theme::font_large)
    #else
      cmd.font(Theme::font_medium)
    #endif
       .tag(10).style(tog10)               .button (BTN_POS(1,2), BTN_SIZE(1,1), F("1"))
    #if HOTENDS < 2
       .enabled(false)
    #else
       .style(tog11)
    #endif
       .tag(11)               .button (BTN_POS(2,2), BTN_SIZE(1,1), F("2"))
       .style(0);

    if(!t_ok) reset_menu_timeout();

    const uint16_t tag7_style = screen_data.ChangeFilamentScreen.repeat_tag == 7 ? STYLE_LIGHT_BTN : 0;
    const uint16_t tag8_style = screen_data.ChangeFilamentScreen.repeat_tag == 8 ? STYLE_LIGHT_BTN : 0;

    #if defined(USE_PORTRAIT_ORIENTATION)
      cmd.font(Theme::font_large)
    #else
      cmd.font(Theme::font_small)
    #endif
       .tag(2) .style(tog2) .button (BTN_POS(2,6), BTN_SIZE(1,1), F( STRINGIFY(LOW_TEMP)  "C (PLA)"))
       .tag(3) .style(tog3) .button (BTN_POS(2,5), BTN_SIZE(1,1), F( STRINGIFY(MED_TEMP)  "C (ABS)"))
       .tag(4) .style(tog4) .button (BTN_POS(2,4), BTN_SIZE(1,1), F( STRINGIFY(HIGH_TEMP) "C (High)"))
       .style(0)

    // Add tags to color gradient
    .cmd(COLOR_MASK(0,0,0,0))
    .tag(2) .rectangle(BTN_POS(1,6), BTN_SIZE(1,1))
    .tag(3) .rectangle(BTN_POS(1,5), BTN_SIZE(1,1))
    .tag(4) .rectangle(BTN_POS(1,4), BTN_SIZE(1,1))
    .cmd(COLOR_MASK(1,1,1,1))

    .cmd(COLOR_RGB(t_ok ? Theme::text_enabled : Theme::text_disabled))
    #if defined(USE_PORTRAIT_ORIENTATION)
       .font(Theme::font_large)
                                              .text   (BTN_POS(1,8),  BTN_SIZE(1,1), F("Unload"))
                                              .text   (BTN_POS(2,8),  BTN_SIZE(1,1), F("Load/Extrude"))
       .tag(5)                  .enabled(t_ok).button (BTN_POS(1,9),  BTN_SIZE(1,1), F("Momentary"))
       .tag(6)                  .enabled(t_ok).button (BTN_POS(2,9),  BTN_SIZE(1,1), F("Momentary"))
       .tag(7).style(tag7_style).enabled(t_ok).button (BTN_POS(1,10), BTN_SIZE(1,1), F("Continuous"))
       .tag(8).style(tag8_style).enabled(t_ok).button (BTN_POS(2,10), BTN_SIZE(1,1), F("Continuous"))
       .tag(1).style(STYLE_LIGHT_BTN)         .button (BTN_POS(1,11), BTN_SIZE(2,1), F("Back"));
    #else
       .font(Theme::font_small)
                                              .text   (BTN_POS(3,3), BTN_SIZE(1,1), F("Unload"))
                                              .text   (BTN_POS(4,3), BTN_SIZE(1,1), F("Load/Extrude"))
       .tag(5)                  .enabled(t_ok).button (BTN_POS(3,4), BTN_SIZE(1,1), F("Momentary"))
       .tag(6)                  .enabled(t_ok).button (BTN_POS(4,4), BTN_SIZE(1,1), F("Momentary"))
       .tag(7).style(tag7_style).enabled(t_ok).button (BTN_POS(3,5), BTN_SIZE(1,1), F("Continuous"))
       .tag(8).style(tag8_style).enabled(t_ok).button (BTN_POS(4,5), BTN_SIZE(1,1), F("Continuous"))
       .font(Theme::font_medium)
       .tag(1).style(STYLE_LIGHT_BTN)         .button (BTN_POS(3,6), BTN_SIZE(2,1), F("Back"));
    #endif
  }
  #undef GRID_COLS
  #undef GRID_ROWS
}

uint8_t ChangeFilamentScreen::getSoftenTemp() {
  switch(screen_data.ChangeFilamentScreen.t_tag) {
    case 2:  return LOW_TEMP;
    case 3:  return MED_TEMP;
    case 4:  return HIGH_TEMP;
    default: return EXTRUDE_MINTEMP;
  }
}

ExtUI::extruder_t ChangeFilamentScreen::getExtruder() {
  switch(screen_data.ChangeFilamentScreen.e_tag) {
    case 13: return ExtUI::E3;
    case 12: return ExtUI::E2;
    case 11: return ExtUI::E1;
    default: return ExtUI::E0;
  }
}

bool ChangeFilamentScreen::onTouchEnd(uint8_t tag) {
  using namespace ExtUI;
  switch(tag) {
    case 1:  GOTO_PREVIOUS();                      break;
    case 2:
    case 3:
    case 4:
      // Change temperature
      screen_data.ChangeFilamentScreen.t_tag = tag;
      setTargetTemp_celsius(getSoftenTemp(), getExtruder());
      break;
    case 7: screen_data.ChangeFilamentScreen.repeat_tag = (screen_data.ChangeFilamentScreen.repeat_tag == 7) ? 0 : 7; break;
    case 8: screen_data.ChangeFilamentScreen.repeat_tag = (screen_data.ChangeFilamentScreen.repeat_tag == 8) ? 0 : 8; break;
    case 10:
    case 11:
      // Change extruder
      screen_data.ChangeFilamentScreen.e_tag      = tag;
      screen_data.ChangeFilamentScreen.t_tag      = 0;
      screen_data.ChangeFilamentScreen.repeat_tag = 0;
      setActiveTool(getExtruder(), true);
      break;
    case 15: GOTO_SCREEN(TemperatureScreen); break;
  }
  return true;
}

bool ChangeFilamentScreen::onTouchHeld(uint8_t tag) {
  if(ExtUI::isMoving()) return false; // Don't allow moves to accumulate
  constexpr float increment = 1;
  #define UI_INCREMENT_AXIS(axis) MoveAxisScreen::setManualFeedrate(axis, increment); UI_INCREMENT(AxisPosition_mm, axis);
  #define UI_DECREMENT_AXIS(axis) MoveAxisScreen::setManualFeedrate(axis, increment); UI_DECREMENT(AxisPosition_mm, axis);
  switch(tag) {
    case  5: case  7: UI_DECREMENT_AXIS(getExtruder()); break;
    case  6: case  8: UI_INCREMENT_AXIS(getExtruder()); break;
    default: return false;
  }
  #undef UI_DECREMENT_AXIS
  #undef UI_INCREMENT_AXIS
  return true;
}

void ChangeFilamentScreen::onIdle() {
  if(screen_data.ChangeFilamentScreen.repeat_tag) onTouchHeld(screen_data.ChangeFilamentScreen.repeat_tag);
  if(refresh_timer.elapsed(STATUS_UPDATE_INTERVAL)) {
    onRefresh();
    refresh_timer.start();
  }
  BaseScreen::onIdle();
}

/******************************** CALIBRATION SCREEN ****************************/

void CalibrationScreen::onEntry() {
  // Clear the display
  CommandProcessor cmd;
  cmd.cmd(CMD_DLSTART)
     .cmd(CLEAR_COLOR_RGB(Theme::background))
     .cmd(CLEAR(true,true,true))
     .cmd(DL::DL_DISPLAY)
     .cmd(CMD_SWAP)
     .execute();

  // Wait for the touch to release before starting,
  // as otherwise the first calibration point could
  // be misinterpreted.
  while(CLCD::is_touching()) {
    #if defined(UI_FRAMEWORK_DEBUG)
      SERIAL_ECHO_START();
      SERIAL_ECHOLNPGM("Waiting for touch release");
    #endif
  }
  BaseScreen::onEntry();
}

void CalibrationScreen::onRedraw(draw_mode_t what) {
  CommandProcessor cmd;
  cmd.cmd(CLEAR_COLOR_RGB(Theme::background))
     .cmd(CLEAR(true,true,true))
  #define GRID_COLS 4
  #define GRID_ROWS 16
  #if defined(USE_PORTRAIT_ORIENTATION)
    .font(Theme::font_large)
    .text  ( BTN_POS(1,8), BTN_SIZE(4,1), F("Touch the dots"))
    .text  ( BTN_POS(1,9), BTN_SIZE(4,1), F("to calibrate"))
  #else
    .font(
      #if defined(LCD_800x480)
        Theme::font_large
      #else
        Theme::font_medium
      #endif
    )
    .text  ( BTN_POS(1,1), BTN_SIZE(4,16), F("Touch the dots to calibrate"))
  #endif
  #undef GRID_COLS
  #undef GRID_ROWS
    .cmd(CMD_CALIBRATE);
}

void CalibrationScreen::onIdle() {
  if(!CommandProcessor::is_processing()) {
    GOTO_SCREEN(StatusScreen);
  }
}

/*************************** GENERIC VALUE ADJUSTMENT SCREEN ******************************/

#if defined(USE_PORTRAIT_ORIENTATION)
  #define GRID_COLS 13
  #define GRID_ROWS 10
#else
  #define GRID_COLS 18
  #define GRID_ROWS  7
#endif

ValueAdjusters::widgets_t::widgets_t(draw_mode_t what) : _what(what) {
  if(what & BACKGROUND) {
    CommandProcessor cmd;
    cmd.cmd(CLEAR_COLOR_RGB(Theme::background))
       .cmd(CLEAR(true,true,true));
  }

  if(what & FOREGROUND) {
    CommandProcessor cmd;
    cmd.font(Theme::font_medium)
    #if defined(USE_PORTRAIT_ORIENTATION)
       .style(STYLE_LIGHT_BTN).tag(1).button( BTN_POS(1,10), BTN_SIZE(13,1), F("Back"));
    #else
       .style(STYLE_LIGHT_BTN).tag(1).button( BTN_POS(15,7), BTN_SIZE(4,1),  F("Back"));
    #endif
  }

  _line = 1;
  _units = PSTR("");
}

ValueAdjusters::widgets_t &ValueAdjusters::widgets_t::precision(uint8_t decimals, precision_default_t initial) {
  _decimals = decimals;
  if(screen_data.ValueAdjusters.increment == 0) {
    screen_data.ValueAdjusters.increment = 243 + (initial - DEFAULT_LOWEST) - _decimals;
  }
  return *this;
}

void ValueAdjusters::widgets_t::heading(const char *label) {
  CommandProcessor cmd;
  cmd.font(Theme::font_medium);
  if(_what & BACKGROUND) {
    #if defined(USE_PORTRAIT_ORIENTATION)
      cmd.tag(0).fgcolor(Theme::background).button( BTN_POS(1, _line), BTN_SIZE(12,1), progmem_str(label), OPT_FLAT);
    #else
      cmd.tag(0).fgcolor(Theme::background).button( BTN_POS(5, _line), BTN_SIZE(8,1),  progmem_str(label), OPT_FLAT);
    #endif
  }

  _line++;
}

#if defined(USE_PORTRAIT_ORIENTATION)
  #if defined(LCD_800x480)
    #undef EDGE_R
    #define EDGE_R 20
  #else
    #undef EDGE_R
    #define EDGE_R 10
  #endif
#endif

void ValueAdjusters::widgets_t::_draw_increment_btn(uint8_t line, const uint8_t tag) {
  CommandProcessor  cmd;
  const char        *label = PSTR("?");
  uint8_t            pos;

  if(screen_data.ValueAdjusters.increment == 0) {
    screen_data.ValueAdjusters.increment = tag; // Set the default value to be the first.
  }

  if(screen_data.ValueAdjusters.increment == tag) {
    cmd.style(STYLE_LIGHT_BTN);
  }

  switch(tag) {
    case 240: label = PSTR(   ".001"); pos = _decimals - 3; break;
    case 241: label = PSTR(   ".01" ); pos = _decimals - 2; break;
    case 242: label = PSTR(  "0.1"  ); pos = _decimals - 1; break;
    case 243: label = PSTR(  "1"    ); pos = _decimals + 0; break;
    case 244: label = PSTR( "10"    ); pos = _decimals + 1; break;
    default:  label = PSTR("100"    ); pos = _decimals + 2; break;
  }

  cmd.tag(tag)
  #if defined(USE_PORTRAIT_ORIENTATION)
    .font(Theme::font_small);
  #else
    .font(Theme::font_medium);
  #endif
  switch(pos) {
    #if defined(USE_PORTRAIT_ORIENTATION)
      case 0: cmd.button( BTN_POS(5,_line), BTN_SIZE(2,1), progmem_str(label)); break;
      case 1: cmd.button( BTN_POS(7,_line), BTN_SIZE(2,1), progmem_str(label)); break;
      case 2: cmd.button( BTN_POS(9,_line), BTN_SIZE(2,1), progmem_str(label)); break;
    #else
      case 0: cmd.button( BTN_POS(15,2),    BTN_SIZE(4,1), progmem_str(label)); break;
      case 1: cmd.button( BTN_POS(15,3),    BTN_SIZE(4,1), progmem_str(label)); break;
      case 2: cmd.button( BTN_POS(15,4),    BTN_SIZE(4,1), progmem_str(label)); break;
    #endif
  }
}

void ValueAdjusters::widgets_t::increments() {
  if(_what & BACKGROUND) {
    CommandProcessor cmd;
    cmd.fgcolor(Theme::background)
       .tag(0)
    #if defined(USE_PORTRAIT_ORIENTATION)
       .font(Theme::font_small).button( BTN_POS(1, _line),  BTN_SIZE(4,1), F("Increment:"), OPT_FLAT);
    #else
       .font(Theme::font_medium).button( BTN_POS(15,1),     BTN_SIZE(4,1), F("Increment:"), OPT_FLAT);
    #endif
  }

  if(_what & FOREGROUND) {
      _draw_increment_btn(_line+1, 245 - _decimals);
      _draw_increment_btn(_line+1, 244 - _decimals);
      _draw_increment_btn(_line+1, 243 - _decimals);
  }

  #if defined(USE_PORTRAIT_ORIENTATION)
  _line++;
  #endif
}

void ValueAdjusters::widgets_t::adjuster_sram_val(uint8_t tag, const char *label, const char *value, bool is_enabled) {
  CommandProcessor cmd;

  if(_what & BACKGROUND) {
    cmd.enabled(1)
       .font(Theme::font_small)
       .fgcolor(_color)            .tag(0).button( BTN_POS(5,_line), BTN_SIZE(5,1), F(""),               OPT_FLAT)
       .fgcolor(Theme::background) .tag(0).button( BTN_POS(1,_line), BTN_SIZE(4,1), (progmem_str) label, OPT_FLAT);
  }

  if(_what & FOREGROUND) {
    default_button_colors();
    cmd.font(Theme::font_medium)
       .tag(is_enabled ? tag   : 0).enabled(is_enabled).button( BTN_POS(10,_line), BTN_SIZE(2,1),  F("-"))
       .tag(is_enabled ? tag+1 : 0).enabled(is_enabled).button( BTN_POS(12,_line), BTN_SIZE(2,1),  F("+"));
    cmd.tag(0).font(Theme::font_small).text ( BTN_POS(5,_line), BTN_SIZE(5,1), is_enabled ? value : "-");
  }

  _line++;
}

void ValueAdjusters::widgets_t::adjuster(uint8_t tag, const char *label, const char *value, bool is_enabled) {
  if(_what & BACKGROUND) {
    adjuster_sram_val(tag, label, nullptr);
  }

  if(_what & FOREGROUND) {
    char b[strlen_P(value)+1];
    strcpy_P(b,value);
    adjuster_sram_val(tag, label, b, is_enabled);
  }
}

void ValueAdjusters::widgets_t::adjuster(uint8_t tag, const char *label, float value, bool is_enabled) {
  if(_what & BACKGROUND) {
    adjuster_sram_val(tag, label, nullptr);
  }

  if(_what & FOREGROUND) {
    char b[32];
    dtostrf(value, 5, _decimals, b);
    strcat_P(b, PSTR(" "));
    strcat_P(b, (const char*) _units);
    adjuster_sram_val(tag, label, b, is_enabled);
  }
}

void ValueAdjusters::widgets_t::button(uint8_t tag, const char *label, bool is_enabled) {
  if(_what & FOREGROUND) {
    CommandProcessor cmd;
    cmd.tag(is_enabled ? tag   : 0)
       .enabled(is_enabled)
    #if defined(USE_PORTRAIT_ORIENTATION)
       .font(Theme::font_small)
    #else
       .font(Theme::font_medium)
    #endif
    .button(BTN_POS(5,_line), BTN_SIZE(9,1), progmem_str(label));
  }

  _line++;
}

void ValueAdjusters::widgets_t::text_field(uint8_t tag, const char *label, const char *value, bool is_enabled) {
  CommandProcessor cmd;

  if(_what & BACKGROUND) {
    cmd.enabled(1)
       .font(Theme::font_small)
       .fgcolor(_color)            .tag(0).button( BTN_POS(5,_line), BTN_SIZE(9,1), F(""),               OPT_FLAT)
       .fgcolor(Theme::background) .tag(0).button( BTN_POS(1,_line), BTN_SIZE(4,1), (progmem_str) label, OPT_FLAT);
  }

  if(_what & FOREGROUND) {
    cmd.font(Theme::font_medium)
       .tag(tag).font(Theme::font_small).text ( BTN_POS(5,_line), BTN_SIZE(9,1), is_enabled ? value : "-");
  }

  _line++;
}

void ValueAdjusters::widgets_t::two_buttons(uint8_t tag1, const char *label1, uint8_t tag2, const char *label2, bool is_enabled) {
  if(_what & FOREGROUND) {
    CommandProcessor cmd;
    cmd.enabled(is_enabled)
    #if defined(USE_PORTRAIT_ORIENTATION)
       .font(Theme::font_small)
    #else
       .font(Theme::font_medium)
    #endif
    .tag(is_enabled ? tag1: 0).button(BTN_POS(5,_line),   BTN_SIZE(4.5,1), progmem_str(label1))
    .tag(is_enabled ? tag2: 0).button(BTN_POS(9.5,_line), BTN_SIZE(4.5,1), progmem_str(label2));
  }

  _line++;
}

void ValueAdjusters::widgets_t::toggle(uint8_t tag, const char *label, const char *text, bool value, bool is_enabled) {
  if(_what & BACKGROUND) {
    CommandProcessor cmd;
    cmd.fgcolor(Theme::background)
       .tag(0)
       .font(Theme::font_small)
    #if defined(USE_PORTRAIT_ORIENTATION)
       .button( BTN_POS(1, _line), BTN_SIZE( 8,1), progmem_str(label), OPT_FLAT);
    #else
       .button( BTN_POS(1, _line), BTN_SIZE(10,1), progmem_str(label), OPT_FLAT);
    #endif
  }

  if(_what & FOREGROUND) {
    CommandProcessor cmd;
    cmd.tag(is_enabled ? tag   : 0)
       .enabled(is_enabled)
       .font(Theme::font_small)
    #if defined(USE_PORTRAIT_ORIENTATION)
      .toggle(BTN_POS( 9,_line), BTN_SIZE(5,1), progmem_str(text), value);
    #else
      .toggle(BTN_POS(10,_line), BTN_SIZE(4,1), progmem_str(text), value);
    #endif
  }

  _line++;
}

void ValueAdjusters::widgets_t::home_buttons(uint8_t tag) {
  if(_what & BACKGROUND) {
    CommandProcessor cmd;
    cmd.fgcolor(Theme::background)
       .tag(0)
       .font(Theme::font_small)
       .button( BTN_POS(1, _line),  BTN_SIZE(4,1), F("Home:"), OPT_FLAT);
  }

  if(_what & FOREGROUND) {
    CommandProcessor cmd;
    cmd
    #if defined(USE_PORTRAIT_ORIENTATION)
       .font(Theme::font_small)
    #else
       .font(Theme::font_medium)
    #endif
       .tag(tag+0).button(BTN_POS(5,_line),  BTN_SIZE(2,1), F("X"))
       .tag(tag+1).button(BTN_POS(7,_line),  BTN_SIZE(2,1), F("Y"))
       .tag(tag+2).button(BTN_POS(9,_line),  BTN_SIZE(2,1), F("Z"))
       .tag(tag+3).button(BTN_POS(11,_line), BTN_SIZE(3,1), F("All"));
  }

  _line++;
}

#undef EDGE_R
#define EDGE_R 0

#undef GRID_COLS
#undef GRID_ROWS

void ValueAdjusters::onEntry() {
  screen_data.ValueAdjusters.increment = 0; // This will force the increment to be picked while drawing.
  BaseScreen::onEntry();
}

bool ValueAdjusters::onTouchEnd(uint8_t tag) {
  switch(tag) {
    case 1:           GOTO_PREVIOUS();                            return true;
    case 240 ... 245: screen_data.ValueAdjusters.increment = tag; break;
    default:          return current_screen.onTouchHeld(tag);
  }
  return true;
}

float ValueAdjusters::getIncrement() {
  switch(screen_data.ValueAdjusters.increment) {
    case 240: return   0.001;
    case 241: return   0.01;
    case 242: return   0.1;
    case 243: return   1.0;
    case 244: return  10.0;
    case 245: return 100.0;
    default:  return   0.0;
  }
}

/******************************** MOVE AXIS SCREEN ******************************/

void MoveAxisScreen::onEntry() {
  // Since Marlin keeps only one absolute position for all the extruders,
  // we have to keep track of the relative motion of individual extruders
  // ourselves. The relative distances are reset to zero whenever this
  // screen is entered.

  for(uint8_t i = 0; i < ExtUI::extruderCount; i++) {
    screen_data.MoveAxisScreen.e_rel[i] = 0;
  }
  ValueAdjusters::onEntry();
}

void MoveAxisScreen::onRedraw(draw_mode_t what) {
  using namespace ExtUI;
  widgets_t w(what);
  w.precision(1);
  w.units(PSTR("mm"));
  w.heading(                                PSTR("Move Axis"));
  w.home_buttons(20);
  w.color(Theme::x_axis  )   .adjuster(  2, PSTR("X:"),  getAxisPosition_mm(X), canMove(X));
  w.color(Theme::y_axis  )   .adjuster(  4, PSTR("Y:"),  getAxisPosition_mm(Y), canMove(Y));
  w.color(Theme::z_axis  )   .adjuster(  6, PSTR("Z:"),  getAxisPosition_mm(Z), canMove(Z));

  #if EXTRUDERS == 1
    w.color(Theme::e_axis)   .adjuster(  8, PSTR("E:"),  screen_data.MoveAxisScreen.e_rel[0], canMove(E0));
  #elif EXTRUDERS > 1
    w.color(Theme::e_axis)   .adjuster(  8, PSTR("E1:"), screen_data.MoveAxisScreen.e_rel[0], canMove(E0));
    w.color(Theme::e_axis)   .adjuster( 10, PSTR("E2:"), screen_data.MoveAxisScreen.e_rel[1], canMove(E1));
    #if EXTRUDERS > 2
      w.color(Theme::e_axis) .adjuster( 12, PSTR("E3:"), screen_data.MoveAxisScreen.e_rel[2], canMove(E2));
    #endif
    #if EXTRUDERS > 3
      w.color(Theme::e_axis) .adjuster( 14, PSTR("E4:"), screen_data.MoveAxisScreen.e_rel[3], canMove(E3));
    #endif
  #endif
  w.increments();
}

bool MoveAxisScreen::onTouchHeld(uint8_t tag) {
  using namespace ExtUI;
  #define UI_INCREMENT_AXIS(axis) setManualFeedrate(axis, increment); UI_INCREMENT(AxisPosition_mm, axis);
  #define UI_DECREMENT_AXIS(axis) setManualFeedrate(axis, increment); UI_DECREMENT(AxisPosition_mm, axis);
  const float increment = getIncrement();
  switch(tag) {
    case  2: UI_DECREMENT_AXIS(X); break;
    case  3: UI_INCREMENT_AXIS(X); break;
    case  4: UI_DECREMENT_AXIS(Y); break;
    case  5: UI_INCREMENT_AXIS(Y); break;
    case  6: UI_DECREMENT_AXIS(Z); break;
    case  7: UI_INCREMENT_AXIS(Z); break;
    // For extruders, also update relative distances.
    case  8: UI_DECREMENT_AXIS(E0); screen_data.MoveAxisScreen.e_rel[0] -= increment; break;
    case  9: UI_INCREMENT_AXIS(E0); screen_data.MoveAxisScreen.e_rel[0] += increment; break;
    #if EXTRUDERS > 1
    case 10: UI_DECREMENT_AXIS(E1); screen_data.MoveAxisScreen.e_rel[1] -= increment; break;
    case 11: UI_INCREMENT_AXIS(E1); screen_data.MoveAxisScreen.e_rel[1] += increment; break;
    #endif
    #if EXTRUDERS > 2
    case 12: UI_DECREMENT_AXIS(E2); screen_data.MoveAxisScreen.e_rel[2] -= increment; break;
    case 13: UI_INCREMENT_AXIS(E2); screen_data.MoveAxisScreen.e_rel[2] += increment; break;
    #endif
    #if EXTRUDERS > 3
    case 14: UI_DECREMENT_AXIS(E3); screen_data.MoveAxisScreen.e_rel[3] -= increment; break;
    case 15: UI_INCREMENT_AXIS(E3); screen_data.MoveAxisScreen.e_rel[3] += increment; break;
    #endif
    case 20: enqueueCommands_P(PSTR("G28 X")); break;
    case 21: enqueueCommands_P(PSTR("G28 Y")); break;
    case 22: enqueueCommands_P(PSTR("G28 Z")); break;
    case 23: enqueueCommands_P(PSTR("G28"));   break;
    default:
      return false;
  }
  #undef UI_DECREMENT_AXIS
  #undef UI_INCREMENT_AXIS
  return true;
}

float MoveAxisScreen::getManualFeedrate(uint8_t axis, float increment_mm) {
  // Compute feedrate so that the tool lags the adjuster when it is
  // being held down, this allows enough margin for the planner to
  // connect segments and even out the motion.
  constexpr float max_manual_feedrate[XYZE] = MAX_MANUAL_FEEDRATE;
  return min(max_manual_feedrate[axis]/60, abs(increment_mm * TOUCH_REPEATS_PER_SECOND * 0.80));
}

void MoveAxisScreen::setManualFeedrate(ExtUI::axis_t axis, float increment_mm) {
  ExtUI::setFeedrate_mm_s(getManualFeedrate(X_AXIS + (axis - ExtUI::X), increment_mm));
}

void MoveAxisScreen::setManualFeedrate(ExtUI::extruder_t extruder, float increment_mm) {
  ExtUI::setFeedrate_mm_s(getManualFeedrate(E_AXIS, increment_mm));
}

void MoveAxisScreen::onIdle() {
  if(refresh_timer.elapsed(STATUS_UPDATE_INTERVAL)) {
    onRefresh();
    refresh_timer.start();
  }
  BaseScreen::onIdle();
}

/******************************* TEMPERATURE SCREEN ******************************/

void TemperatureScreen::onRedraw(draw_mode_t what) {
  using namespace ExtUI;
  widgets_t w(what);
  w.precision(0).color(Theme::temp).units(PSTR("C"));
  w.heading(         PSTR("Temperature:"));
  w.button(30, PSTR("Cooldown (All Off)"));
  #if HOTENDS == 1
    w.adjuster(   2, PSTR("Hot End:"),   getTargetTemp_celsius(E0));
  #else
    w.adjuster(   2, PSTR("Hot End 1:"), getTargetTemp_celsius(E0));
    w.adjuster(   4, PSTR("Hot End 2:"), getTargetTemp_celsius(E1));
    #if HOTENDS > 2
      w.adjuster( 6, PSTR("Hot End 3:"), getTargetTemp_celsius(E2));
    #endif
    #if HOTENDS > 3
      w.adjuster( 8, PSTR("Hot End 4:"), getTargetTemp_celsius(E3));
    #endif
  #endif
  #if HAS_HEATED_BED
    w.adjuster(    20, PSTR("Bed:"),     getTargetTemp_celsius(BED));
  #endif
  #if FAN_COUNT > 0
    w.color(Theme::fan_speed).units(PSTR("%"));
    w.adjuster(    10, PSTR("Fan Speed:"), getTargetFan_percent(FAN0));
  #endif
  w.increments();
}

bool TemperatureScreen::onTouchHeld(uint8_t tag) {
  using namespace ExtUI;
  const float increment = getIncrement();
  switch(tag) {
    case 20: UI_DECREMENT(TargetTemp_celsius, BED); break;
    case 21: UI_INCREMENT(TargetTemp_celsius, BED); break;
    case  2: UI_DECREMENT(TargetTemp_celsius, E0); break;
    case  3: UI_INCREMENT(TargetTemp_celsius, E0); break;
    #if HOTENDS > 1
    case  4: UI_DECREMENT(TargetTemp_celsius, E1); break;
    case  5: UI_INCREMENT(TargetTemp_celsius, E1); break;
    #endif
    #if HOTENDS > 2
    case  6: UI_DECREMENT(TargetTemp_celsius, E2); break;
    case  7: UI_INCREMENT(TargetTemp_celsius, E2); break;
    #endif
    #if HOTENDS > 3
    case  8: UI_DECREMENT(TargetTemp_celsius, E3); break;
    case  9: UI_INCREMENT(TargetTemp_celsius, E3); break;
    #endif
    #if FAN_COUNT > 0
    case 10: UI_DECREMENT(TargetFan_percent, FAN0);      break;
    case 11: UI_INCREMENT(TargetFan_percent, FAN0);      break;
    #endif
    case 30:
      setTargetTemp_celsius(0,E0);
      #if HOTENDS > 1
        setTargetTemp_celsius(0,E1);
        #if HOTENDS > 2
          setTargetTemp_celsius(0,E2);
          #if HOTENDS > 3
            setTargetTemp_celsius(0,E4);
          #endif
        #endif
      #endif
      #if HAS_HEATED_BED
        setTargetTemp_celsius(0,BED);
      #endif
      #if FAN_COUNT > 0
        setTargetFan_percent(0,FAN0);
      #endif
      break;
    default:
      return false;
  }
  return true;
}

/******************************* STEPS SCREEN ******************************/

void StepsScreen::onRedraw(draw_mode_t what) {
  using namespace ExtUI;
  widgets_t w(what);
  w.precision(0);
  w.units(PSTR("st/mm"));
  w.heading(                               PSTR("Steps/mm"));
  w.color(Theme::x_axis)     .adjuster( 2, PSTR("X:"),  getAxisSteps_per_mm(X) );
  w.color(Theme::y_axis)     .adjuster( 4, PSTR("Y:"),  getAxisSteps_per_mm(Y) );
  w.color(Theme::z_axis)     .adjuster( 6, PSTR("Z:"),  getAxisSteps_per_mm(Z) );
  #if EXTRUDERS == 1 || DISABLED(DISTINCT_E_FACTORS)
    w.color(Theme::e_axis)   .adjuster( 8, PSTR("E:"),  getAxisSteps_per_mm(E0) );
  #elif EXTRUDERS > 1
    w.color(Theme::e_axis)   .adjuster( 8, PSTR("E1:"), getAxisSteps_per_mm(E0) );
    w.color(Theme::e_axis)   .adjuster(10, PSTR("E2:"), getAxisSteps_per_mm(E1) );
    #if EXTRUDERS > 2
      w.color(Theme::e_axis) .adjuster(12, PSTR("E3:"), getAxisSteps_per_mm(E2) );
    #endif
    #if EXTRUDERS > 3
      w.color(Theme::e_axis) .adjuster(14, PSTR("E4:"), getAxisSteps_per_mm(E3) );
    #endif
  #endif
  w.increments();
}

bool StepsScreen::onTouchHeld(uint8_t tag) {
  using namespace ExtUI;
  const float increment = getIncrement();
  switch(tag) {
    case  2: UI_DECREMENT(AxisSteps_per_mm, X);  break;
    case  3: UI_INCREMENT(AxisSteps_per_mm, X);  break;
    case  4: UI_DECREMENT(AxisSteps_per_mm, Y);  break;
    case  5: UI_INCREMENT(AxisSteps_per_mm, Y);  break;
    case  6: UI_DECREMENT(AxisSteps_per_mm, Z);  break;
    case  7: UI_INCREMENT(AxisSteps_per_mm, Z);  break;
    case  8: UI_DECREMENT(AxisSteps_per_mm, E0); break;
    case  9: UI_INCREMENT(AxisSteps_per_mm, E0); break;
    #if EXTRUDERS > 1
    case 10: UI_DECREMENT(AxisSteps_per_mm, E1); break;
    case 11: UI_INCREMENT(AxisSteps_per_mm, E1); break;
    #endif
    #if EXTRUDERS > 2
    case 12: UI_DECREMENT(AxisSteps_per_mm, E2); break;
    case 13: UI_INCREMENT(AxisSteps_per_mm, E2); break;
    #endif
    #if EXTRUDERS > 3
    case 14: UI_DECREMENT(AxisSteps_per_mm, E3); break;
    case 15: UI_INCREMENT(AxisSteps_per_mm, E3); break;
    #endif
    default:
      return false;
  }
  return true;
}

/***************************** Z-OFFSET SCREEN ***************************/

#if HAS_BED_PROBE
  void ZOffsetScreen::onRedraw(draw_mode_t what) {
    widgets_t w(what);
    w.precision(2, ValueAdjusters::DEFAULT_MIDRANGE).units(PSTR("mm"));

    w.heading(                          PSTR("Z Offset"));
    w.color(Theme::z_axis).adjuster(4,  PSTR("Z Offset:"), ExtUI::getZOffset_mm());
    w.increments();
  }

  bool ZOffsetScreen::onTouchHeld(uint8_t tag) {
    const float increment = getIncrement();
    switch(tag) {
      case 4: UI_DECREMENT(ZOffset_mm); break;
      case 5: UI_INCREMENT(ZOffset_mm); break;
      default:
        return false;
    }
    return true;
  }
#endif // HAS_BED_PROBE

/***************************** NOZZLE OFFSET SCREEN ***************************/

#if HOTENDS > 1
  void NozzleOffsetScreen::onEntry() {
    using namespace ExtUI;

    // Since we don't allow the user to edit the offsets for E0,
    // make sure they are all zero.
    normalizeNozzleOffset(X);
    normalizeNozzleOffset(Y);
    normalizeNozzleOffset(Z);
  }

  void NozzleOffsetScreen::onRedraw(draw_mode_t what) {
    using namespace ExtUI;
    widgets_t w(what);
    w.precision(2).units(PSTR("mm"));

    w.heading(                          PSTR("Nozzle Offset"));
    w.color(Theme::x_axis).adjuster(2,  PSTR("X:"), ExtUI::getNozzleOffset_mm(X, E1));
    w.color(Theme::y_axis).adjuster(4,  PSTR("Y:"), ExtUI::getNozzleOffset_mm(Y, E1));
    w.color(Theme::z_axis).adjuster(6,  PSTR("Z:"), ExtUI::getNozzleOffset_mm(Z, E1));
    #if ENABLED(LULZBOT_CALIBRATION_GCODE)
    w.button(8, PSTR("Measure automatically"), !isPrinting());
    #endif
    w.increments();
  }

  bool NozzleOffsetScreen::onTouchHeld(uint8_t tag) {
    using namespace ExtUI;
    const float increment = getIncrement();
    switch(tag) {
      case  2: UI_DECREMENT(NozzleOffset_mm, X, E1); break;
      case  3: UI_INCREMENT(NozzleOffset_mm, X, E1); break;
      case  4: UI_DECREMENT(NozzleOffset_mm, Y, E1); break;
      case  5: UI_INCREMENT(NozzleOffset_mm, Y, E1); break;
      case  6: UI_DECREMENT(NozzleOffset_mm, Z, E1); break;
      case  7: UI_INCREMENT(NozzleOffset_mm, Z, E1); break;
      #if ENABLED(LULZBOT_CALIBRATION_GCODE)
      case  8: GOTO_SCREEN(ConfirmAutoCalibration); return true;
      #endif
      default:
        return false;
    }
    return true;
  }
#endif // HOTENDS > 1

/***************************** ADJUST OFFSETS SCREEN ***************************/

#if ENABLED(BABYSTEPPING)
  void AdjustOffsetsScreen::onEntry() {
    screen_data.AdjustOffsetsScreen.show_offsets = false;
    #if EXTRUDERS > 1
      screen_data.AdjustOffsetsScreen.link_nozzles = true;
    #endif
    LOOP_XYZ(i) {
      screen_data.AdjustOffsetsScreen.rel[i] = 0;
    }
    ValueAdjusters::onEntry();
  }

  void AdjustOffsetsScreen::onRedraw(draw_mode_t what) {
    using namespace ExtUI;
    widgets_t w(what);
    w.precision(2, ValueAdjusters::DEFAULT_MIDRANGE).units(PSTR("mm"));

    w.heading(                          PSTR("Nudge Nozzle"));
    #if ENABLED(BABYSTEP_XY)
    w.color(Theme::x_axis).adjuster(2,  PSTR("X:"), screen_data.AdjustOffsetsScreen.rel[0] / getAxisSteps_per_mm(X));
    w.color(Theme::y_axis).adjuster(4,  PSTR("Y:"), screen_data.AdjustOffsetsScreen.rel[1] / getAxisSteps_per_mm(Y));
    #endif
    w.color(Theme::z_axis).adjuster(6,  PSTR("Z:"), screen_data.AdjustOffsetsScreen.rel[2] / getAxisSteps_per_mm(Z));
    w.increments();
    #if EXTRUDERS > 1
      w.toggle  (8,  PSTR("Adjust Both Nozzles:"), PSTR("no\xFFyes"), screen_data.AdjustOffsetsScreen.link_nozzles, PSTR("Yes\nNo"));
    #endif
    w.toggle  (9,  PSTR("Show Offsets:"), PSTR("no\xFFyes"), screen_data.AdjustOffsetsScreen.show_offsets, PSTR("Yes\nNo"));

    if(screen_data.AdjustOffsetsScreen.show_offsets) {
      char str[19], num1[7];
      dtostrf(getZOffset_mm(), 4, 2, num1);
      sprintf_P(str, PSTR("%s mm"), num1);

      w.draw_mode(BOTH);
      w.color(Theme::other);
      w.text_field  (0,  PSTR("Z Offset"), str);

      #if EXTRUDERS > 1
        char num2[7], num3[7];
        dtostrf(getNozzleOffset_mm(X, E1), 4, 2, num1);
        dtostrf(getNozzleOffset_mm(Y, E1), 4, 2, num2);
        dtostrf(getNozzleOffset_mm(Z, E1), 4, 2, num3);
        sprintf_P(str, PSTR("%s; %s; %s mm"), num1, num2, num3);
        w.text_field  (0,  PSTR("Noz. Offset"), str);
      #endif
    }
  }

  bool AdjustOffsetsScreen::onTouchHeld(uint8_t tag) {
    using namespace ExtUI;
    const float inc  = getIncrement();
    #if EXTRUDERS > 1
      const bool  link = screen_data.AdjustOffsetsScreen.link_nozzles;
    #else
      constexpr bool link = true;
    #endif
    int16_t steps;
    switch(tag) {
      case  2: steps = mmToWholeSteps(inc, X); smartAdjustAxis_steps(-steps, X, link); screen_data.AdjustOffsetsScreen.rel[0] -= steps; break;
      case  3: steps = mmToWholeSteps(inc, X); smartAdjustAxis_steps( steps, X, link); screen_data.AdjustOffsetsScreen.rel[0] += steps; break;
      case  4: steps = mmToWholeSteps(inc, Y); smartAdjustAxis_steps(-steps, Y, link); screen_data.AdjustOffsetsScreen.rel[1] -= steps; break;
      case  5: steps = mmToWholeSteps(inc, Y); smartAdjustAxis_steps( steps, Y, link); screen_data.AdjustOffsetsScreen.rel[1] += steps; break;
      case  6: steps = mmToWholeSteps(inc, Z); smartAdjustAxis_steps(-steps, Z, link); screen_data.AdjustOffsetsScreen.rel[2] -= steps; break;
      case  7: steps = mmToWholeSteps(inc, Z); smartAdjustAxis_steps( steps, Z, link); screen_data.AdjustOffsetsScreen.rel[2] += steps; break;
      #if EXTRUDERS > 1
        case  8: screen_data.AdjustOffsetsScreen.link_nozzles = !link; break;
      #endif
      case  9: screen_data.AdjustOffsetsScreen.show_offsets = !screen_data.AdjustOffsetsScreen.show_offsets; break;
      default:
        return false;
    }
    return true;
  }

  bool AdjustOffsetsScreen::onTouchEnd(uint8_t tag) {
    if(tag == 1) {
      SaveSettingsScreen::promptToSaveSettings();
      return true;
    } else {
      return ValueAdjusters::onTouchEnd(tag);
    }
  }

  void AdjustOffsetsScreen::onIdle() {
    reset_menu_timeout();
  }
#endif // ENABLED(BABYSTEPPING)

/************************* BACKLASH COMPENSATION SCREEN **********************/

#if ENABLED(BACKLASH_GCODE)
  void BacklashCompensationScreen::onRedraw(draw_mode_t what) {
    using namespace ExtUI;
    widgets_t w(what);
    w.precision(2).units(PSTR("mm"));
    w.heading(                          PSTR("Axis Backlash"));
    w.color(Theme::x_axis).adjuster(2,  PSTR("X:"), ExtUI::getAxisBacklash_mm(X));
    w.color(Theme::y_axis).adjuster(4,  PSTR("Y:"), ExtUI::getAxisBacklash_mm(Y));
    w.color(Theme::z_axis).adjuster(6,  PSTR("Z:"), ExtUI::getAxisBacklash_mm(Z));
    #if ENABLED(LULZBOT_CALIBRATION_GCODE)
    w.button(12, PSTR("Measure automatically"));
    #endif
    w.color(Theme::other ).adjuster(8,  PSTR("Smoothing:"),  ExtUI::getBacklashSmoothing_mm());
    w.precision(0).units(PSTR("%"))
                          .adjuster(10, PSTR("Correction:"), ExtUI::getBacklashCorrection_percent());
    w.precision(2).increments();
  }

  bool BacklashCompensationScreen::onTouchHeld(uint8_t tag) {
    using namespace ExtUI;
    const float increment = getIncrement();
    switch(tag) {
      case  2:  UI_DECREMENT(AxisBacklash_mm, X); break;
      case  3:  UI_INCREMENT(AxisBacklash_mm, X); break;
      case  4:  UI_DECREMENT(AxisBacklash_mm, Y); break;
      case  5:  UI_INCREMENT(AxisBacklash_mm, Y); break;
      case  6:  UI_DECREMENT(AxisBacklash_mm, Z); break;
      case  7:  UI_INCREMENT(AxisBacklash_mm, Z); break;
      case  8:  UI_DECREMENT(BacklashSmoothing_mm); break;
      case  9:  UI_INCREMENT(BacklashSmoothing_mm); break;
      case  10: UI_DECREMENT_BY(BacklashCorrection_percent, increment*100);  break;
      case  11: UI_INCREMENT_BY(BacklashCorrection_percent, increment*100);  break;
      #if ENABLED(LULZBOT_CALIBRATION_GCODE)
      case  12:  GOTO_SCREEN(ConfirmAutoCalibration); return true;
      #endif
      default:
        return false;
    }
    return true;
  }
#endif // ENABLED(BACKLASH_GCODE)

/**************************** MAX FEEDRATE SCREEN ****************************/

void MaxFeedrateScreen::onRedraw(draw_mode_t what) {
  widgets_t w(what);
  w.precision(0).units(PSTR("%"));

  w.heading(PSTR("Print Speed"));
  w.adjuster(4,  PSTR("Speed"), ExtUI::getFeedrate_percent());
  w.increments();
}

bool MaxFeedrateScreen::onTouchHeld(uint8_t tag) {
  const float increment = getIncrement();
  switch(tag) {
    case 4: UI_DECREMENT(Feedrate_percent); break;
    case 5: UI_INCREMENT(Feedrate_percent); break;
    default:
      return false;
  }
  return true;
}

/***************************** MAX VELOCITY SCREEN ****************************/

void MaxVelocityScreen::onRedraw(draw_mode_t what) {
  using namespace ExtUI;
  widgets_t w(what);
  w.precision(0);
  w.units(PSTR("mm/s"));
  w.heading(                                PSTR("Maximum Velocity"));
  w.color(Theme::x_axis)     .adjuster(  2, PSTR("X:"),  ExtUI::getAxisMaxFeedrate_mm_s(X) );
  w.color(Theme::y_axis)     .adjuster(  4, PSTR("Y:"),  ExtUI::getAxisMaxFeedrate_mm_s(Y) );
  w.color(Theme::z_axis)     .adjuster(  6, PSTR("Z:"),  ExtUI::getAxisMaxFeedrate_mm_s(Z) );
  #if EXTRUDERS == 1 || DISABLED(DISTINCT_E_FACTORS)
    w.color(Theme::e_axis)   .adjuster(  8, PSTR("E:"),  ExtUI::getAxisMaxFeedrate_mm_s(E0) );
  #elif EXTRUDERS > 1
    w.color(Theme::e_axis)   .adjuster(  8, PSTR("E1:"), ExtUI::getAxisMaxFeedrate_mm_s(E0) );
    w.color(Theme::e_axis)   .adjuster( 10, PSTR("E2:"), ExtUI::getAxisMaxFeedrate_mm_s(E1) );
    #if EXTRUDERS > 2
      w.color(Theme::e_axis) .adjuster( 12, PSTR("E3:"), ExtUI::getAxisMaxFeedrate_mm_s(E2) );
    #endif
    #if EXTRUDERS > 3
      w.color(Theme::e_axis) .adjuster( 14, PSTR("E4:"), ExtUI::getAxisMaxFeedrate_mm_s(E3) );
    #endif
  #endif
  w.increments();
}

bool MaxVelocityScreen::onTouchHeld(uint8_t tag) {
  using namespace ExtUI;
  const float increment = getIncrement();
  switch(tag) {
    case  2: UI_DECREMENT(AxisMaxFeedrate_mm_s, X); break;
    case  3: UI_INCREMENT(AxisMaxFeedrate_mm_s, X); break;
    case  4: UI_DECREMENT(AxisMaxFeedrate_mm_s, Y); break;
    case  5: UI_INCREMENT(AxisMaxFeedrate_mm_s, Y); break;
    case  6: UI_DECREMENT(AxisMaxFeedrate_mm_s, Z); break;
    case  7: UI_INCREMENT(AxisMaxFeedrate_mm_s, Z); break;
    case  8: UI_DECREMENT(AxisMaxFeedrate_mm_s, E0); break;
    case  9: UI_INCREMENT(AxisMaxFeedrate_mm_s, E0); break;
    #if EXTRUDERS > 1 && ENABLED(DISTINCT_E_FACTORS)
    case 10: UI_DECREMENT(AxisMaxFeedrate_mm_s, E1); break;
    case 11: UI_INCREMENT(AxisMaxFeedrate_mm_s, E1); break;
    #endif
    #if EXTRUDERS > 2 && ENABLED(DISTINCT_E_FACTORS)
    case 12: UI_DECREMENT(AxisMaxFeedrate_mm_s, E2); break;
    case 13: UI_INCREMENT(AxisMaxFeedrate_mm_s, E2); break;
    #endif
    #if EXTRUDERS > 3 && ENABLED(DISTINCT_E_FACTORS)
    case 14: UI_DECREMENT(AxisMaxFeedrate_mm_s, E3); break;
    case 15: UI_INCREMENT(AxisMaxFeedrate_mm_s, E3); break;
    #endif
    default:
      return false;
  }
  return true;
}

/***************************** MAX ACCELERATION SCREEN ****************************/

void MaxAccelerationScreen::onRedraw(draw_mode_t what) {
  using namespace ExtUI;
  widgets_t w(what);
  w.precision(0);
  w.units(PSTR("mm/s^2"));
  w.heading(                             PSTR("Maximum Acceleration"));
  w.color(Theme::x_axis)  .adjuster(  2, PSTR("X:"),  ExtUI::getAxisMaxAcceleration_mm_s2(X) );
  w.color(Theme::y_axis)  .adjuster(  4, PSTR("Y:"),  ExtUI::getAxisMaxAcceleration_mm_s2(Y) );
  w.color(Theme::z_axis)  .adjuster(  6, PSTR("Z:"),  ExtUI::getAxisMaxAcceleration_mm_s2(Z) );
  #if EXTRUDERS == 1 || DISABLED(DISTINCT_E_FACTORS)
    w.color(Theme::e_axis) .adjuster( 8, PSTR("E:"),  ExtUI::getAxisMaxAcceleration_mm_s2(E0) );
  #elif EXTRUDERS > 1
    w.color(Theme::e_axis) .adjuster( 8, PSTR("E1:"), ExtUI::getAxisMaxAcceleration_mm_s2(E0) );
    w.color(Theme::e_axis) .adjuster(10, PSTR("E2:"), ExtUI::getAxisMaxAcceleration_mm_s2(E1) );
    #if EXTRUDERS > 2
    w.color(Theme::e_axis) .adjuster(12, PSTR("E3:"), ExtUI::getAxisMaxAcceleration_mm_s2(E2) );
    #endif
    #if EXTRUDERS > 3
    w.color(Theme::e_axis) .adjuster(14, PSTR("E4:"), ExtUI::getAxisMaxAcceleration_mm_s2(E3) );
    #endif
  #endif
  w.increments();
}

bool MaxAccelerationScreen::onTouchHeld(uint8_t tag) {
  using namespace ExtUI;
  const float increment = getIncrement();
  switch(tag) {
    case  2: UI_DECREMENT(AxisMaxAcceleration_mm_s2, X ); break;
    case  3: UI_INCREMENT(AxisMaxAcceleration_mm_s2, X );  break;
    case  4: UI_DECREMENT(AxisMaxAcceleration_mm_s2, Y ); break;
    case  5: UI_INCREMENT(AxisMaxAcceleration_mm_s2, Y ); break;
    case  6: UI_DECREMENT(AxisMaxAcceleration_mm_s2, Z ); break;
    case  7: UI_INCREMENT(AxisMaxAcceleration_mm_s2, Z ); break;
    case  8: UI_DECREMENT(AxisMaxAcceleration_mm_s2, E0); break;
    case  9: UI_INCREMENT(AxisMaxAcceleration_mm_s2, E0); break;
    #if EXTRUDERS > 1 && ENABLED(DISTINCT_E_FACTORS)
    case 10: UI_DECREMENT(AxisMaxAcceleration_mm_s2, E1); break;
    case 11: UI_INCREMENT(AxisMaxAcceleration_mm_s2, E1); break;
    #endif
    #if EXTRUDERS > 2 && ENABLED(DISTINCT_E_FACTORS)
    case 12: UI_DECREMENT(AxisMaxAcceleration_mm_s2, E2); break;
    case 13: UI_INCREMENT(AxisMaxAcceleration_mm_s2, E2); break;
    #endif
    #if EXTRUDERS > 3 && ENABLED(DISTINCT_E_FACTORS)
    case 14: UI_DECREMENT(AxisMaxAcceleration_mm_s2, E3); break;
    case 15: UI_INCREMENT(AxisMaxAcceleration_mm_s2, E3); break;
    #endif
    default:
      return false;
  }
  return true;
}

/***************************** DEFAULT ACCELERATION SCREEN ****************************/

void DefaultAccelerationScreen::onRedraw(draw_mode_t what) {
  widgets_t w(what);
  w.precision(0);
  w.units(PSTR("mm/s^2"));
  w.heading(                             PSTR("Default Acceleration"));
  w.color(Theme::other);
  w.adjuster(  2, PSTR("Printing:"),   ExtUI::getPrintingAcceleration_mm_s2() );
  w.adjuster(  4, PSTR("Travel:"),     ExtUI::getTravelAcceleration_mm_s2() );
  w.adjuster(  6, PSTR("Retraction:"), ExtUI::getRetractAcceleration_mm_s2() );
  w.increments();
  w.button(    8, PSTR("Set Axis Maximum"));
}

bool DefaultAccelerationScreen::onTouchHeld(uint8_t tag) {
  const float increment = getIncrement();
  switch(tag) {
    case  2: UI_DECREMENT(PrintingAcceleration_mm_s2); break;
    case  3: UI_INCREMENT(PrintingAcceleration_mm_s2); break;
    case  4: UI_DECREMENT(TravelAcceleration_mm_s2);   break;
    case  5: UI_INCREMENT(TravelAcceleration_mm_s2);   break;
    case  6: UI_DECREMENT(RetractAcceleration_mm_s2);  break;
    case  7: UI_INCREMENT(RetractAcceleration_mm_s2);  break;
    case  8: GOTO_SCREEN(MaxAccelerationScreen);       break;
    default:
      return false;
  }
  return true;
}

/***************************** JUNCTION DEVIATION SCREEN *****************************/

#if ENABLED(JUNCTION_DEVIATION)
  void JunctionDeviationScreen::onRedraw(draw_mode_t what) {
    widgets_t w(what);
    w.precision(2);
    w.units(PSTR("mm"));
    w.heading(                          PSTR("Junction Deviation"));
    w.color(Theme::other) .adjuster( 2, PSTR(""), ExtUI::getJunctionDeviation_mm() );
    w.increments();
  }

  bool JunctionDeviationScreen::onTouchHeld(uint8_t tag) {
    const float increment = getIncrement();
    switch(tag) {
      case  2: UI_DECREMENT(JunctionDeviation_mm); break;
      case  3: UI_INCREMENT(JunctionDeviation_mm); break;
      default:
        return false;
    }
    onRefresh();
    return true;
  }
#endif

/************************************** JERK SCREEN ************************************/

#if DISABLED(JUNCTION_DEVIATION)
  void JerkScreen::onRedraw(draw_mode_t what) {
    using namespace ExtUI;
    widgets_t w(what);
    w.precision(1);
    w.units(PSTR("mm/s"));
    w.heading(                           PSTR("Maximum Jerk"));
    w.color(Theme::x_axis) .adjuster( 2, PSTR("X:"), ExtUI::getAxisMaxJerk_mm_s(X) );
    w.color(Theme::y_axis) .adjuster( 4, PSTR("Y:"), ExtUI::getAxisMaxJerk_mm_s(Y) );
    w.color(Theme::z_axis) .adjuster( 6, PSTR("Z:"), ExtUI::getAxisMaxJerk_mm_s(Z) );
    w.color(Theme::e_axis) .adjuster( 8, PSTR("E:"), ExtUI::getAxisMaxJerk_mm_s(E0) );
    w.increments();
  }

  bool JerkScreen::onTouchHeld(uint8_t tag) {
    using namespace ExtUI;
    const float increment = getIncrement();
    switch(tag) {
      case  2: UI_DECREMENT(AxisMaxJerk_mm_s, X); break;
      case  3: UI_INCREMENT(AxisMaxJerk_mm_s, X); break;
      case  4: UI_DECREMENT(AxisMaxJerk_mm_s, Y); break;
      case  5: UI_INCREMENT(AxisMaxJerk_mm_s, Y); break;
      case  6: UI_DECREMENT(AxisMaxJerk_mm_s, Z); break;
      case  7: UI_INCREMENT(AxisMaxJerk_mm_s, Z); break;
      case  8: UI_DECREMENT(AxisMaxJerk_mm_s, E0); break;
      case  9: UI_INCREMENT(AxisMaxJerk_mm_s, E0); break;
      default:
        return false;
    }
    return true;
  }
#endif

/*************************** FILAMENT OPTIONS SCREEN *****************************/

#if ENABLED(LIN_ADVANCE) || ENABLED(FILAMENT_RUNOUT_SENSOR)
  void FilamentOptionsScreen::onRedraw(draw_mode_t what) {
    using namespace ExtUI;
    widgets_t w(what);
    w.precision(1).color(Theme::e_axis);
    #if ENABLED(LIN_ADVANCE)
      w.heading(             PSTR("Linear Advance:"));
      #if EXTRUDERS == 1
        w.adjuster(       2, PSTR("K:"),    ExtUI::getLinearAdvance_mm_mm_s(E0) );
      #else
        w.adjuster(       2, PSTR("K E1:"), ExtUI::getLinearAdvance_mm_mm_s(E0) );
        w.adjuster(       4, PSTR("K E2:"), ExtUI::getLinearAdvance_mm_mm_s(E1) );
        #if EXTRUDERS > 2
          w.adjuster(     6, PSTR("K E3:"), ExtUI::getLinearAdvance_mm_mm_s(E2) );
          #if EXTRUDERS > 3
            w.adjuster(   8, PSTR("K E4:"), ExtUI::getLinearAdvance_mm_mm_s(E3) );
          #endif
        #endif
      #endif
    #endif

    #if ENABLED(FILAMENT_RUNOUT_SENSOR)
      w.heading( PSTR("Runout Detection:"));
      #if defined(FILAMENT_RUNOUT_DISTANCE_MM)
        w.units(PSTR("mm"));
        if(ExtUI::getFilamentRunoutEnabled()) {
          w.adjuster( 10, PSTR("Distance:"), ExtUI::getFilamentRunoutDistance_mm() );
        } else {
          w.adjuster( 10, PSTR("Distance:"), PSTR("disabled") );
        }
      #else
        w.adjuster(   10, PSTR("Status:"), ExtUI::getFilamentRunoutEnabled() ? PSTR("enabled") : PSTR("disabled") );
      #endif
    #endif
      w.heading(PSTR(""));
      w.increments();
  }

  bool FilamentOptionsScreen::onTouchHeld(uint8_t tag) {
    using namespace ExtUI;
    const float increment = getIncrement();
    switch(tag) {
    #if ENABLED(LIN_ADVANCE)
        case  2: UI_DECREMENT(LinearAdvance_mm_mm_s, E0); break;
        case  3: UI_INCREMENT(LinearAdvance_mm_mm_s, E0); break;
        #if EXTRUDERS > 1
          case  4: UI_DECREMENT(LinearAdvance_mm_mm_s, E1);  break;
          case  5: UI_INCREMENT(LinearAdvance_mm_mm_s, E1); break;
          #if EXTRUDERS > 2
            case  6: UI_DECREMENT(LinearAdvance_mm_mm_s, E2);  break;
            case  7: UI_INCREMENT(LinearAdvance_mm_mm_s, E2);  break;
            #if EXTRUDERS > 3
              case  8: UI_DECREMENT(LinearAdvance_mm_mm_s, E3);  break;
              case  9: UI_INCREMENT(LinearAdvance_mm_mm_s, E3);  break;
            #endif
          #endif
        #endif
      #endif
      #if ENABLED(FILAMENT_RUNOUT_SENSOR)
        #if defined(FILAMENT_RUNOUT_DISTANCE_MM)
          case  10: UI_DECREMENT(FilamentRunoutDistance_mm); break;
          case  11: UI_INCREMENT(FilamentRunoutDistance_mm); break;
        #else
          case  10: ExtUI::setFilamentRunoutEnabled(false); break;
          case  11: ExtUI::setFilamentRunoutEnabled(true);  break;
        #endif
      #endif
      default:
        return false;
    }

    #if ENABLED(FILAMENT_RUNOUT_SENSOR) && defined(FILAMENT_RUNOUT_DISTANCE_MM)
      setFilamentRunoutEnabled(getFilamentRunoutDistance_mm() >= FILAMENT_RUNOUT_DISTANCE_MM);
    #endif

    onRefresh();
    return true;
  }

  bool FilamentOptionsScreen::onTouchEnd(uint8_t tag) {
    if(tag == 1 && !IS_PARENT_SCREEN(AdvancedSettingsScreen)) {
      /* The AdvancedSettingsScreen will prompt the user to
         save settings; any other parent, do it here */
      SaveSettingsScreen::promptToSaveSettings();
      return true;
    } else {
      return ValueAdjusters::onTouchEnd(tag);
    }
  }
#endif

/*********************** INTERFACE SETTINGS SCREEN ********************/

void InterfaceSettingsScreen::onStartup() {
  loadSettings();
}

void InterfaceSettingsScreen::onEntry() {
  screen_data.InterfaceSettingsScreen.brightness = CLCD::get_brightness();
  screen_data.InterfaceSettingsScreen.volume     = FTDI::SoundPlayer::get_volume();
  BaseScreen::onEntry();
}

void InterfaceSettingsScreen::onRedraw(draw_mode_t what) {
  using namespace ExtUI;
  CommandProcessor cmd;

  if(what & BACKGROUND) {
    cmd.cmd(CLEAR_COLOR_RGB(Theme::background))
       .cmd(CLEAR(true,true,true))

    #define GRID_COLS 4
    #if defined(USE_PORTRAIT_ORIENTATION)
      #define GRID_ROWS 7
    #else
      #define GRID_ROWS 6
    #endif

       .font(Theme::font_medium)
       .text      (BTN_POS(1,1), BTN_SIZE(4,1), F("Interface Settings"))
    #undef EDGE_R
    #define EDGE_R 30
       .font(Theme::font_small)
       .tag(0).text      (BTN_POS(1,2), BTN_SIZE(2,1), F("Screen brightness:"), OPT_RIGHTX | OPT_CENTERY)
              .text      (BTN_POS(1,3), BTN_SIZE(2,1), F("Sound volume:"),      OPT_RIGHTX | OPT_CENTERY)
              .text      (BTN_POS(1,4), BTN_SIZE(2,1), F("Screen lock:"),       OPT_RIGHTX | OPT_CENTERY);
      #ifdef SPI_FLASH_SS
        cmd   .text      (BTN_POS(1,5), BTN_SIZE(2,1), F("Boot animation:"),    OPT_RIGHTX | OPT_CENTERY);
      #endif
    #undef EDGE_R
    #define EDGE_R 0
  }

  if(what & FOREGROUND) {
    #if defined(USE_PORTRAIT_ORIENTATION)
      constexpr uint8_t w = 2;
    #else
      constexpr uint8_t w = 1;
    #endif

    cmd.font(Theme::font_medium)

    #undef EDGE_R
    #define EDGE_R 30
       .tag(2).slider    (BTN_POS(3,2), BTN_SIZE(2,1), screen_data.InterfaceSettingsScreen.brightness, 128)
       .tag(3).slider    (BTN_POS(3,3), BTN_SIZE(2,1), screen_data.InterfaceSettingsScreen.volume,     0xFF)
       .tag(4).toggle    (BTN_POS(3,4), BTN_SIZE(w,1), F("off\xFFon"), LockScreen::is_enabled())
       #ifdef SPI_FLASH_SS
       .tag(5).toggle    (BTN_POS(3,5), BTN_SIZE(w,1), F("off\xFFon"), UIData::animations_enabled())
       #endif
    #undef EDGE_R
    #define EDGE_R 0
    #if defined(USE_PORTRAIT_ORIENTATION)
      .tag(6).button    (BTN_POS(1,6), BTN_SIZE(4,1), F("Customize Sounds"))
      .style(STYLE_LIGHT_BTN)
      .tag(1).button    (BTN_POS(1,7), BTN_SIZE(4,1), F("Back"));
    #else
      .tag(6).button    (BTN_POS(1,6), BTN_SIZE(2,1), F("Customize Sounds"))
      .style(STYLE_LIGHT_BTN)
      .tag(1).button    (BTN_POS(3,6), BTN_SIZE(2,1), F("Back"));
    #endif
  }
}

bool InterfaceSettingsScreen::onTouchEnd(uint8_t tag) {
  switch(tag) {
    case 1: GOTO_PREVIOUS();                               break;
    case 4:
      if(!LockScreen::is_enabled()) {
        LockScreen::enable();
      } else {
        LockScreen::disable();
      }
      break;
    case 5: UIData::enable_animations(!UIData::animations_enabled());; break;
    case 6: GOTO_SCREEN(InterfaceSoundsScreen); break;
    default:
      return false;
  }
  return true;
}

bool InterfaceSettingsScreen::onTouchStart(uint8_t tag) {
  #undef EDGE_R
  #define EDGE_R 30
  CommandProcessor cmd;
  switch(tag) {
    case 2: cmd.track_linear(BTN_POS(3,3), BTN_SIZE(2,1), 2).execute(); break;
    case 3: cmd.track_linear(BTN_POS(3,4), BTN_SIZE(2,1), 3).execute(); break;
    default: break;
  }
  #undef EDGE_R
  #define EDGE_R 0
  #undef GRID_COLS
  #undef GRID_ROWS
  return true;
}

void InterfaceSettingsScreen::onIdle() {
  if(refresh_timer.elapsed(TOUCH_UPDATE_INTERVAL)) {
    refresh_timer.start();

    uint16_t value;
    CommandProcessor cmd;
    switch(cmd.track_tag(value)) {
      case 2:
        screen_data.InterfaceSettingsScreen.brightness = float(value) * 128 / 0xFFFF;
        CLCD::set_brightness(screen_data.InterfaceSettingsScreen.brightness);
        break;
      case 3:
        screen_data.InterfaceSettingsScreen.volume = value >> 8;
        FTDI::SoundPlayer::set_volume(screen_data.InterfaceSettingsScreen.volume);
        break;
      default:
        return;
    }
    onRefresh();
  }
  BaseScreen::onIdle();
}

void InterfaceSettingsScreen::defaultSettings() {
  using namespace ExtUI;

  LockScreen::passcode = 0;
  FTDI::SoundPlayer::set_volume(255);
  CLCD::set_brightness(255);
  UIData::reset_value();
  InterfaceSoundsScreen::defaultSettings();
  // TODO: This really should be moved to the EEPROM
  #if ENABLED(BACKLASH_GCODE)
    constexpr float backlash[XYZ] = BACKLASH_DISTANCE_MM;
    setAxisBacklash_mm(backlash[X_AXIS], X);
    setAxisBacklash_mm(backlash[Y_AXIS], Y);
    setAxisBacklash_mm(backlash[Z_AXIS], Z);
    setBacklashCorrection_percent(BACKLASH_CORRECTION);
    #ifdef BACKLASH_SMOOTHING_MM
      setBacklashSmoothing_mm(BACKLASH_SMOOTHING_MM);
    #endif
  #endif
  #if ENABLED(FILAMENT_RUNOUT_SENSOR) && defined(FILAMENT_RUNOUT_DISTANCE_MM)
    setFilamentRunoutDistance_mm(FILAMENT_RUNOUT_DISTANCE_MM);
    setFilamentRunoutEnabled(true);
  #endif
}

void InterfaceSettingsScreen::saveSettings() {
  using namespace ExtUI;

  persistent_data_t      data;
  data.magic_word        = data.MAGIC_WORD;
  data.version           = 0;
  data.sound_volume      = FTDI::SoundPlayer::get_volume();
  data.screen_brightness = CLCD::get_brightness();
  data.passcode          = LockScreen::passcode;
  data.bit_flags         = UIData::get_value();
  data.touch_transform_a = CLCD::mem_read_32(REG_TOUCH_TRANSFORM_A);
  data.touch_transform_b = CLCD::mem_read_32(REG_TOUCH_TRANSFORM_B);
  data.touch_transform_c = CLCD::mem_read_32(REG_TOUCH_TRANSFORM_C);
  data.touch_transform_d = CLCD::mem_read_32(REG_TOUCH_TRANSFORM_D);
  data.touch_transform_e = CLCD::mem_read_32(REG_TOUCH_TRANSFORM_E);
  data.touch_transform_f = CLCD::mem_read_32(REG_TOUCH_TRANSFORM_F);
  #if ENABLED(LULZBOT_BACKUP_EEPROM_INFORMATION)
    #if ENABLED(PRINTCOUNTER)
      // Keep a backup of the print counter information in SPI EEPROM
      // since the emulated EEPROM on the Due HAL does not survive
      // a reflash.
      printStatistics* stats   = print_job_timer.getStatsPtr();
      data.total_prints        = stats->totalPrints;
      data.finished_prints     = stats->finishedPrints;
      data.total_print_time    = stats->printTime;
      data.longest_print       = stats->longestPrint;
      data.total_filament_used = stats->filamentUsed;
    #endif
    #if EXTRUDERS > 1
      data.nozzle_offsets_mm[X_AXIS] = getNozzleOffset_mm(X, E1);
      data.nozzle_offsets_mm[Y_AXIS] = getNozzleOffset_mm(Y, E1);
      data.nozzle_offsets_mm[Z_AXIS] = getNozzleOffset_mm(Z, E1);
    #endif
    data.nozzle_z_offset           = getZOffset_mm();
  #endif
  // TODO: This really should be moved to the EEPROM
  #if ENABLED(BACKLASH_GCODE)
    data.backlash_distance_mm[X_AXIS] = getAxisBacklash_mm(X);
    data.backlash_distance_mm[Y_AXIS] = getAxisBacklash_mm(Y);
    data.backlash_distance_mm[Z_AXIS] = getAxisBacklash_mm(Z);
    data.backlash_correction          = getBacklashCorrection_percent();
    #ifdef BACKLASH_SMOOTHING_MM
      data.backlash_smoothing_mm      = getBacklashSmoothing_mm();
    #endif
  #endif
  #if ENABLED(FILAMENT_RUNOUT_SENSOR) && defined(FILAMENT_RUNOUT_DISTANCE_MM)
    data.runout_sensor_mm             = getFilamentRunoutDistance_mm();
    data.runout_sensor_enabled        = getFilamentRunoutEnabled();
  #endif
  for(uint8_t i = 0; i < InterfaceSoundsScreen::NUM_EVENTS; i++)
    data.event_sounds[i] = InterfaceSoundsScreen::event_sounds[i];
  UIFlashStorage::write_config_data(&data, sizeof(data));
}

void InterfaceSettingsScreen::loadSettings() {
  using namespace ExtUI;

  persistent_data_t data;
  UIFlashStorage::read_config_data(&data, sizeof(data));
  if(data.magic_word == data.MAGIC_WORD && data.version == 0) {
    FTDI::SoundPlayer::set_volume(data.sound_volume);
    CLCD::set_brightness(data.screen_brightness);
    LockScreen::passcode = data.passcode;
    UIData::set_value(data.bit_flags);
    CLCD::mem_write_32(REG_TOUCH_TRANSFORM_A, data.touch_transform_a);
    CLCD::mem_write_32(REG_TOUCH_TRANSFORM_B, data.touch_transform_b);
    CLCD::mem_write_32(REG_TOUCH_TRANSFORM_C, data.touch_transform_c);
    CLCD::mem_write_32(REG_TOUCH_TRANSFORM_D, data.touch_transform_d);
    CLCD::mem_write_32(REG_TOUCH_TRANSFORM_E, data.touch_transform_e);
    CLCD::mem_write_32(REG_TOUCH_TRANSFORM_F, data.touch_transform_f);
    #if ENABLED(LULZBOT_BACKUP_EEPROM_INFORMATION)
      #if ENABLED(PRINTCOUNTER)
        printStatistics* stats   = print_job_timer.getStatsPtr();
        stats->totalPrints       = max(stats->totalPrints,    data.total_prints);
        stats->finishedPrints    = max(stats->finishedPrints, data.finished_prints);
        stats->printTime         = max(stats->printTime,      data.total_print_time);
        stats->longestPrint      = max(stats->longestPrint,   data.longest_print);
        stats->filamentUsed      = max(stats->filamentUsed,   data.total_filament_used);
      #endif
      #if EXTRUDERS > 1
        setNozzleOffset_mm(data.nozzle_offsets_mm[X_AXIS], X, E1);
        setNozzleOffset_mm(data.nozzle_offsets_mm[Y_AXIS], Y, E1);
        setNozzleOffset_mm(data.nozzle_offsets_mm[Z_AXIS], Z, E1);
      #endif
      setZOffset_mm(data.nozzle_z_offset);
    #endif
    // TODO: This really should be moved to the EEPROM
    #if ENABLED(BACKLASH_GCODE)
      setAxisBacklash_mm(data.backlash_distance_mm[X_AXIS], X);
      setAxisBacklash_mm(data.backlash_distance_mm[Y_AXIS], Y);
      setAxisBacklash_mm(data.backlash_distance_mm[Z_AXIS], Z);
      setBacklashCorrection_percent(data.backlash_correction);
      #ifdef BACKLASH_SMOOTHING_MM
        setBacklashSmoothing_mm(data.backlash_smoothing_mm);
      #endif
    #endif
    #if ENABLED(FILAMENT_RUNOUT_SENSOR) && defined(FILAMENT_RUNOUT_DISTANCE_MM)
      setFilamentRunoutDistance_mm(data.runout_sensor_mm);
      setFilamentRunoutEnabled(data.runout_sensor_enabled);
    #endif
    for(uint8_t i = 0; i < InterfaceSoundsScreen::NUM_EVENTS; i++)
      InterfaceSoundsScreen::event_sounds[i] = data.event_sounds[i];
  }
}

/************************* INTERFACE SOUNDS SCREEN ********************/

class SoundChoices {
  private:
    static PROGMEM const struct sound_list_t {
      const char *const PROGMEM name;
      const FTDI::SoundPlayer::sound_t* data;
    } sound_list[];
  public:
    static const uint8_t n;
    static inline const char* name(uint8_t val) {
      return (const char* ) pgm_read_ptr_near(&sound_list[val].name);
    }
    static inline FTDI::SoundPlayer::sound_t* data(uint8_t val) {
      return (FTDI::SoundPlayer::sound_t*) pgm_read_ptr_near(&sound_list[val].data);
    }
};

const SoundChoices::sound_list_t SoundChoices::sound_list[] = {
  {"Silence",      FTDI::silence},
  {"Twinkle",      FTDI::twinkle},
  {"Chimes",       FTDI::chimes},
  {"Fanfare",      FTDI::fanfare},
  {"Sad Trombone", FTDI::sad_trombone},
  {"Big Band",     FTDI::big_band},
  {"Beeping",      FTDI::beeping},
  {"Alarm",        FTDI::alarm},
  {"Warble",       FTDI::warble},
  {"Carousel",     FTDI::carousel},
  {"Beats",        FTDI::beats},
  {"Bach Joy",     FTDI::js_bach_joy},
  {"Bach Toccata", FTDI::js_bach_toccata}
};

const uint8_t SoundChoices::n = N_ELEMENTS(SoundChoices::sound_list);

uint8_t InterfaceSoundsScreen::event_sounds[];

const char* InterfaceSoundsScreen::getSoundSelection(event_t event) {
  return SoundChoices::name(event_sounds[event]);
}

void InterfaceSoundsScreen::toggleSoundSelection(event_t event) {
  event_sounds[event] = (event_sounds[event]+1) % SoundChoices::n;
  playEventSound(event);
}

void InterfaceSoundsScreen::setSoundSelection(event_t event, const FTDI::SoundPlayer::sound_t* sound) {
  for(uint8_t i = 0; i < SoundChoices::n; i++) {
    if(SoundChoices::data(i) == sound) {
      event_sounds[event] = i;
    }
  }
}

void InterfaceSoundsScreen::playEventSound(event_t event, FTDI::play_mode_t mode) {
  sound.play(SoundChoices::data(event_sounds[event]), mode);
}

void InterfaceSoundsScreen::defaultSettings() {
  setSoundSelection(PRINTING_STARTED,  FTDI::twinkle);
  setSoundSelection(PRINTING_FINISHED, FTDI::fanfare);
  setSoundSelection(PRINTING_FAILED,   FTDI::sad_trombone);
}

void InterfaceSoundsScreen::onRedraw(draw_mode_t what) {
  using namespace ExtUI;
  CommandProcessor cmd;

  if(what & BACKGROUND) {
    cmd.cmd(CLEAR_COLOR_RGB(Theme::background))
       .cmd(CLEAR(true,true,true))

    #define GRID_COLS 4
    #define GRID_ROWS 9

       .font(Theme::font_medium)
       .text(BTN_POS(1,1), BTN_SIZE(4,1), F("Interface Sounds"))
    #undef EDGE_R
    #define EDGE_R 30
       .font(Theme::font_small)
       .tag(0).text      (BTN_POS(1,2), BTN_SIZE(2,1), F("Sound volume:"),   OPT_RIGHTX | OPT_CENTERY)
              .text      (BTN_POS(1,3), BTN_SIZE(2,1), F("Click sounds:"),   OPT_RIGHTX | OPT_CENTERY)
              .text      (BTN_POS(1,5), BTN_SIZE(2,1), F("Print starting:"), OPT_RIGHTX | OPT_CENTERY)
              .text      (BTN_POS(1,6), BTN_SIZE(2,1), F("Print finished:"), OPT_RIGHTX | OPT_CENTERY)
              .text      (BTN_POS(1,7), BTN_SIZE(2,1), F("Print error:"),    OPT_RIGHTX | OPT_CENTERY);
    #undef EDGE_R
    #define EDGE_R 0
  }

  if(what & FOREGROUND) {
    #if defined(USE_PORTRAIT_ORIENTATION)
      constexpr uint8_t w = 2;
    #else
      constexpr uint8_t w = 1;
    #endif

    cmd.font(Theme::font_medium)
    #undef EDGE_R
    #define EDGE_R 30
       .tag(2).slider    (BTN_POS(3,2), BTN_SIZE(2,1), screen_data.InterfaceSettingsScreen.volume, 0xFF)
       .tag(3).toggle    (BTN_POS(3,3), BTN_SIZE(w,1), F("off\xFFon"), UIData::touch_sounds_enabled())
    #undef EDGE_R
    #define EDGE_R 0
       .tag(4).button    (BTN_POS(3,5), BTN_SIZE(2,1), getSoundSelection(PRINTING_STARTED))
       .tag(5).button    (BTN_POS(3,6), BTN_SIZE(2,1), getSoundSelection(PRINTING_FINISHED))
       .tag(6).button    (BTN_POS(3,7), BTN_SIZE(2,1), getSoundSelection(PRINTING_FAILED))
       .style(STYLE_LIGHT_BTN)
       .tag(1).button    (BTN_POS(1,9), BTN_SIZE(4,1), F("Back"));
  }
}

bool InterfaceSoundsScreen::onTouchEnd(uint8_t tag) {
  switch(tag) {
    case 1: GOTO_PREVIOUS();                                              break;
    case 3: UIData::enable_touch_sounds(!UIData::touch_sounds_enabled()); break;
    case 4: toggleSoundSelection(PRINTING_STARTED);                       break;
    case 5: toggleSoundSelection(PRINTING_FINISHED);                      break;
    case 6: toggleSoundSelection(PRINTING_FAILED);                        break;
    default:
      return false;
  }
  return true;
}

bool InterfaceSoundsScreen::onTouchStart(uint8_t tag) {
  CommandProcessor cmd;
  #undef EDGE_R
  #define EDGE_R 30
  switch(tag) {
    case 2: cmd.track_linear(BTN_POS(3,2), BTN_SIZE(2,1), 2).execute(); break;
    default: break;
  }
  #undef EDGE_R
  #define EDGE_R 0
  #undef GRID_COLS
  #undef GRID_ROWS
  return true;
}

void InterfaceSoundsScreen::onIdle() {
  if(refresh_timer.elapsed(TOUCH_UPDATE_INTERVAL)) {
    refresh_timer.start();

    uint16_t value;
    CommandProcessor cmd;
    switch(cmd.track_tag(value)) {
      case 2:
        screen_data.InterfaceSettingsScreen.volume = value >> 8;
        FTDI::SoundPlayer::set_volume(screen_data.InterfaceSettingsScreen.volume);
        break;
      default:
        return;
    }
    onRefresh();
  }
  BaseScreen::onIdle();
}

/****************************** LOCK SCREEN ***************************/

uint16_t LockScreen::passcode = 0;

void LockScreen::onEntry() {
  const uint8_t siz = sizeof(screen_data.LockScreen.passcode);
  memset(screen_data.LockScreen.passcode, '_', siz-1);
  screen_data.LockScreen.passcode[siz-1] = '\0';
  BaseScreen::onEntry();
}

void LockScreen::onRedraw(draw_mode_t what) {
  CommandProcessor cmd;

  if(what & BACKGROUND) {
    cmd.cmd(CLEAR_COLOR_RGB(Theme::background))
       .cmd(CLEAR(true,true,true));
  }

  if(what & FOREGROUND) {
    #if defined(USE_PORTRAIT_ORIENTATION)
      #define GRID_COLS 1
      #define GRID_ROWS 10
    #else
      #define GRID_COLS 1
      #define GRID_ROWS 7
    #endif

    #undef MARGIN_T
    #undef MARGIN_B
    #define MARGIN_T 3
    #define MARGIN_B 3

    default_button_colors();

    progmem_str message;
    switch(message_style()) {
      case 'w':
        message = F("Wrong passcode!");
        break;
      case 'g':
        message = F("Passcode accepted!");
        break;
      default:
        if(passcode == 0) {
          message = F("Select Passcode:");
        } else {
          message = F("Enter Passcode:");
        }
    }
    message_style() = '\0'; // Terminate the string.

    #if defined(USE_PORTRAIT_ORIENTATION)
      constexpr uint8_t l = 6;
    #else
      constexpr uint8_t l = 3;
    #endif

    cmd.font(Theme::font_large)
       #if defined(USE_PORTRAIT_ORIENTATION)
       .text(BTN_POS(1,2), BTN_SIZE(1,1), message)
       .font(Theme::font_xlarge)
       .text(BTN_POS(1,4), BTN_SIZE(1,1), screen_data.LockScreen.passcode)
       #else
       .text(BTN_POS(1,1), BTN_SIZE(1,1), message)
       .font(Theme::font_xlarge)
       .text(BTN_POS(1,2), BTN_SIZE(1,1), screen_data.LockScreen.passcode)
       .font(Theme::font_large)
       #endif
       #if defined(USE_NUMERIC_PASSCODE)
       .keys(BTN_POS(1,l+1), BTN_SIZE(1,1), F("123"),        get_pressed_tag())
       .keys(BTN_POS(1,l+2), BTN_SIZE(1,1), F("456"),        get_pressed_tag())
       .keys(BTN_POS(1,l+3), BTN_SIZE(1,1), F("789"),        get_pressed_tag())
       .keys(BTN_POS(1,l+4), BTN_SIZE(1,1), F("0.<"),        get_pressed_tag());
       #else
       .keys(BTN_POS(1,l+1), BTN_SIZE(1,1), F("1234567890"), get_pressed_tag())
       .keys(BTN_POS(1,l+2), BTN_SIZE(1,1), F("qwertyuiop"), get_pressed_tag())
       .keys(BTN_POS(1,l+3), BTN_SIZE(1,1), F("asdfghijkl"), get_pressed_tag())
       .keys(BTN_POS(1,l+4), BTN_SIZE(1,1), F("zxcvbnm!?<"), get_pressed_tag());
       #endif

    #undef MARGIN_T
    #undef MARGIN_B
    #define MARGIN_T MARGIN_DEFAULT
    #define MARGIN_B MARGIN_DEFAULT

    #undef GRID_COLS
    #undef GRID_ROWS
  }
}

char &LockScreen::message_style() {
  // We use the last byte of the passcode string as a flag to indicate,
  // which message to show.
  constexpr uint8_t last_char = sizeof(screen_data.LockScreen.passcode)-1;
  return screen_data.LockScreen.passcode[last_char];
}

void LockScreen::onPasscodeEntered() {
  if(passcode == 0) {
    // We are defining a passcode
    message_style() = 0;
    onRefresh();
    sound.play(twinkle, PLAY_SYNCHRONOUS);
    passcode = compute_checksum();
    GOTO_PREVIOUS();
  } else {
    // We are verifying a passcode
    if(passcode == compute_checksum()) {
      message_style() = 'g';
      onRefresh();
      sound.play(twinkle, PLAY_SYNCHRONOUS);
      GOTO_PREVIOUS();
    } else {
      message_style() = 'w';
      onRefresh();
      sound.play(sad_trombone, PLAY_SYNCHRONOUS);
      current_screen.forget(); // Discard the screen the user was trying to go to.
      GOTO_PREVIOUS();
    }
  }
}

bool LockScreen::onTouchEnd(uint8_t tag) {
  char *c = strchr(screen_data.LockScreen.passcode,'_');
  if(c) {
    if(tag == '<') {
      if(c != screen_data.LockScreen.passcode) {
        // Backspace deletes previous entered characters.
        *--c = '_';
      }
    } else {
      // Append character to passcode
      *c++ = tag;
      if(*c == '\0') {
        // If at last character, then process the code.
        onPasscodeEntered();
      }
    }
  }
  return true;
}

uint16_t LockScreen::compute_checksum() {
  uint16_t checksum = 0;
  const char* c = screen_data.LockScreen.passcode;
  while(*c) {
    checksum = (checksum << 2) ^ *c++;
  }
  if(checksum == 0) checksum = 0xFFFF; // Prevent a zero checksum
  return checksum;
}

// This function should be called *after* calling GOTO_SCREEN
// to move to new screen. If a passcode is enabled, it will
// immediately jump to the keypad screen, pushing the previous
// screen onto the stack. If the code is entered correctly,
// the stack will be popped, allowing the user to proceed to
// the new screen. Otherwise it will be popped twice, taking
// the user back to where they were before.
void LockScreen::check_passcode() {
  if(passcode == 0) return;
  message_style() = 0;
  GOTO_SCREEN(LockScreen);
}

bool LockScreen::is_enabled() {
  return passcode != 0;
}

void LockScreen::disable() {
  passcode = 0;
}

void LockScreen::enable() {
  message_style() = 0;
  passcode = 0;
  GOTO_SCREEN(LockScreen);
}

/***************************** FILES SCREEN ***************************/

void FilesScreen::onEntry() {
  screen_data.FilesScreen.cur_page        = 0;
  screen_data.FilesScreen.selected_tag    = 0xFF;
  gotoPage(0);

  BaseScreen::onEntry();
}

const char *FilesScreen::getSelectedShortFilename() {
  ExtUI::FileList files;
  files.seek(getFileForTag(screen_data.FilesScreen.selected_tag), true);
  return files.shortFilename();
}

void FilesScreen::drawSelectedFile() {
  ExtUI::FileList files;
  files.seek(getFileForTag(screen_data.FilesScreen.selected_tag), true);
  screen_data.FilesScreen.flags.is_dir = files.isDir();
  drawFileButton(files.filename(), screen_data.FilesScreen.selected_tag, screen_data.FilesScreen.flags.is_dir, true);
  default_button_colors();
}

uint16_t FilesScreen::getFileForTag(uint8_t tag) {
  return screen_data.FilesScreen.cur_page * files_per_page + tag - 2;
}

#if defined(USE_PORTRAIT_ORIENTATION)
  #define GRID_COLS  6
  #define GRID_ROWS (files_per_page + header_h + footer_h)
#else
  #define GRID_COLS  6
  #define GRID_ROWS (files_per_page + header_h + footer_h)
#endif

void FilesScreen::drawFileButton(const char* filename, uint8_t tag, bool is_dir, bool is_highlighted) {
  const uint8_t line = getLineForTag(tag)+1;
  CommandProcessor cmd;
  cmd.tag(tag);
  cmd.fgcolor(is_highlighted ? Theme::files_selected : Theme::background);
  cmd.font(Theme::font_medium)
     .button( BTN_POS(1,header_h+line), BTN_SIZE(6,1), F(""),    OPT_FLAT)
     .text  ( BTN_POS(1,header_h+line), BTN_SIZE(6,1), filename, OPT_CENTERY);
  if(is_dir) {
    cmd.text( BTN_POS(1,header_h+line), BTN_SIZE(6,1), F("> "),  OPT_CENTERY | OPT_RIGHTX);
  }
}

void FilesScreen::drawFileList() {
  ExtUI::FileList files;
  screen_data.FilesScreen.num_page = max(1,(ceil)(float(files.count()) / files_per_page));
  screen_data.FilesScreen.cur_page = min(screen_data.FilesScreen.cur_page, screen_data.FilesScreen.num_page-1);
  screen_data.FilesScreen.flags.is_root  = files.isAtRootDir();

  #undef MARGIN_T
  #undef MARGIN_B
  #define MARGIN_T 0
  #define MARGIN_B 0
  uint16_t fileIndex = screen_data.FilesScreen.cur_page * files_per_page;
  for(uint8_t i = 0; i < files_per_page; i++, fileIndex++) {
    if(files.seek(fileIndex)) {
      drawFileButton(files.filename(), getTagForLine(i), files.isDir(), false);
    } else {
      break;
    }
  }
}

void FilesScreen::drawHeader() {
  const bool prev_enabled = screen_data.FilesScreen.cur_page > 0;
  const bool next_enabled = screen_data.FilesScreen.cur_page < (screen_data.FilesScreen.num_page - 1);

  #undef MARGIN_T
  #undef MARGIN_B
  #define MARGIN_T 0
  #define MARGIN_B 2

  char str[16];
  sprintf_P(str, PSTR("Page %d of %d"),
    screen_data.FilesScreen.cur_page + 1, screen_data.FilesScreen.num_page);

  CommandProcessor cmd;
  cmd.font(Theme::font_small)
     .tag(0).button( BTN_POS(2,1), BTN_SIZE(4,header_h), str, OPT_CENTER | OPT_FLAT);

  cmd.font(Theme::font_medium)
     .style(STYLE_LIGHT_BTN)
     .tag(241).enabled(prev_enabled).button( BTN_POS(1,1),  BTN_SIZE(1,header_h), F("<"))
     .tag(242).enabled(next_enabled).button( BTN_POS(6,1),  BTN_SIZE(1,header_h), F(">"));
}

void FilesScreen::drawFooter() {
  #undef MARGIN_T
  #undef MARGIN_B
  #if defined(USE_PORTRAIT_ORIENTATION)
  #define MARGIN_T 15
  #define MARGIN_B 5
  #else
  #define MARGIN_T 5
  #define MARGIN_B 5
  #endif
  const bool    has_selection = screen_data.FilesScreen.selected_tag != 0xFF;
  const uint8_t back_tag      = screen_data.FilesScreen.flags.is_root ? 240 : 245;
  const uint8_t y             = GRID_ROWS - footer_h + 1;
  const uint8_t h             = footer_h;

  CommandProcessor cmd;
  cmd.font(Theme::font_medium);
  cmd.style(has_selection ? STYLE_DEFAULT : STYLE_LIGHT_BTN)
     .tag(back_tag).button( BTN_POS(4,y), BTN_SIZE(3,h), F("Back"));

  cmd.enabled(has_selection)
     .style(has_selection ? STYLE_LIGHT_BTN : STYLE_DEFAULT);
  if(screen_data.FilesScreen.flags.is_dir) {
    cmd.tag(244).button( BTN_POS(1, y), BTN_SIZE(3,h), F("Open"));
  } else {
    cmd.tag(243).button( BTN_POS(1, y), BTN_SIZE(3,h), F("Print"));
  }
}

void FilesScreen::onRedraw(draw_mode_t what) {
  CommandProcessor cmd;

  if(what & FOREGROUND) {
    default_button_colors();
    drawHeader();
    drawSelectedFile();
    drawFooter();
  }
}

void FilesScreen::gotoPage(uint8_t page) {
  screen_data.FilesScreen.selected_tag = 0xFF;
  screen_data.FilesScreen.cur_page     = page;
  CommandProcessor cmd;
  cmd.cmd(CMD_DLSTART)
     .cmd(CLEAR_COLOR_RGB(Theme::background))
     .cmd(CLEAR(true,true,true));
  default_button_colors();
  drawFileList();
  storeBackground();
}

#undef MARGIN_T
#undef MARGIN_B
#define MARGIN_T MARGIN_DEFAULT
#define MARGIN_B MARGIN_DEFAULT

#undef GRID_COLS
#undef GRID_ROWS

bool FilesScreen::onTouchEnd(uint8_t tag) {
  switch(tag) {
    case 240: GOTO_PREVIOUS();                  return true;
    case 241:
      if(screen_data.FilesScreen.cur_page > 0) {
        gotoPage(screen_data.FilesScreen.cur_page-1);
      }
      break;
    case 242:
      if(screen_data.FilesScreen.cur_page < (screen_data.FilesScreen.num_page-1)) {
        gotoPage(screen_data.FilesScreen.cur_page+1);
      }
      break;
    case 243:
      ExtUI::printFile(getSelectedShortFilename());
      StatusScreen::setStatusMessage(PSTR("Print Starting"));
      GOTO_SCREEN(StatusScreen);
      return true;
    case 244:
      {
        ExtUI::FileList files;
        files.changeDir(getSelectedShortFilename());
        gotoPage(0);
      }
      break;
    case 245:
      {
        ExtUI::FileList files;
        files.upDir();
        gotoPage(0);
      }
      break;
    default:
      if(tag < 240) {
        screen_data.FilesScreen.selected_tag = tag;
      }
      break;
  }
  return true;
}

/******************************* DEVELOPER MENU *****************************/

#if ENABLED(DEVELOPER_SCREENS)
  void DeveloperScreen::onRedraw(draw_mode_t what) {
    if(what & BACKGROUND) {
      CommandProcessor cmd;
      cmd.cmd(CLEAR_COLOR_RGB(Theme::background))
         .cmd(CLEAR(true,true,true))
         .font(Theme::font_medium);

      default_button_colors();

      #ifdef SPI_FLASH_SS
      constexpr bool has_flash = true;
      #else
      constexpr bool has_flash = false;
      #endif

      #if ENABLED(SDSUPPORT)
      constexpr bool has_media = true;
      #else
      constexpr bool has_media = false;
      #endif

      #if defined(USE_PORTRAIT_ORIENTATION)
        #define GRID_ROWS 10
        #define GRID_COLS 1
        cmd.font(Theme::font_large)         .text  ( BTN_POS(1,1), BTN_SIZE(1,1), F("Developer Menu"))
           .tag(2).font(Theme::font_medium) .button( BTN_POS(1,2), BTN_SIZE(1,1), F("Show All Widgets"))
           .tag(3)                          .button( BTN_POS(1,3), BTN_SIZE(1,1), F("Show Touch Registers"))
           .tag(9)                          .button( BTN_POS(1,4), BTN_SIZE(1,1), F("Show Pin States"))
           .tag(4)                          .button( BTN_POS(1,5), BTN_SIZE(1,1), F("Play Song"))
           .tag(5).enabled(has_media)       .button( BTN_POS(1,6), BTN_SIZE(1,1), F("Play Video from Media"))
           .tag(6).enabled(has_flash)       .button( BTN_POS(1,7), BTN_SIZE(1,1), F("Play Video from SPI Flash"))
           .tag(7).enabled(has_flash)       .button( BTN_POS(1,8), BTN_SIZE(1,1), F("Load Video to SPI Flash"))
           .tag(8).enabled(has_flash)       .button( BTN_POS(1,9), BTN_SIZE(1,1), F("Erase SPI Flash"))
           .tag(1).style(STYLE_LIGHT_BTN)   .button( BTN_POS(1,10), BTN_SIZE(1,1), F("Back"));
        #undef GRID_COLS
        #undef GRID_ROWS
      #else
        #define GRID_ROWS 6
        #define GRID_COLS 2
        cmd.font(Theme::font_medium)        .text  ( BTN_POS(1,1), BTN_SIZE(2,1), F("Developer Menu"))
           .tag(2).font(Theme::font_small)  .button( BTN_POS(1,2), BTN_SIZE(1,1), F("Show All Widgets"))
           .tag(3)                          .button( BTN_POS(1,3), BTN_SIZE(1,1), F("Show Touch Registers"))
           .tag(9)                          .button( BTN_POS(1,4), BTN_SIZE(1,1), F("Show Pin States"))
           .tag(4)                          .button( BTN_POS(1,5), BTN_SIZE(1,1), F("Play Song"))
           .tag(5).enabled(has_media)       .button( BTN_POS(2,2), BTN_SIZE(1,1), F("Play Video from Media"))
           .tag(6).enabled(has_flash)       .button( BTN_POS(2,3), BTN_SIZE(1,1), F("Play Video from SPI Flash"))
           .tag(7).enabled(has_flash)       .button( BTN_POS(2,4), BTN_SIZE(1,1), F("Load Video to SPI Flash"))
           .tag(8).enabled(has_flash)       .button( BTN_POS(2,5), BTN_SIZE(1,1), F("Erase SPI Flash"))
           .tag(1).style(STYLE_LIGHT_BTN)   .button( BTN_POS(1,6), BTN_SIZE(2,1), F("Back"));
        #undef GRID_COLS
        #undef GRID_ROWS
      #endif
    }
  }

  bool DeveloperScreen::onTouchEnd(uint8_t tag) {
    switch(tag) {
      case 1:  GOTO_PREVIOUS();                            break;
      case 2:  GOTO_SCREEN(WidgetsScreen);                 break;
      case 3:  GOTO_SCREEN(CalibrationRegistersScreen);    break;
      case 4:  sound.play(js_bach_joy, PLAY_ASYNCHRONOUS); break;
      #if ENABLED(SDSUPPORT)
      case 5: if(!MediaPlayerScreen::playCardMedia()) AlertBoxScreen::showError(F("Cannot open STARTUP.AVI")); break;
      #endif
      #ifdef SPI_FLASH_SS
      case 6: if(!MediaPlayerScreen::playBootMedia()) AlertBoxScreen::showError(F("No boot media available")); break;
      case 7:
      {
        SpinnerScreen::show(F("Saving..."));
        UIFlashStorage::error_t res = UIFlashStorage::write_media_file(F("STARTUP.AVI"));
        SpinnerScreen::hide();
        switch(res) {
          case UIFlashStorage::SUCCESS:         AlertBoxScreen::show(     F("File copied!")); break;
          case UIFlashStorage::READ_ERROR:      AlertBoxScreen::showError(F("Failed to read file")); break;
          case UIFlashStorage::VERIFY_ERROR:    AlertBoxScreen::showError(F("Failed to verify file")); break;
          case UIFlashStorage::FILE_NOT_FOUND:  AlertBoxScreen::showError(F("Cannot open STARTUP.AVI")); break;
          case UIFlashStorage::WOULD_OVERWRITE: AlertBoxScreen::showError(F("Cannot overwrite existing media.")); break;
        }
        break;
      }
      case 8: GOTO_SCREEN(EraseSPIFlashScreen); break;
      #endif
      case 9: GOTO_SCREEN(DiagnosticsScreen); break;
      default: return false;
    }
    return true;
  }
#endif

/**************************** ERASE SPI FLASH SCREEN ***************************/

#if ENABLED(DEVELOPER_SCREENS)
  void EraseSPIFlashScreen::onRedraw(draw_mode_t what) {
    drawMessage(
      F("Are you sure?"),
      F("SPI flash will be erased.")
    );
    drawYesNoButtons();
  }

  bool EraseSPIFlashScreen::onTouchEnd(uint8_t tag) {
    switch(tag) {
      case 1:
        SpinnerScreen::show(F("Erasing..."));
        UIFlashStorage::erase_chip();
        SpinnerScreen::hide();
        AlertBoxScreen::show(F("SPI flash erased"));
        // Remove EraseSPIFlashScreen from the stack
        // so the alert box doesn't return to me.
        current_screen.forget();
        return true;
      default:
        return DialogBoxBaseClass::onTouchEnd(tag);
    }
  }
#endif

/***************************** WIDGET DEMO SCREEN ***************************/

#if ENABLED(DEVELOPER_SCREENS)
  uint16_t dial_val;
  uint16_t slider_val;
  bool     show_grid;

  void WidgetsScreen::onEntry() {
    BaseScreen::onEntry();
    CLCD::turn_on_backlight();
    FTDI::SoundPlayer::set_volume(255);
  }

  void WidgetsScreen::onRedraw(draw_mode_t what) {
    using namespace ExtUI;
    CommandProcessor cmd;
    cmd.cmd(CLEAR_COLOR_RGB(Theme::background))
       .cmd(CLEAR(true,true,true));

    cmd.bgcolor(Theme::theme_dark);

    #if defined(USE_PORTRAIT_ORIENTATION)
      #define GRID_COLS 3
      #define GRID_ROWS 8
      cmd.font(Theme::font_large)
                .text      (BTN_POS(1,1),  BTN_SIZE(3,1), F("Sample Widgets"))
         .tag(2).dial      (BTN_POS(1,2),  BTN_SIZE(1,2), dial_val)
         .tag(0).clock     (BTN_POS(1,4),  BTN_SIZE(1,2), 3, 30, 45, 0)
                .gauge     (BTN_POS(1,6),  BTN_SIZE(1,2), 5, 4, slider_val,  0xFFFFU)

         .font(Theme::font_medium)
         .tag(4).slider    (BTN_POS(2,3),  BTN_SIZE(2,1), slider_val,        0xFFFFU)
         .tag(5).progress  (BTN_POS(2,4),  BTN_SIZE(2,1), slider_val,        0xFFFFU)
         .tag(6).scrollbar (BTN_POS(2,5),  BTN_SIZE(2,1), slider_val, 1000,  0xFFFFU)

         .font(Theme::font_small)
         .tag(0).text      (BTN_POS(2,6),  BTN_SIZE(1,1), F("Show grid:"))
         .tag(7).toggle    (BTN_POS(3,6),  BTN_SIZE(1,1), F("no\xFFyes"), show_grid)

         .font(Theme::font_medium)
         .tag(1).button    (BTN_POS(1, 8), BTN_SIZE(1,1), F("Back"))
                .button    (BTN_POS(2, 8), BTN_SIZE(1,1), F("1"))
                .button    (BTN_POS(3, 8), BTN_SIZE(1,1), F("2"));
    #else
      #define GRID_COLS 4
      #define GRID_ROWS 8

      cmd.font(Theme::font_large)
                .text      (BTN_POS(1,1),  BTN_SIZE(4,1), F("Sample Widgets"))
         .tag(2).dial      (BTN_POS(1,2),  BTN_SIZE(1,3), dial_val)
         .tag(3).dial      (BTN_POS(1,5),  BTN_SIZE(1,3), slider_val)
         .tag(0).clock     (BTN_POS(2,2),  BTN_SIZE(1,3), 3, 30, 45, 0)
                .gauge     (BTN_POS(2,5),  BTN_SIZE(1,3), 5, 4, slider_val,  0xFFFFU)

         .font(Theme::font_medium)
         .tag(4).slider    (BTN_POS(3,3),  BTN_SIZE(2,1), slider_val,        0xFFFFU)
         .tag(5).progress  (BTN_POS(3,4),  BTN_SIZE(2,1), slider_val,        0xFFFFU)
         .tag(6).scrollbar (BTN_POS(3,5),  BTN_SIZE(2,1), slider_val, 1000,  0xFFFFU)

         .font(Theme::font_small)
         .tag(0).text      (BTN_POS(3,6),  BTN_SIZE(1,1), F("Show grid:"))
         .tag(7).toggle    (BTN_POS(4,6),  BTN_SIZE(1,1), F("no\xFFyes"), show_grid)

         .font(Theme::font_medium)
         .tag(1).button    (BTN_POS(1, 8), BTN_SIZE(2,1), F("Back"))
                .button    (BTN_POS(3, 8), BTN_SIZE(1,1), F("1"))
                .button    (BTN_POS(4, 8), BTN_SIZE(1,1), F("2"));
    #endif

    if(show_grid) DRAW_LAYOUT_GRID
  }

  bool WidgetsScreen::onTouchStart(uint8_t tag) {
    CommandProcessor cmd;
    switch(tag) {
      case 1: GOTO_PREVIOUS();                                               break;
    #if defined(USE_PORTRAIT_ORIENTATION)
      case 2: cmd.track_circular (BTN_POS(1,2), BTN_SIZE(1,2), 2).execute(); break;
      case 4: cmd.track_linear   (BTN_POS(2,3), BTN_SIZE(2,1), 4).execute(); break;
      case 5: cmd.track_linear   (BTN_POS(2,4), BTN_SIZE(2,1), 5).execute(); break;
      case 6: cmd.track_linear   (BTN_POS(2,5), BTN_SIZE(2,1), 6).execute(); break;
    #else
      case 2: cmd.track_circular (BTN_POS(1,2), BTN_SIZE(1,3), 2).execute(); break;
      case 3: cmd.track_circular (BTN_POS(1,5), BTN_SIZE(1,3), 3).execute(); break;
      case 4: cmd.track_linear   (BTN_POS(3,3), BTN_SIZE(2,1), 4).execute(); break;
      case 5: cmd.track_linear   (BTN_POS(3,4), BTN_SIZE(2,1), 5).execute(); break;
      case 6: cmd.track_linear   (BTN_POS(3,5), BTN_SIZE(2,1), 6).execute(); break;
    #endif
      case 7: show_grid = !show_grid; break;
      default:
        return false;
    }

    #undef GRID_COLS
    #undef GRID_ROWS

    return true;
  }

  void WidgetsScreen::onIdle() {
    if(refresh_timer.elapsed(TOUCH_UPDATE_INTERVAL)) {
      refresh_timer.start();

      uint16_t value;
      CommandProcessor cmd;
      switch(cmd.track_tag(value)) {
        case 1:
          dial_val   = value; break;
        case 2:
        case 3:
        case 4:
        case 5:
          slider_val = value; break;
        default:
          return;
      }
      onRefresh();
    }
    BaseScreen::onIdle();
  }
#endif

/*************** DEVELOPER MENU: CALIBRATION REGISTERS SCREEN **********************/

#if ENABLED(DEVELOPER_SCREENS)
  void CalibrationRegistersScreen::onRedraw(draw_mode_t what) {
    const uint32_t T_Transform_A = CLCD::mem_read_32(REG_TOUCH_TRANSFORM_A);
    const uint32_t T_Transform_B = CLCD::mem_read_32(REG_TOUCH_TRANSFORM_B);
    const uint32_t T_Transform_C = CLCD::mem_read_32(REG_TOUCH_TRANSFORM_C);
    const uint32_t T_Transform_D = CLCD::mem_read_32(REG_TOUCH_TRANSFORM_D);
    const uint32_t T_Transform_E = CLCD::mem_read_32(REG_TOUCH_TRANSFORM_E);
    const uint32_t T_Transform_F = CLCD::mem_read_32(REG_TOUCH_TRANSFORM_F);
    char b[20];

    CommandProcessor cmd;
    cmd.cmd(CLEAR_COLOR_RGB(Theme::background))
       .cmd(CLEAR(true,true,true));

    #define GRID_ROWS 7
    #define GRID_COLS 2
    cmd.tag(0)
       .font(28)
       .fgcolor(Theme::transformA)  .button( BTN_POS(1,1), BTN_SIZE(1,1), F("TOUCH TRANSFORM_A"))
       .fgcolor(Theme::transformB)  .button( BTN_POS(1,2), BTN_SIZE(1,1), F("TOUCH TRANSFORM_B"))
       .fgcolor(Theme::transformC)  .button( BTN_POS(1,3), BTN_SIZE(1,1), F("TOUCH TRANSFORM_C"))
       .fgcolor(Theme::transformD)  .button( BTN_POS(1,4), BTN_SIZE(1,1), F("TOUCH TRANSFORM_D"))
       .fgcolor(Theme::transformE)  .button( BTN_POS(1,5), BTN_SIZE(1,1), F("TOUCH TRANSFORM_E"))
       .fgcolor(Theme::transformF)  .button( BTN_POS(1,6), BTN_SIZE(1,1), F("TOUCH TRANSFORM_F"))

       .fgcolor(Theme::transformVal).button( BTN_POS(2,1), BTN_SIZE(1,1), F(""), OPT_FLAT)
       .fgcolor(Theme::transformVal).button( BTN_POS(2,2), BTN_SIZE(1,1), F(""), OPT_FLAT)
       .fgcolor(Theme::transformVal).button( BTN_POS(2,3), BTN_SIZE(1,1), F(""), OPT_FLAT)
       .fgcolor(Theme::transformVal).button( BTN_POS(2,4), BTN_SIZE(1,1), F(""), OPT_FLAT)
       .fgcolor(Theme::transformVal).button( BTN_POS(2,5), BTN_SIZE(1,1), F(""), OPT_FLAT)
       .fgcolor(Theme::transformVal).button( BTN_POS(2,6), BTN_SIZE(1,1), F(""), OPT_FLAT);

    sprintf_P(b, PSTR("0x%08lX"), T_Transform_A); cmd.font(28).text  ( BTN_POS(2,1), BTN_SIZE(1,1), b);
    sprintf_P(b, PSTR("0x%08lX"), T_Transform_B); cmd.font(28).text  ( BTN_POS(2,2), BTN_SIZE(1,1), b);
    sprintf_P(b, PSTR("0x%08lX"), T_Transform_C); cmd.font(28).text  ( BTN_POS(2,3), BTN_SIZE(1,1), b);
    sprintf_P(b, PSTR("0x%08lX"), T_Transform_D); cmd.font(28).text  ( BTN_POS(2,4), BTN_SIZE(1,1), b);
    sprintf_P(b, PSTR("0x%08lX"), T_Transform_E); cmd.font(28).text  ( BTN_POS(2,5), BTN_SIZE(1,1), b);
    sprintf_P(b, PSTR("0x%08lX"), T_Transform_F); cmd.font(28).text  ( BTN_POS(2,6), BTN_SIZE(1,1), b);

    cmd.style(STYLE_LIGHT_BTN).tag(1).font(Theme::font_medium).button( BTN_POS(2,7), BTN_SIZE(1,1), F("Back"));
    #undef GRID_COLS
    #undef GRID_ROWS
  }

  bool CalibrationRegistersScreen::onTouchEnd(uint8_t tag) {
    switch(tag) {
      case 1:        GOTO_PREVIOUS();                 break;
      default:
        return false;
    }
    return true;
  }
#endif

/****************** DEVELOPER MENU: DIAGNOSTICS SCREEN *************************/

#if ENABLED(DEVELOPER_SCREENS)
  void DiagnosticsScreen::onEntry() {
    BaseScreen::onEntry();
    #if defined(LULZBOT_ENABLE_PROBE_PINS)
      LULZBOT_ENABLE_PROBE_PINS(true)
    #endif
  }

  void DiagnosticsScreen::onExit() {
    BaseScreen::onExit();
    #if defined(LULZBOT_ENABLE_PROBE_PINS)
      LULZBOT_ENABLE_PROBE_PINS(true)
    #endif
  }

  void DiagnosticsScreen::onRedraw(draw_mode_t what) {
    CommandProcessor cmd;
    cmd.cmd(CLEAR_COLOR_RGB(Theme::background))
       .cmd(CLEAR(true,true,true));

    #define GRID_ROWS 5
    #define GRID_COLS 3

    #define PIN_BTN(X,Y,PIN)          button(BTN_POS(X,Y), BTN_SIZE(1,1), F(#PIN))
    #define PIN_ENABLED(PIN,INV,X,Y)  cmd.enabled(1).style(READ(PIN##_PIN) != INV ? STYLE_RED_BTN : 0).PIN_BTN(X,Y,PIN);
    #define PIN_DISABLED(PIN,INV,X,Y) cmd.enabled(0).PIN_BTN(X,Y,PIN);

    cmd.font(Theme::font_large).text(BTN_POS(1,1),  BTN_SIZE(3,1), F("Pin States")).font(26);
    #if PIN_EXISTS(X_MAX)
      PIN_ENABLED (X_MAX,X_MAX_ENDSTOP_INVERTING,1,2)
    #else
      PIN_DISABLED(X_MAX,X_MAX_ENDSTOP_INVERTING,1,2)
    #endif
    #if PIN_EXISTS(Y_MAX)
      PIN_ENABLED (Y_MAX,Y_MAX_ENDSTOP_INVERTING,2,2)
    #else
      PIN_DISABLED(Y_MAX,Y_MAX_ENDSTOP_INVERTING,2,2)
    #endif
    #if PIN_EXISTS(Z_MAX)
      PIN_ENABLED (Z_MAX,Z_MAX_ENDSTOP_INVERTING,3,2)
    #else
      PIN_DISABLED(Z_MAX,Z_MAX_ENDSTOP_INVERTING,3,2)
    #endif
    #if PIN_EXISTS(X_MIN)
      PIN_ENABLED (X_MIN,X_MIN_ENDSTOP_INVERTING,1,3)
    #else
      PIN_DISABLED(X_MIN,X_MIN_ENDSTOP_INVERTING,1,3)
    #endif
    #if PIN_EXISTS(Y_MIN)
      PIN_ENABLED (Y_MIN,Y_MIN_ENDSTOP_INVERTING,2,3)
    #else
      PIN_DISABLED(Y_MIN,Y_MIN_ENDSTOP_INVERTING,2,3)
    #endif
    #if PIN_EXISTS(Z_MIN)
      PIN_ENABLED (Z_MIN,Z_MIN_ENDSTOP_INVERTING,3,3)
    #else
      PIN_DISABLED(Z_MIN,Z_MIN_ENDSTOP_INVERTING,3,3)
    #endif
    #if PIN_EXISTS(FIL_RUNOUT)
      PIN_ENABLED (FIL_RUNOUT,FIL_RUNOUT_INVERTING,1,4)
    #else
      PIN_DISABLED(FIL_RUNOUT,FIL_RUNOUT_INVERTING,1,4)
    #endif
    #if PIN_EXISTS(FIL_RUNOUT2)
      PIN_ENABLED (FIL_RUNOUT2,FIL_RUNOUT_INVERTING,2,4)
    #else
      PIN_DISABLED(FIL_RUNOUT2,FIL_RUNOUT_INVERTING,2,4)
    #endif
    #if PIN_EXISTS(Z_MIN_PROBE)
      PIN_ENABLED (Z_MIN_PROBE,Z_MIN_PROBE_ENDSTOP_INVERTING,3,4)
    #else
      PIN_DISABLED(Z_MIN_PROBE,Z_MIN_PROBE_ENDSTOP_INVERTING,3,4)
    #endif

    cmd.style(STYLE_LIGHT_BTN).tag(1).font(Theme::font_medium).button( BTN_POS(1,5), BTN_SIZE(3,1), F("Back"));
    #undef GRID_COLS
    #undef GRID_ROWS
  }

  bool DiagnosticsScreen::onTouchEnd(uint8_t tag) {
    switch(tag) {
      case 1:        GOTO_PREVIOUS();                 break;
      default:
        return false;
    }
    return true;
  }

  void DiagnosticsScreen::onIdle() {
    constexpr uint32_t DIAGNOSTICS_UPDATE_INTERVAL = 100;

    if(refresh_timer.elapsed(DIAGNOSTICS_UPDATE_INTERVAL)) {
      onRefresh();
      refresh_timer.start();
      reset_menu_timeout();
    }
    BaseScreen::onIdle();
  }
#endif

/***************************** MEDIA PLAYER SCREEN ***************************/

void MediaPlayerScreen::onEntry() {
  BaseScreen::onEntry();
  CLCD::turn_on_backlight();
  FTDI::SoundPlayer::set_volume(255);
}

void MediaPlayerScreen::onRedraw(draw_mode_t what) {
}

bool MediaPlayerScreen::playCardMedia() {
  char fname[15];
  strcpy_P(fname, PSTR("STARTUP.AVI"));

  MediaFileReader reader;
  if(!reader.open(fname))
    return false;

  SERIAL_ECHO_START();
  SERIAL_ECHOLNPGM("Starting to play STARTUP.AVI");
  playStream(&reader, MediaFileReader::read);
  reader.close();
  return true;
}

// Attempt to play media from the onboard SPI flash chip
bool MediaPlayerScreen::playBootMedia() {
  UIFlashStorage::BootMediaReader reader;
  if(!reader.isAvailable()) return false;

  SERIAL_ECHO_START();
  SERIAL_ECHOLNPGM("Starting to play boot video");
  playStream(&reader, UIFlashStorage::BootMediaReader::read);
  return true;
}

void MediaPlayerScreen::playStream(void *obj, media_streamer_func_t *data_stream) {
  #if defined(USE_FTDI_FT810)
    // Set up the media FIFO on the end of RAMG, as the top of RAMG
    // will be used as the framebuffer.

    uint8_t        buf[512];
    const uint32_t block_size = 512;
    const uint32_t fifo_size  = block_size * 2;
    const uint32_t fifo_start = RAM_G + RAM_G_SIZE - fifo_size;

    CommandProcessor cmd;
    cmd.cmd(CMD_DLSTART)
       .cmd(CLEAR_COLOR_RGB(0x000000))
       .cmd(CLEAR(true,true,true))
       .cmd(DL::DL_DISPLAY)
       .cmd(CMD_SWAP)
       .execute()
       .cmd(CMD_DLSTART)
       .mediafifo(fifo_start, fifo_size)
       .playvideo(OPT_FULLSCREEN | OPT_MEDIAFIFO | OPT_NOTEAR | OPT_SOUND)
       .cmd(DL::DL_DISPLAY)
       .cmd(CMD_SWAP)
       .execute();

    uint32_t writePtr = 0;
    int16_t  nBytes;

    uint32_t t = millis();
    uint8_t timeouts;

    spiInit(SPI_HALF_SPEED); // Boost SPI speed for video playback

    do {
      // Write block n
      nBytes = (*data_stream)(obj, buf, block_size);
      if(nBytes == -1) break;

      if(millis() - t > 10) {
        ExtUI::yield();
        watchdog_reset();
        t = millis();
      }

      CLCD::mem_write_bulk (fifo_start + writePtr, buf, nBytes);

      // Wait for FTDI810 to finish playing block n-1
      timeouts = 20;
      do {
        if(millis() - t > 10) {
          ExtUI::yield();
          watchdog_reset();
          t = millis();
          timeouts--;
          if(timeouts == 0) {
            SERIAL_ECHO_START();
            SERIAL_ECHOLNPGM("Timeout playing video");
            cmd.reset();
            goto exit;
          }
        }
      } while(CLCD::mem_read_32(REG_MEDIAFIFO_READ) != writePtr);

      // Start playing block n
      writePtr = (writePtr + nBytes) % fifo_size;
      CLCD::mem_write_32(REG_MEDIAFIFO_WRITE, writePtr);
    } while(nBytes == block_size);

    SERIAL_ECHO_START();
    SERIAL_ECHOLNPGM("Done playing video");

  exit:
    spiInit(SPI_SPEED); // Restore default speed

    // Since playing media overwrites RAMG, we need to reinitialize
    // everything that is stored in RAMG.
    cmd.cmd(CMD_DLSTART).execute();
    DLCache::init();
    StatusScreen::onStartup();
  #endif // USE_FTDI_FT810
}

/***************************** MARLIN CALLBACKS  ***************************/

namespace ExtUI {
  void onPrinterKilled(const char* lcd_msg) {
    KillScreen::show(progmem_str(lcd_msg));
  }

  void onMediaInserted() {
    StatusScreen::setStatusMessage(F(MSG_SD_INSERTED));
    sound.play(media_inserted, PLAY_ASYNCHRONOUS);
  }

  void onMediaRemoved() {
    StatusScreen::setStatusMessage(F(MSG_SD_REMOVED));
    sound.play(media_removed, PLAY_ASYNCHRONOUS);
    if(AT_SCREEN(FilesScreen)) {
      GOTO_SCREEN(StatusScreen)
    }
  }

  void onMediaError() {
    sound.play(sad_trombone, PLAY_ASYNCHRONOUS);
    AlertBoxScreen::showError(F("Unable to read media."));
  }

  void onStatusChanged(const char* lcd_msg) {
    StatusScreen::setStatusMessage(lcd_msg);
  }

  void onStatusChanged(progmem_str lcd_msg) {
    StatusScreen::setStatusMessage(lcd_msg);
  }

  void onPrintTimerStarted() {
    InterfaceSoundsScreen::playEventSound(InterfaceSoundsScreen::PRINTING_STARTED);
  }

  void onPrintTimerStopped() {
    InterfaceSoundsScreen::playEventSound(InterfaceSoundsScreen::PRINTING_FINISHED);
  }

  void onPrintTimerPaused() {
  }

  void onFilamentRunout(const extruder_t extruder) {
    char lcd_msg[30];
    sprintf_P(lcd_msg, PSTR("Extruder %d Filament Error"), extruder + 1);
    StatusScreen::setStatusMessage(lcd_msg);
    InterfaceSoundsScreen::playEventSound(InterfaceSoundsScreen::PRINTING_FAILED);
  }

  void onFactoryReset() {
    InterfaceSettingsScreen::defaultSettings();
  }

  void onStoreSettings() {
    InterfaceSettingsScreen::saveSettings();
  }

  void onLoadSettings() {
    InterfaceSettingsScreen::loadSettings();
  }
}

#endif // EXTENSIBLE_UI