/**
 * Pins for the Aleph Objects Color LCD Touchscreen.
 *
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

#if defined(LULZBOT_Quiver_TAZ7_CLCD)                                   // ARCHIM       J20 HEADER   +3.3V Pin-24   GND Pin-23
    #define CLCD_BB_SPI_CS                 99                           // PB6 J20 Pin-9
    #define CLCD_BB_SPI_SCLK               95                           // PB0 J20 Pin-6
    #define CLCD_BB_SPI_MOSI               94                           // PB1 J20 Pin-5
    #define CLCD_BB_SPI_MISO              101                           // PB5 J20 Pin-10
    #define CLCD_MOD_RESET                 93                           // PB2 J20 Pin-8
#endif

#if defined(LULZBOT_Gladiola_Mini_CLCD)                                 // MINI RAMBO   P1 HEADER  +5V Pin-10  GND Pin-9
    #define CLCD_BB_SPI_SCLK               71                           // PG3 P1 Pin-8
    #define CLCD_BB_SPI_MOSI               85                           // PH7 P1 Pin-7
    #define CLCD_BB_SPI_CS                 70                           // PG4 P1 Pin-6
    #define CLCD_MOD_RESET                 19                           // PD2 P1 Pin-5
    #define CLCD_BB_SPI_MISO               82                           // PD5 P1 Pin-4
    #define CLCD_AUX_0                     18                           // PD3 P1 Pin-3
    #define CLCD_AUX_1                      9                           // PH6 P1 Pin-2
    #define CLCD_AUX_2                     84                           // PH2 P1 Pin-1
#endif

#if defined(LULZBOT_Hibiscus_Mini2_CLCD)                           // EINSY        P1 HEADER  +5V Pin-1  GND Pin-2
    #define CLCD_BB_SPI_SCLK               71                           // PG3 P1 Pin-3
    #define CLCD_BB_SPI_MOSI               85                           // PH7 P1 Pin-4
    #define CLCD_BB_SPI_CS                 70                           // PG4 P1 Pin-5
    #define CLCD_MOD_RESET                 19                           // PD2 P1 Pin-6
    #define CLCD_BB_SPI_MISO               82                           // PD5 P1 Pin-7
    #define CLCD_AUX_0                     18                           // PD3 P1 Pin-8
    #define CLCD_AUX_1                      9                           // PH6 P1 Pin-9
    #define CLCD_AUX_2                     84                           // PH2 P1 Pin-10
#endif