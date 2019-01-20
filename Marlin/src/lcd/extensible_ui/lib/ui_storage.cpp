/******************
 * ui_storage.cpp *
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

#include "../../../sd/SdFile.h"
#include "../../../sd/cardreader.h"

#include "ui.h"

#include "ftdi_eve_constants.h"
#include "ftdi_eve_functions.h"
#include "ftdi_eve_spi.h"

#include "ui_filereader.h"
#include "ui_storage.h"

#if ENABLED(EXTENSIBLE_UI)

using namespace FTDI::SPI;
using namespace FTDI::SPI::most_significant_byte_first;

bool UIFlashStorage::is_present = false;

#ifdef SPI_FLASH_SS
/************************** SPI Flash Chip Interface **************************/

  void SPIFlash::wait_while_busy() {
    uint8_t status;
    safe_delay(1);
    do {
     spi_flash_select();
     spi_write_8(READ_STATUS_1);
     status = spi_read_8();
     spi_flash_deselect();
     safe_delay(1);
    } while(status & 1);
  }

  void SPIFlash::erase_sector_4k(uint32_t addr) {
    spi_flash_select();
    spi_write_8(WRITE_ENABLE);
    spi_flash_deselect();

    spi_flash_select();
    spi_write_8(ERASE_4K);
    spi_write_24(addr);
    spi_flash_deselect();

    wait_while_busy();
  }

  void SPIFlash::erase_sector_64k(uint32_t addr) {
    spi_flash_select();
    spi_write_8(WRITE_ENABLE);
    spi_flash_deselect();

    spi_flash_select();
    spi_write_8(ERASE_64K);
    spi_write_24(addr);
    spi_flash_deselect();

    wait_while_busy();
  }

  void SPIFlash::spi_write_begin(uint32_t addr) {
    spi_flash_select();
    spi_write_8(WRITE_ENABLE);
    spi_flash_deselect();

    spi_flash_select();
    spi_write_8(PAGE_PROGRAM);
    spi_write_24(addr);
  }

  void SPIFlash::spi_write_end() {
    spi_flash_deselect();
    wait_while_busy();
  }

  void SPIFlash::spi_read_begin(uint32_t addr) {
    spi_flash_select();
    spi_write_8(READ_DATA);
    spi_write_24(addr);
  }

  void SPIFlash::spi_read_end() {
    spi_flash_deselect();
  }

  void SPIFlash::erase_chip() {
    spi_flash_select();
    spi_write_8(WRITE_ENABLE);
    spi_flash_deselect();

    spi_flash_select();
    spi_write_8(ERASE_CHIP);
    spi_flash_deselect();
    wait_while_busy();
  }

  void SPIFlash::read_jedec_id(uint8_t &manufacturer_id, uint8_t &device_type, uint8_t &capacity) {
    spi_flash_select();
    spi_write_8(READ_JEDEC_ID);
    manufacturer_id = spi_recv();
    device_type     = spi_recv();
    capacity        = spi_recv();
    spi_flash_deselect ();
  }

  /* This function writes "size" bytes from "data" starting at addr, while properly
   * taking into account the special case of writing across a 256 byte page boundary.
   * Returns the addr directly after the write.
   */
  uint32_t SPIFlash::write(uint32_t addr, const void *_data, size_t size) {
    const uint8_t *data = (const uint8_t*) _data;
    while(size) {
      const uint32_t page_start = addr & 0xFFFF00ul;
      const uint32_t page_end   = page_start + 256;
      const uint32_t write_size = min(page_end - addr, size);
      spi_write_begin(addr);
      spi_write_bulk<ram_write>(data, write_size);
      spi_write_end();
      addr += write_size;
      size -= write_size;
      data += write_size;
    }
    return addr;
  }

  uint32_t SPIFlash::read(uint32_t addr, void *data, size_t size) {
    spi_read_begin(addr);
    spi_read_bulk(data, size);
    spi_read_end();
    return addr + size;
  }

  /******************************* UI STORAGE ROUTINES ******************************/

  bool UIFlashStorage::check_known_device() {
    uint8_t manufacturer_id, device_type, capacity;
    read_jedec_id(manufacturer_id, device_type, capacity);

    const bool is_known =
        ((manufacturer_id == 0xEF) && (device_type == 0x40) && (capacity == 0x15)) || // unknown
        ((manufacturer_id == 0x01) && (device_type == 0x40) && (capacity == 0x15)) || // Cypress S25FL116K
        ((manufacturer_id == 0xEF) && (device_type == 0x14) && (capacity == 0x15)) || // Winbond W25Q16JV
        ((manufacturer_id == 0x1F) && (device_type == 0x86) && (capacity == 0x01)) ;  // Adesto AT255F161

    if(!is_known) {
      SERIAL_ECHO_START(); SERIAL_ECHOLNPGM("Unable to locate supported SPI Flash Memory.");
      SERIAL_ECHO_START(); SERIAL_ECHOLNPAIR("  Manufacturer ID, got: ", manufacturer_id);
      SERIAL_ECHO_START(); SERIAL_ECHOLNPAIR("  Device Type    , got: ", device_type);
      SERIAL_ECHO_START(); SERIAL_ECHOLNPAIR("  Capacity       , got: ", capacity);
    }

    return is_known;
  }

  void UIFlashStorage::initialize() {
    for(uint8_t i = 0; i < 10; i++) {
      if(check_known_device()) {
        is_present = true;
        break;
      }
      safe_delay(1000);
    }
  }

  /**************************** DATA STORAGE AREA (first 4K or 64k) ********************/

  #if defined(DATA_STORAGE_SIZE_64K)
    constexpr uint32_t data_storage_area_size = 64 * 1024;
  #else
    constexpr uint32_t data_storage_area_size = 4 * 1024;
  #endif

  constexpr uint32_t data_addr = 0;

  /* In order to provide some degree of wear leveling, each data write to the
   * SPI Flash chip is appended to data that was already written before, until
   * the data storage area is completely filled. New data is written preceeded
   * with a 32-bit delimiter 'LULZ', so that we can distinguish written and
   * unwritten data:
   *
   *        'LULZ'         <--- 1st record delimiter
   *        <data_byte>
   *        <data_byte>
   *        <data_byte>
   *        'LULZ'         <--- 2nd record delimiter
   *        <data_byte>
   *        <data_byte>
   *        <data_byte>
   *           ...
   *        'LULZ'         <--- Last record delimiter
   *        <data_byte>
   *        <data_byte>
   *        <data_byte>
   *        0xFF           <--- Start of free space
   *        0xFF
   *           ...
   *
   * This function walks down the data storage area, verifying that the
   * delimiters are either 'LULZ' or 0xFFFFFFFF. In the case that an invalid
   * delimiter is found, this function returns -1, indicating that the Flash
   * data is invalid (this will happen if the block_size changed with respect
   * to earlier firmware). Otherwise, it returns the offset of the last
   * valid delimiter 'LULZ', indicating the most recently written data.
   */
  int32_t UIFlashStorage::get_config_read_offset(uint32_t block_size) {
    uint16_t stride = 4 + block_size;
    int32_t read_offset = -1;

    for(uint32_t offset = 0; offset < (data_storage_area_size - stride); offset += stride) {
      uint32_t delim;
      spi_read_begin(offset);
      spi_read_bulk (&delim, sizeof(delim));
      spi_read_end();
      switch(delim) {
        case 0xFFFFFFFFul: return read_offset;
        case delimiter:    read_offset = offset; break;
        default:
          SERIAL_ECHO_START(); SERIAL_ECHOLNPAIR("Invalid delimiter in Flash: ", delim);
          return -1;
      }
    }
    SERIAL_ECHO_START(); SERIAL_ECHOLNPGM("No LULZ delimiter found.");
    return -1;
  }

  /* This function returns the offset at which new data should be
   * appended, or -1 if the Flash needs to be erased */
  int32_t UIFlashStorage::get_config_write_offset(uint32_t block_size) {
    int32_t read_offset = get_config_read_offset(block_size);
    if(read_offset == -1) return -1; // The SPI flash is invalid

    int32_t write_offset = read_offset + 4 + block_size;
    if((write_offset + 4 + block_size) > data_storage_area_size) {
      SERIAL_ECHO_START(); SERIAL_ECHOLNPGM("Not enough free space in Flash.");
      return -1; // Not enough free space
    }
    return write_offset;
  }

  bool UIFlashStorage::verify_config_data(const void *data, size_t size) {
    if(!is_present) return false;

    int32_t read_addr = get_config_read_offset(size);
    if(read_addr == -1) return false;

    uint32_t delim;
    spi_read_begin(read_addr);
    spi_read_bulk (&delim, sizeof(delim));
    bool ok = spi_verify_bulk(data,size);
    spi_read_end();
    return ok && delim == delimiter;
  }

  bool UIFlashStorage::read_config_data(void *data, size_t size) {
    if(!is_present) return false;

    int32_t read_addr = get_config_read_offset(size);
    if(read_addr == -1) return false;

    uint32_t delim;
    spi_read_begin(read_addr);
    spi_read_bulk (&delim, sizeof(delim));
    spi_read_bulk (data, size);
    spi_read_end();
    return delim == delimiter;
  }

  void UIFlashStorage::write_config_data(const void *data, size_t size) {
    if(!is_present) {
      SERIAL_ECHO_START(); SERIAL_ECHOLNPGM("SPI Flash chip not present. Not saving UI settings.");
      return;
    }

    // Since Flash storage has a limited number of write cycles,
    // make sure that the data is different before rewriting.

    if(verify_config_data(data, size)) {
      SERIAL_ECHO_START(); SERIAL_ECHOLNPGM("UI settings already written, skipping write.");
      return;
    }

    int16_t write_addr = get_config_write_offset(size);
    if(write_addr == -1) {
      SERIAL_ECHO_START();
      SERIAL_ECHOPGM("Erasing UI settings from SPI Flash... ");
      #if defined(DATA_STORAGE_SIZE_64K)
        erase_sector_64k(0);
      #else
        erase_sector_4k(0);
      #endif
      write_addr = 0;
      SERIAL_ECHOLNPGM("DONE");
    }

    SERIAL_ECHO_START();
    SERIAL_ECHOPAIR("Writing UI settings to SPI Flash (offset ", write_addr);
    SERIAL_ECHOPGM(")...");

    const uint32_t delim = delimiter;
    write_addr = write(write_addr, &delim, sizeof(delim));
    write_addr = write(write_addr, data, size);

    SERIAL_ECHOLNPGM("DONE");
  }

  /**************************** MEDIA STORAGE AREA *****************************/

  /* The media storage area follows the data storage area. It consists
   * of a file index followed by the data for one or more media files.
   *
   * The file index consists of an array of 32-bit file sizes. If a file
   * is not present, the file's size will be set to 0xFFFFFFFF
   */

  constexpr uint32_t media_storage_addr    = data_storage_area_size;
  constexpr uint8_t  media_storage_slots   = 4;

  void UIFlashStorage::erase_chip() {
    SERIAL_ECHO_START(); SERIAL_ECHOPGM("Erasing SPI Flash...");
    SPIFlash::erase_chip();
    SERIAL_ECHOLNPGM("DONE");
  }

  uint32_t UIFlashStorage::get_media_file_start(uint8_t slot) {
    uint32_t addr = media_storage_addr + sizeof(uint32_t) * media_storage_slots;
    spi_read_begin(media_storage_addr);
    for(uint8_t i = 0; i < slot; i++) {
      addr += spi_read_32();
    }
    spi_read_end();
    return addr;
  }

  void UIFlashStorage::set_media_file_size(uint8_t slot, uint32_t size) {
    spi_write_begin(media_storage_addr + sizeof(uint32_t) * slot);
    spi_write_32(size);
    spi_write_end();
  }

  uint32_t UIFlashStorage::get_media_file_size(uint8_t slot) {
    spi_read_begin(media_storage_addr + sizeof(uint32_t) * slot);
    uint32_t size = spi_read_32();
    spi_read_end();
    return size;
  }

  /* Writes a media file into a slot. Media files must be written sequentially following
   * by a chip erase and it is not possible to overwrite files. */
  UIFlashStorage::error_t UIFlashStorage::write_media_file(progmem_str filename, uint8_t slot) {
    uint32_t addr;
    uint8_t buff[write_page_size];

    strcpy_P( (char*) buff, (const char*) filename);

    MediaFileReader reader;
    if(!reader.open((char*) buff)) {
      SERIAL_ECHO_START(); SERIAL_ECHOLNPGM("Unable to find media file");
      return FILE_NOT_FOUND;
    }

    if(get_media_file_size(slot) != 0xFFFFFFFFUL) {
      SERIAL_ECHO_START(); SERIAL_ECHOLNPGM("Media file already exists");
      return WOULD_OVERWRITE;
    }

    SERIAL_ECHO_START(); SERIAL_ECHOPGM("Writing SPI Flash...");

    set_media_file_size(slot, reader.size());
    addr = get_media_file_start(slot);

    // Write out the file itself
    for(;;) {
      const int16_t nBytes = reader.read(buff, write_page_size);
      if(nBytes == -1) {
        SERIAL_ECHOLNPGM("Failed to read from file");
        return READ_ERROR;
      }

      addr = write(addr, buff, nBytes);
      if(nBytes != write_page_size)
        break;

      #if ENABLED(EXTENSIBLE_UI)
        ExtUI::yield();
      #endif
    }

    SERIAL_ECHOLNPGM("DONE");

    SERIAL_ECHO_START(); SERIAL_ECHOPGM("Verifying SPI Flash...");

    bool verifyOk = true;

    // Verify the file index

    if(get_media_file_start(slot+1) != (get_media_file_start(slot) + reader.size())) {
      SERIAL_ECHOLNPGM("File index verification failed. ");
      verifyOk = false;
    }

    // Verify the file itself
    addr = get_media_file_start(slot);
    reader.rewind();

    while(verifyOk) {
      const int16_t nBytes = reader.read(buff, write_page_size);
      if(nBytes == -1) {
        SERIAL_ECHOPGM("Failed to read from file");
        verifyOk = false;
        break;
      }

      spi_read_begin(addr);
      if(!spi_verify_bulk(buff, nBytes)) {
        verifyOk = false;
        spi_read_end();
        break;
      }
      spi_read_end();

      addr += nBytes;
      if(nBytes != write_page_size) break;
      #if ENABLED(EXTENSIBLE_UI)
        ExtUI::yield();
      #endif
    };

    if(verifyOk) {
      SERIAL_ECHOLNPGM("DONE");
      return SUCCESS;
    } else {
      SERIAL_ECHOLNPGM("FAIL");
      return VERIFY_ERROR;
    }
  }

  bool UIFlashStorage::BootMediaReader::isAvailable(uint32_t slot) {
    bytes_remaining = get_media_file_size(0);
    if(bytes_remaining != 0xFFFFFFFFUL) {
      SERIAL_ECHO_START(); SERIAL_ECHOLNPAIR("Boot media file size:", bytes_remaining);
      addr = get_media_file_start(0);
      return true;
    } else {
      return false;
    }
  }

  int16_t UIFlashStorage::BootMediaReader::read(void *data, const size_t size) {
    if(bytes_remaining == 0xFFFFFFFFUL) return -1;

    if(size > bytes_remaining)
      return read(data, bytes_remaining);

    if(size > 0) {
      spi_read_begin(addr);
      spi_read_bulk(data, size);
      spi_read_end();
      addr += size;
      bytes_remaining -= size;
    }

    return size;
  }

  int16_t UIFlashStorage::BootMediaReader::read(void *obj, void *data, const size_t size) {
    return reinterpret_cast<UIFlashStorage::BootMediaReader*>(obj)->read(data, size);
  }

#else
  void UIFlashStorage::initialize()                                           {}
  void UIFlashStorage::write_config_data(const void *data, size_t size)       {}
  bool UIFlashStorage::verify_config_data(const void *, size_t)               {return false;}
  bool UIFlashStorage::read_config_data(void *data, size_t size)              {return false;}
  UIFlashStorage::error_t UIFlashStorage::write_media_file(progmem_str filename, uint8_t slot) {return FILE_NOT_FOUND;}
  void UIFlashStorage::erase_chip()                                           {}

  bool UIFlashStorage::BootMediaReader::isAvailable(uint32_t slot)            {return false;}
  int16_t UIFlashStorage::BootMediaReader::read(void *, const size_t)         {return -1;}
  int16_t UIFlashStorage::BootMediaReader::read(void *, void *, const size_t) {return -1;}
#endif // SPI_FLASH_SS
#endif // EXTENSIBLE_UI