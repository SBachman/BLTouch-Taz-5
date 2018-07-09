/********************************************************************************************
 * This program/sketch is used to run a USB Thumb Drive.                                    *
 *                                                                                          *
 * NOTE - This Arduino Sketch has been modified to initialize a MAX3421E USB Host Interface *
 * chip, write 3 test files, print out the directory of the thumb drive and print out the   *
 * contents of a short .txt file.                                                           *
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
#include "masstorage.h"

#define MAX_USB_RST 7

// USB host objects.v
USB usb;
BulkOnly bulk(&usb);

#define error(msg) {Serial.print("Error: "); Serial.println(msg);}

#define TIMEOUT_MILLIS 4000

//------------------------------------------------------------------------------
bool initUSB(USB* usb) {
 uint8_t last_state = 0;
 uint8_t current_state = 0;
  uint32_t m = millis();
  
  for (uint8_t i = 0; usb->Init(1000) == -1; i++)
  {
    //if (USB_FAT_DBG_MODE) {
      Serial.println(F("No USB HOST Shield?"));
    //}
    if (i > 10) {
      return false;
    }
  }
#if USB_FAT_DBG_MODE
  Serial.print(F("Host initialized, ms: "));
  Serial.println(millis() - m);
#endif  // USB_FAT_DBG_MODE

  usb->vbusPower(vbus_on);
#if USB_FAT_DBG_MODE  
  Serial.print(F("USB powered, ms: "));
  Serial.println(millis() - m);
#endif  // USB_FAT_DBG_MODE
  
  while ((millis() - m) < TIMEOUT_MILLIS) {
    usb->Task();  
    current_state = usb->getUsbTaskState();
#if USB_FAT_DBG_MODE    
    if (last_state != current_state) {
      Serial.print(F("USB state: "));
      Serial.print(current_state, HEX);
      Serial.print(F(", ms: "));
      Serial.println(millis() - m);
    }
    last_state = current_state;
#endif  // USB_FAT_DBG_MODE    
    if(current_state == USB_STATE_RUNNING) {
      return true;
    }
  }
  return false;
}

//------------------------------------------------------------------------------
void setup()
{
  pinMode(MAX_USB_RST, OUTPUT);
  digitalWrite(MAX_USB_RST, HIGH);
  
  Serial.begin(9600);

  Serial.print("USB THUMB DRIVE FILE TEST\n\n");

  Serial.print("Initializing The USB Bus\n");

  // Initialize the USB bus.
  
  if (!initUSB(&usb))
  {
    error("initUSB failed");   
  }
  else
  {
    Serial.print("USB Initialized\n");
  }

  if(bulk.LUNIsGood(0)) {
    Serial.print("LUN Capacity: ");
    Serial.println(bulk.GetCapacity(0));

    const uint32_t sectorSize = bulk.GetSectorSize(0);
    
    Serial.print("Sector Size: ");
    Serial.println(sectorSize);

    uint8_t buf[512];
    
    
    const uint8_t  lun = 0;
    const uint32_t addr = 0;
    if(bulk.Read(lun, addr, sectorSize, 1, buf) == 0) {
      Serial.print("Read a block: \n");
      Serial.println((char*)buf);
    } else {
      Serial.print("Failed to a read block\n");
    }

    const char *message = PSTR("This is a test of writing raw data!");
    strcpy_P(buf, message);

    if(bulk.Write(lun, addr, sectorSize, 1, buf) == 0) {
      Serial.print("Wrote a block\n");
    } else {
      Serial.print("Failed to write a block\n");
    }
  } else {
    Serial.print("LUN zero is not good\n");
  }
}
//------------------------------------------------------------------------------
void loop () {}
