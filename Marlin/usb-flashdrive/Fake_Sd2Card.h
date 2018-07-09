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
 * \file
 * \brief Sd2Card class for V2 SD/SDHC cards
 */

#ifndef _FAKE_SD2CARD_H_
#define _FAKE_SD2CARD_H_

#include "SdFatConfig.h"
#include "SdInfo.h"

// SPI speed is F_CPU/2^(1 + index), 0 <= index <= 6
uint8_t const SPI_FULL_SPEED = 0,         // Set SCK to max rate of F_CPU/2. See Sd2Card::setSckRate().
              SPI_HALF_SPEED = 1,         // Set SCK rate to F_CPU/4. See Sd2Card::setSckRate().
              SPI_QUARTER_SPEED = 2,      // Set SCK rate to F_CPU/8. See Sd2Card::setSckRate().
              SPI_EIGHTH_SPEED = 3,       // Set SCK rate to F_CPU/16. See Sd2Card::setSckRate().
              SPI_SIXTEENTH_SPEED = 4;    // Set SCK rate to F_CPU/32. See Sd2Card::setSckRate().

/**
 * define SOFTWARE_SPI to use bit-bang SPI
 */
#if MEGA_SOFT_SPI
  #define SOFTWARE_SPI
#elif USE_SOFTWARE_SPI
  #define SOFTWARE_SPI
#endif

// SPI pin definitions - do not edit here - change in SdFatConfig.h
#if DISABLED(SOFTWARE_SPI)
  // hardware pin defs
  #define SD_CHIP_SELECT_PIN SS_PIN   // The default chip select pin for the SD card is SS.
  // The following three pins must not be redefined for hardware SPI.
  #define SPI_MOSI_PIN MOSI_PIN       // SPI Master Out Slave In pin
  #define SPI_MISO_PIN MISO_PIN       // SPI Master In Slave Out pin
  #define SPI_SCK_PIN SCK_PIN         // SPI Clock pin
#else  // SOFTWARE_SPI
  #define SD_CHIP_SELECT_PIN SOFT_SPI_CS_PIN  // SPI chip select pin
  #define SPI_MOSI_PIN SOFT_SPI_MOSI_PIN      // SPI Master Out Slave In pin
  #define SPI_MISO_PIN SOFT_SPI_MISO_PIN      // SPI Master In Slave Out pin
  #define SPI_SCK_PIN SOFT_SPI_SCK_PIN        // SPI Clock pin
#endif  // SOFTWARE_SPI

/**
 * \class Sd2Card
 * \brief Raw access to SD and SDHC flash memory cards.
 */
class Sd2Card {
  private:
    static bool usbHostReady();

  public:
    static bool isInserted();

    /**
     * Initialize an SD flash memory card with default clock rate and chip
     * select pin.  See sd2Card::init(uint8_t sckRateID, uint8_t chipSelectPin).
     *
     * \return true for success or false for failure.
     */
    bool init(uint8_t sckRateID = 0, uint8_t chipSelectPin = SD_CHIP_SELECT_PIN);
    bool readBlock(uint32_t block, uint8_t* dst);
    bool writeBlock(uint32_t blockNumber, const uint8_t* src);
};

#endif  // _FAKE_SD2CARD_H_
