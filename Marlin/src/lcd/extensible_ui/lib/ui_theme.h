/**************
 * ui_theme.h *
 **************/

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

#ifndef _UI_THEME_
#define _UI_THEME_

/************************************ MENU THEME ********************************/

namespace Theme {
  #define COLOR_CORRECTION(rgb)  ( \
    (uint32_t((((rgb) & 0xFF0000) >> 16) * 1.00) << 16) | \
    (uint32_t((((rgb) & 0x00FF00) >>  8) * 1.00) <<  8) | \
    (uint32_t((((rgb) & 0x0000FF) >>  0) *  .75) <<  0))

  #define COLOR_BLEND(a,b,f) COLOR_CORRECTION( \
    (uint32_t((((a) & 0xFF0000) >> 16) *    f + (((b) & 0xFF0000) >> 16) * (1-f))  << 16) | \
    (uint32_t((((a) & 0x00FF00) >>  8) *    f + (((b) & 0x00FF00) >>  8) * (1-f))  <<  8) | \
    (uint32_t((((a) & 0x0000FF) >>  0) *    f + (((b) & 0x0000FF) >>  0) * (1-f))  <<  0))

  constexpr uint32_t theme_darkest = COLOR_CORRECTION(0x444444);
  constexpr uint32_t theme_dark    = COLOR_CORRECTION(0x777777);

  constexpr uint32_t background    = theme_darkest;

  constexpr uint32_t text_enabled  = 0xFFFFFF;
  constexpr uint32_t text_disabled = theme_dark;


  constexpr uint32_t x_axis        = COLOR_CORRECTION(0xFF0000);
  constexpr uint32_t y_axis        = COLOR_CORRECTION(0x00BB00);
  constexpr uint32_t z_axis        = COLOR_CORRECTION(0x0000FF);
  constexpr uint32_t e_axis        = theme_dark;
  constexpr uint32_t feedrate      = theme_dark;
  constexpr uint32_t other         = theme_dark;

  // Files screens

  constexpr uint32_t files_selected = COLOR_BLEND(0xC1DB2F,0x788814,0.33);

  // Status screen
  constexpr uint32_t progress      = theme_dark;
  constexpr uint32_t status_msg    = theme_dark;
  constexpr uint32_t fan_speed     = COLOR_CORRECTION(0x3771CB);
  constexpr uint32_t temp          = COLOR_CORRECTION(0x892ca0);
  constexpr uint32_t axis_label    = theme_dark;

  constexpr uint32_t disabled_icon = 0x101010;

  // Calibration Registers Screen
  constexpr uint32_t transformA    = 0x3010D0;
  constexpr uint32_t transformB    = 0x4010D0;
  constexpr uint32_t transformC    = 0x5010D0;
  constexpr uint32_t transformD    = 0x6010D0;
  constexpr uint32_t transformE    = 0x7010D0;
  constexpr uint32_t transformF    = 0x8010D0;
  constexpr uint32_t transformVal  = 0x104010;

  // Disabled vs enabled buttons
  struct default_btn {
    static constexpr uint32_t rgb_enabled  = 0xFFFFFF;
    static constexpr uint32_t grad_enabled = 0xFFFFFF;
    static constexpr uint32_t fg_enabled   = COLOR_BLEND(0x999999,0x666666,0.33);
    static constexpr uint32_t rgb_disabled = background;
    static constexpr uint32_t fg_disabled  = background;
  };

  struct light_btn {
    static constexpr uint32_t rgb_enabled  = 0xFFFFFF;
    static constexpr uint32_t grad_enabled = 0xFFFFFF;
    static constexpr uint32_t fg_enabled   = COLOR_BLEND(0xC1DB2F,0x788814,0.33);
    static constexpr uint32_t rgb_disabled = background;
    static constexpr uint32_t fg_disabled  = background;
  };

  struct red_btn {
    static constexpr uint32_t rgb_enabled  = 0xFFFFFF;
    static constexpr uint32_t grad_enabled = 0xFFFFFF;
    static constexpr uint32_t fg_enabled   = 0xFF0000;
    static constexpr uint32_t rgb_disabled = background;
    static constexpr uint32_t fg_disabled  = background;
  };

#if defined(LCD_800x480)
  #if defined(USE_PORTRAIT_ORIENTATION)
    constexpr int16_t  font_small    = 29;
    constexpr int16_t  font_medium   = 30;
    constexpr int16_t  font_large    = 30;
    constexpr int16_t  font_xlarge   = 31;
  #else
    constexpr int16_t  font_small    = 30;
    constexpr int16_t  font_medium   = 30;
    constexpr int16_t  font_large    = 31;
    constexpr int16_t  font_xlarge   = 31;
  #endif
  constexpr float      icon_scale    = 1.0;
#elif defined(LCD_480x272)
  #if defined(USE_PORTRAIT_ORIENTATION)
  constexpr int16_t  font_small    = 26;
  constexpr int16_t  font_medium   = 27;
  constexpr int16_t  font_large    = 28;
  constexpr int16_t  font_xlarge   = 29;
  constexpr float    icon_scale    = 0.7;
  #else
  constexpr int16_t  font_small    = 27;
  constexpr int16_t  font_medium   = 28;
  constexpr int16_t  font_large    = 30;
  constexpr int16_t  font_xlarge   = 31;
  constexpr float    icon_scale    = 0.6;
  #endif
#elif defined(LCD_320x240)
  #if defined(USE_PORTRAIT_ORIENTATION)
  constexpr int16_t  font_small    = 26;
  constexpr int16_t  font_medium   = 27;
  constexpr int16_t  font_large    = 27;
  constexpr int16_t  font_xlarge   = 28;
  constexpr float    icon_scale    = 0.6;
  #else
  constexpr int16_t  font_small    = 26;
  constexpr int16_t  font_medium   = 27;
  constexpr int16_t  font_large    = 29;
  constexpr int16_t  font_xlarge   = 30;
  constexpr float    icon_scale    = 0.5;
  #endif
#endif

  constexpr FTDI::effect_t press_sound   = FTDI::CHACK;
  constexpr FTDI::effect_t repeat_sound  = FTDI::CHACK;
  constexpr FTDI::effect_t unpress_sound = FTDI::POP;

/*************************** COLOR SCALES **********************************/

  const FTDI::rgb_t cool_rgb (  0,   0,   0);
  const FTDI::rgb_t low_rgb  (128,   0,   0);
  const FTDI::rgb_t med_rgb  (255, 128,   0);
  const FTDI::rgb_t high_rgb (255, 255, 128);

  uint32_t getWarmColor(uint16_t temp, uint16_t cool, uint16_t low, uint16_t med, uint16_t high);

};
#endif // _UI_THEME_