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

/********************************************************************************************
 *                                                                                          *
 * The code is leveraged from the following:                                                *
 *                                                                                          *
 * Copyright (C) 2011 Circuits At Home, LTD. All rights reserved.                           *
 *                                                                                          *
 * This software may be distributed and modified under the terms of the GNU                 *
 * General Public License version 2 (GPL2) as published by the Free Software                *
 * Foundation and appearing in the file GPL2.TXT included in the packaging of               *
 * this file. Please note that GPL2 Section 2[b] requires that all works based              *
 * on this software must also be made publicly available under the terms of                 *
 * the GPL2 ("Copyleft").                                                                   *
 *                                                                                          *
 * Contact information                                                                      *
 * -------------------                                                                      *
 *                                                                                          *
 * Circuits At Home, LTD                                                                    *
 * Web      :  http://www.circuitsathome.com                                                *
 * e-mail   :  support@circuitsathome.com                                                   *
 *                                                                                          *
 * SPECIAL NOTE - In order to work with a modified Eisny or RAMBo, the SPI chip select pin  *
 * (CS) (D10) has been remapped from PORTB Pin-4 to PORTB Pin-0.  This has been done in the *
 * __AVR_ATmega2560__ section of the avrpins.h file.                                        *
 *                                                                                          *
 ********************************************************************************************/

#include <SPI.h>

#include "Marlin.h"

#define USB_HOST_SERIAL customizedSerial

#if defined(MACROS_H)
  #undef MACROS_H // The USB host library wants to redefine this.
  #include "lib/masstorage.cpp"
  #include "lib/message.cpp"
  #include "lib/Usb.cpp"
#else
  #include "lib/masstorage.cpp"
  #include "lib/message.cpp"
  #include "lib/Usb.cpp"
  #undef MACROS_H
#endif

#include "Fake_Sd2Card.h"

USB usb;
BulkOnly bulk(&usb);

bool Sd2Card::usbHostReady() {
  static enum {
    USB_HOST_UNINITIALIZED,
    USB_HOST_FAILED_INIT,
    USB_HOST_INITIALIZED
  } state = USB_HOST_UNINITIALIZED;

  if(state == USB_HOST_UNINITIALIZED) {
     if(usb.Init(1000) == -1) {
       SERIAL_ECHOLNPGM("USB host failed to initialize");
       state = USB_HOST_FAILED_INIT;
     } else {
       usb.vbusPower(vbus_on);
       SERIAL_ECHOLNPGM("USB host initialized");
       state = USB_HOST_INITIALIZED;
     }
  }

  return state == USB_HOST_INITIALIZED;
}

// Marlin will call this to learn whether an SD card is inserted.
bool Sd2Card::isInserted() {
   if(usbHostReady()) {
    const uint8_t lastUsbTaskState = usb.getUsbTaskState();
    usb.Task();
    const uint8_t newUsbTaskState  = usb.getUsbTaskState();
    if(lastUsbTaskState == USB_STATE_RUNNING && newUsbTaskState != USB_STATE_RUNNING) {
      // the user pulled the flash drive. Make sure the bulk storage driver releases the address
      SERIAL_ECHOLNPGM("Drive removed\n");
      bulk.Release();
    }
    return newUsbTaskState == USB_STATE_RUNNING;
   }
}

// Marlin calls this whenever an SD card is detected, so this method
// should not be used to initialize the USB host library
bool Sd2Card::init(uint8_t sckRateID, uint8_t chipSelectPin) {
  if(!usbHostReady())
    return false;

  if(!bulk.LUNIsGood(0)) {
    SERIAL_ECHOLNPGM("LUN zero is not good\n");
    return false;
  }

  SERIAL_ECHOLNPAIR("LUN Capacity: ",bulk.GetCapacity(0));

  const uint32_t sectorSize = bulk.GetSectorSize(0);
  if(sectorSize != 512) {
    SERIAL_ECHOLNPAIR("Expecting sector size of 512, got: ",sectorSize);
    return false;
  }

  return true;
}

bool Sd2Card::readBlock(uint32_t block, uint8_t* dst) {
  return bulk.Read(0, block, 512, 1, dst) == 0;
}

bool Sd2Card::writeBlock(uint32_t blockNumber, const uint8_t* src) {
  return bulk.Write(0, blockNumber, 512, 1, src) == 0;
}