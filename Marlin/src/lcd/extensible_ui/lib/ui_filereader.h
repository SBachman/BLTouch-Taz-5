/****************
 * ui_filereader.h *
 ****************/

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

#ifndef _UI_FILEREADER_
#define _UI_FILEREADER_

#include "../../../sd/SdFile.h"
#include "../../../sd/cardreader.h"

class MediaFileReader {
  private:
    Sd2Card  card;
    SdVolume volume;
    SdFile   root, file;

  public:
    bool open(const char* filename);
    int16_t read(void *buff, size_t bytes);
    uint32_t size();
    void rewind();
    void close();

    static int16_t read(void *obj, void *buff, size_t bytes);
};

#endif // _UI_FILEREADER_