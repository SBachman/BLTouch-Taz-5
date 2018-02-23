/*********************
 * FT810_SPI.h *
 *********************/


/****************************************************************************
 *   Written By Mark Pelletier 2017 - Aleph Objects, Inc.                   *
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
#ifndef _AO_FT810_SPI_H
#define _AO_FT810_SPI_H

#if defined(PIN_DIR_OUT)
  // If SET_OUTPUT is defined, then map Marlin routines to those
  #define SET_OUTPUT                PIN_DIR_OUT
  #define SET_INPUT_PULLUP(a)       _PIN_DIR_IN(a); _PIN_HIGH(a);
  #define SET_INPUT                 PIN_DIR_IN
  #define WRITE                     SET_PIN
  #define READ                      GET_PIN
#endif

/********************************* SPI Functions *********************************/

void CLCD::spiInit (void) {
#if defined(USE_ARDUINO_HW_SPI)
  SPI.begin();
  SPI.beginTransaction(LCDsettings);
#else
  SET_OUTPUT(CLCD_MOD_RESET);                                           // CLCD_MOD_RST - Module Reset, not SPI
  WRITE(CLCD_MOD_RESET, 1);

  SET_OUTPUT(CLCD_BB_SPI_MOSI);                                         // CLCD_MOSI
  WRITE(CLCD_BB_SPI_MOSI, 1);

  SET_OUTPUT(CLCD_BB_SPI_SCLK);                                         // CLCD_SCLK
  WRITE(CLCD_BB_SPI_SCLK, 0);

  SET_OUTPUT(CLCD_BB_SPI_CS);                                           // CLCD_CS
  WRITE(CLCD_BB_SPI_CS, 1);

  SET_INPUT_PULLUP(CLCD_BB_SPI_MISO);                                   // CLCD_MISO

  delay(50);
#endif
}

void CLCD::spiSelect (void) {                                             // CLCD Bitbanged SPI - Chip Select
  WRITE(CLCD_BB_SPI_CS, 0);
}


void CLCD::spiDeselect (void) {                                           // CLCD Bitbanged SPI - Chip Deselect
  WRITE(CLCD_BB_SPI_CS, 1);
}

void CLCD::Reset (void) {
  WRITE(CLCD_MOD_RESET, 0);
  delay(100);
  WRITE(CLCD_MOD_RESET, 1);
}

void CLCD::Test_Pulse(void)
{
  WRITE(CLCD_AUX_0, 1);
  delayMicroseconds(10);
  WRITE(CLCD_AUX_0, 0);
}

uint8_t CLCD::spiTransfer (uint8_t spiOutByte) {
#ifdef IS_ARDUINO
  SPI.transfer(spiOutByte);
#else

  uint8_t spiIndex  = 0x80;
  uint8_t spiInByte = 0;
  uint8_t k;

  for(k = 0; k <8; k++) {                                               // Output and Read each bit of spiOutByte and spiInByte
    if(spiOutByte & spiIndex) {                                         // Output MOSI Bit
      WRITE(CLCD_BB_SPI_MOSI, 1);
    }
    else {
      WRITE(CLCD_BB_SPI_MOSI, 0);
    }
    WRITE(CLCD_BB_SPI_SCLK, 1);                                                      // Pulse Clock
    WRITE(CLCD_BB_SPI_SCLK, 0);

    if(READ(CLCD_BB_SPI_MISO)) {
      spiInByte |= spiIndex;
    }

    spiIndex >>= 1;
  }
  return spiInByte;
#endif
}

// MEMORY READ FUNCTIONS

void CLCD::Mem_ReadAddr (uint32_t Reg_Address) {                         // Write 4-Byte Address
  CLCD::spiTransfer((Reg_Address >> 16) & 0x3F);                         // Address [21:16]
  CLCD::spiTransfer((Reg_Address >> 8 ) & 0xFF);                         // Address [15:8]
  CLCD::spiTransfer((Reg_Address >> 0)  & 0xFF);                         // Address [7:0]
  CLCD::spiTransfer(0x00);                                               // Dummy Byte
}

void CLCD::Mem_Read_Bulk (uint32_t Reg_Address, uint8_t *Data, uint16_t Len) { // Write 4-Byte Address, Read Multiple Bytes
  CLCD::spiSelect();
  CLCD::Mem_ReadAddr(Reg_Address);

  while(Len--) {
    *Data = CLCD::spiTransfer(0x00);                                     // Read 1 Byte
    *Data++;
  }

  CLCD::spiDeselect();
}

uint8_t CLCD::Mem_Read8 (uint32_t Reg_Address) {                         // Write 4-Byte Address, Read 1-Byte Data
  CLCD::spiSelect();

  CLCD::Mem_ReadAddr(Reg_Address);

  uint8_t R_Data = CLCD::spiTransfer(0x00);                              // Read 1 Byte

  CLCD::spiDeselect();

  return(R_Data);
}


uint16_t CLCD::Mem_Read16 (uint32_t Reg_Address) {                       // Write 4-Byte Address, Read 2-Bytes Data
  CLCD::spiSelect();

  CLCD::Mem_ReadAddr(Reg_Address);

  uint16_t R_Data =  (((uint16_t) CLCD::spiTransfer(0x00)) << 0) |          // Read Byte 1
                     (((uint16_t) CLCD::spiTransfer(0x00)) << 8);           // Read Byte 2

  CLCD::spiDeselect();

  return(R_Data);
}

uint32_t CLCD::Mem_Read32 (uint32_t Reg_Address) {                       // Write 4-Byte Address, Read 4-Bytes Data
  CLCD::spiSelect();

  CLCD::Mem_ReadAddr(Reg_Address);

  uint32_t R_Data =  (((uint32_t) CLCD::spiTransfer(0x00)) <<  0) |         // Read Byte 1
                     (((uint32_t) CLCD::spiTransfer(0x00)) <<  8) |         // Read Byte 2
                     (((uint32_t) CLCD::spiTransfer(0x00)) << 16) |         // Read Byte 3
                     (((uint32_t) CLCD::spiTransfer(0x00)) << 24);          // Read Byte 4

  CLCD::spiDeselect();

  return(R_Data);
}


// MEMORY WRITE FUNCTIONS

void CLCD::Mem_WriteAddr (uint32_t Reg_Address) {                        // Write 3-Byte Address
  CLCD::spiTransfer((Reg_Address >> 16) | 0x80);                         // Address [21:16]
  CLCD::spiTransfer((Reg_Address >> 8 ) & 0xFF);                         // Address [15:8]
  CLCD::spiTransfer((Reg_Address >> 0)  & 0xFF);                         // Address [7:0]
}

void CLCD::Mem_Write_Bulk (uint32_t Reg_Address, const void *Data, uint16_t Len, uint8_t Padding) { // Write 3-Byte Address, Multiple Bytes, plus padding bytes
  const uint8_t* p = (const uint8_t *)Data;
  CLCD::spiSelect();
  CLCD::Mem_WriteAddr(Reg_Address);

  while(Len--) {
    CLCD::spiTransfer(*p++);                                     // Write 1 Byte
  }

  while(Padding--) {
    CLCD::spiTransfer(0);                                           // Padding Bytes
  }

  CLCD::spiDeselect();
}

void CLCD::Mem_Write_Bulk (uint32_t Reg_Address, progmem_str Str, uint16_t Len, uint8_t Padding) { // Write 3-Byte Address, Multiple Bytes, plus padding bytes
  const uint8_t* p = (const uint8_t *)Str;
  CLCD::spiSelect();
  CLCD::Mem_WriteAddr(Reg_Address);

  while(Len--) {
    CLCD::spiTransfer(pgm_read_byte_near(p++));                                     // Write 1 Byte
  }

  while(Padding--) {
    CLCD::spiTransfer(0);                                           // Padding Bytes
  }

  CLCD::spiDeselect();
}

void CLCD::Mem_Write8 (uint32_t Reg_Address, uint8_t W_Data) {           // Write 3-Byte Address, Write 1-Byte Data
  CLCD::spiSelect();

  CLCD::Mem_WriteAddr(Reg_Address);

  CLCD::spiTransfer(W_Data);                                             // Write 1 Byte

  CLCD::spiDeselect();
}


void CLCD::Mem_Write16 (uint32_t Reg_Address, uint16_t W_Data) {        // Write 3-Byte Address, Write 2-Bytes Data
  CLCD::spiSelect();

  CLCD::Mem_WriteAddr(Reg_Address);

  CLCD::spiTransfer((uint8_t) ((W_Data >> 0) & 0x00FF));                        // Write Byte 0
  CLCD::spiTransfer((uint8_t) ((W_Data >> 8) & 0x00FF));                 // Write Byte 1

  CLCD::spiDeselect();
}


void CLCD::Mem_Write32 (uint32_t Reg_Address, uint32_t W_Data) {         // Write 3-Byte Address, Write 4-Bytes Data
  CLCD::spiSelect();

  CLCD::Mem_WriteAddr(Reg_Address);

  CLCD::spiTransfer(W_Data >> 0);    // Write Byte 0
  CLCD::spiTransfer(W_Data >> 8);    // Write Byte 1
  CLCD::spiTransfer(W_Data >> 16);   // Write Byte 2
  CLCD::spiTransfer(W_Data >> 24);   // Write Byte 3

  CLCD::spiDeselect();
}

/**************************** FT800/810 Co-Processor Command FIFO ****************************/

uint32_t CLCD::CommandFifo::getRegCmdWrite() {
  return Mem_Read32(REG_CMD_WRITE) & 0x0FFF;
}

uint32_t CLCD::CommandFifo::getRegCmdRead() {
  return Mem_Read32(REG_CMD_READ) & 0x0FFF;
}

bool CLCD::CommandFifo::Cmd_Is_Idle() {
  return getRegCmdRead() == getRegCmdWrite();
}

void CLCD::CommandFifo::Cmd_Wait_Until_Idle() {
  #if defined(UI_FRAMEWORK_DEBUG)
    const uint32_t startTime = millis();
  #endif
  do {
    #if defined(UI_FRAMEWORK_DEBUG)
      if(millis() - startTime > 3) {
        #if defined (SERIAL_PROTOCOLLNPGM)
          SERIAL_PROTOCOLLNPGM("Timeout on CommandFifo::Wait_Until_Idle()");
        #else
          Serial.println(F("Timeout on CommandFifo::Wait_Until_Idle()"));
        #endif
        break;
      }
    #endif
  } while(!Cmd_Is_Idle());
}

#if defined(LCD_IS_FT800)
void CLCD::CommandFifo::Cmd_Start() {
  if(command_write_ptr == 0xFFFFFFFFul) {
    command_write_ptr = getRegCmdWrite();
  }
}

void CLCD::CommandFifo::Cmd_Execute() {
  if(command_write_ptr != 0xFFFFFFFFul) {
    Mem_Write32(REG_CMD_WRITE, command_write_ptr);
  }
}

void CLCD::CommandFifo::Cmd_Reset() {
  Mem_Write32(REG_CMD_WRITE, 0x00000000);
  Mem_Write32(REG_CMD_READ,  0x00000000);
  command_write_ptr = 0xFFFFFFFFul;
};

template <class T> void CLCD::CommandFifo::_write_unaligned(T data, uint16_t len) {
  const char *ptr = (const char*)data;
  uint32_t bytes_tail, bytes_head;
  uint32_t command_read_ptr;

  #if defined(UI_FRAMEWORK_DEBUG)
  if(command_write_ptr == 0xFFFFFFFFul) {
    #if defined (SERIAL_PROTOCOLLNPGM)
      SERIAL_PROTOCOLLNPGM("Attempt to write to FIFO before CommandFifo::Cmd_Start().");
    #else
      Serial.println(F("Attempt to write to FIFO before CommandFifo::Cmd_Start()."));
    #endif
  }
  #endif

  /* Wait until there is enough space in the circular buffer for the transfer */
  do {
    command_read_ptr = getRegCmdRead();
    if (command_read_ptr <= command_write_ptr) {
      bytes_tail = 4096U - command_write_ptr;
      bytes_head = command_read_ptr;
    } else {
      bytes_tail = command_read_ptr - command_write_ptr;
      bytes_head = 0;
    }
  } while((bytes_tail + bytes_head) < len);

  /* Write as many bytes as possible following REG_CMD_WRITE */
  uint16_t bytes_to_write = min(len, bytes_tail);
  Mem_Write_Bulk (RAM_CMD + command_write_ptr, T(ptr), bytes_to_write);
  command_write_ptr += bytes_to_write;
  ptr  += bytes_to_write;
  len  -= bytes_to_write;

  if(len > 0) {
    /* Write remaining bytes at start of circular buffer */
    Mem_Write_Bulk (RAM_CMD, T(ptr), len);
    command_write_ptr = len;
  }

  if(command_write_ptr == 4096U) {
    command_write_ptr = 0;
  }
}

// Writes len bytes into the FIFO, if len is not
// divisible by four, zero bytes will be written
// to align to the boundary.

template <class T> void CLCD::CommandFifo::write(T data, uint16_t len) {
  const uint8_t padding = MULTIPLE_OF_4(len) - len;

  uint8_t pad_bytes[] = {0, 0, 0, 0};
  _write_unaligned(data,      len);
  _write_unaligned(pad_bytes, padding);
}
#else
uint32_t CLCD::CommandFifo::getRegCmdBSpace() {
  return Mem_Read32(REG_CMDB_SPACE)  & 0x0FFF;
}

void CLCD::CommandFifo::Cmd_Start() {
}

void CLCD::CommandFifo::Cmd_Execute() {
}

void CLCD::CommandFifo::Cmd_Reset() {
  Mem_Write32(REG_CMD_WRITE, 0x00000000);
  Mem_Write32(REG_CMD_READ,  0x00000000);
};

// Writes len bytes into the FIFO, if len is not
// divisible by four, zero bytes will be written
// to align to the boundary.

template <class T> void CLCD::CommandFifo::write(T data, uint16_t len) {
  const uint8_t padding = MULTIPLE_OF_4(len) - len;

  // The FT810 provides a special register that can be used
  // for writing data without us having to do our own FIFO
  // management.
  uint32_t Command_Space = getRegCmdBSpace();
  while(Command_Space < len + padding) {
    Command_Space = getRegCmdBSpace();
  }
  Mem_Write_Bulk(REG_CMDB_WRITE, data, len, padding);
}
#endif

// CO_PROCESSOR COMMANDS

void CLCD::CommandFifo::Cmd (uint32_t cmd32) {                                        // Writes a 32-bit (4 Bytes) Value to the Co-processor Command Buffer FIFO
  write(&cmd32, sizeof(uint32_t));
}

void CLCD::CommandFifo::Cmd (void* data, uint16_t len) {                              // Writes a data structure - always a multiple of 32 bits - to the Co_Processor FIFO.                                                                   // Data structure includes the 32-bit Co_Processor command.
  write(data, len);
}


void CLCD::CommandFifo::Cmd_Str (const char * const data) {
  write(data, strlen(data)+1);
}

void CLCD::CommandFifo::Cmd_Str (progmem_str data) {
  write(data, strlen_P((const char*)data)+1);
}

#endif // _AO_FT810_SPI_H





