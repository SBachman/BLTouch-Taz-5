/*********************
 * FT810_Functions.h *
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

  /****************************************************************************
  *                       FUNCTION MAP                                        *
  *                                                                           *
  * SPI and FT800/810 Commands                                                *
  *                                                                           *
  * CLCD::spiSelect()                  Set CS line to 0                       *
  * CLCD::spiDeselect()                Set CS Line to 1                       *
  * CLCD::Reset()                      Toggle FT800/810 Power Down Line 50 ms *
  * CLCD::spiInit()                    Configure I/O Lines for SPI            *
  * CLCD::spiTransfer()                Send/Receive 1 SPI Byte                *
  * CLCD::Init()                       Set FT800/810 Registers                *
  * CLCD::Enable()                     Turn On FT800/810 PCLK                 *
  * CLCD::Disable()                    Turn Off FT8880/810 PCLK               *
  * CLCD::Set_Backlight()              Set LCD Backlight Level                *
  *                                                                           *
  * MEMORY READ FUNCTIONS                                                     *
  *                                                                           *
  * CLCD::Mem_ReadAddr()               Send 32-Bit Address                    *
  * CLCD::Mem_Read8()                  Read 1 Byte                            *
  * CLCD::Mem_Read16()                 Read 2 Bytes                           *
  * CLCD::Mem_Read32()                 Read 4 Bytes                           *
  *                                                                           *
  * MEMORY WRITE FUNCTIONS                                                    *
  *                                                                           *
  * CLCD::Mem_WriteAddr()              Send 24-Bit Address                    *
  * CLCD::Mem_Write8()                 Write 1 Byte                           *
  * CLCD::Mem_Write16()                Write 2 Bytes                          *
  * CLCD::Mem_Write32()                Write 4 Bytes                          *
  *                                                                           *
  * HOST COMMAND FUNCTION                                                     *
  *                                                                           *
  * CLCD::Host_Cmd()                   Send 24-Bit Host Command               *
  *                                                                           *
  * COMMAND BUFFER FUNCTIONS                                                  *
  *                                                                           *
  * CLCD::Cmd()                        Send 32-Bit Value(4 Bytes)CMD Buffer   *
  * CLCD::Cmd()                        Send Data Structure with 32-Bit Cmd    *
  * CLCD::Cmd_Str()                    Send Text String in 32-Bit Multiples   *

  *                                                                           *
  * FT800/810 GRAPHIC COMMANDS                                                *
  *                                                                           *
  * class CLCD:CommandFifo {}          Class to control Cmd FIFO              *

  * Fifo::Start()                      Wait for CP finish - Set FIFO Ptr      *
  * Fifo::Execute()                    Set REG_CMD_WRITE and start CP         *
  * Fifo::Reset()                      Set Cmd Buffer Pointers to 0           *
  *
  * Fifo::Cmd_Clear_Color()            Set Clear Screen Color                 *
  * Fifo::Cmd_Clear()                  Clear Color-Stencil-Tag Buffer(s)      *
  * Fifo::Cmd_Color()                  Set Current Color                      *
  * Fifo::Cmd_Set_Foreground_Color     Set Graphic Item Foreground Color      *
  * Fifo::Cmd_Set_Background_Color     Set Graphic Item Background Color      *
  * Fifo::Cmd_Bitmap_Source()          Set RAM Address of Bitmap Data         *
  * Fifo::Cmd_Bitmap_Layout()          Configure Bitmap Format and Layout     *
  * Fifo::Cmd_Bitmap_Size()            Set Bitmap Dimensions                  *
  * Fifo::Cmd_Bitmap_Handle()          Assign Handle ID to a Bitmap           *
  * Fifo::Cmd_Begin()                  Begin Drawing a Primative              *
  * Fifo::Cmd_Mem_Copy()               Copy a Block of Memory                 *
  * Fifo::Cmd_Append()                 Append Commands to Current DL          *
  * Fifo::Cmd_Gradient_Color()         Set 3D Button Highlight Color          *
  * Fifo::Cmd_Vertex_2II()             Set Coordinates of Graphics Primative  *
  * Fifo::Cmd_Draw_Button()            Draw Button with Bulk Write            *
  * Fifo::Cmd_Draw_Text()              Draw Text with Bulk Write              *
  *
  *
  */


 /**************************************************
  * RAM_G Graphics RAM Allocation                  *
  *                                                *
  * Address    Use                                 *
  *                                                *
  *    8000    Extruder Bitmap                     *
  *    8100    Bed Heat Bitmap                     *
  *    8200    Fan Bitmap                          *
  *    8300    Thumb Drive Symbol Bitmap           *
  *   35000    Static DL Space (FT800)             *
  *   F5000    Static DL Space (FT810)             *
  **************************************************/

#ifndef _AO_FT810_FUNC_H
#define _AO_FT810_FUNC_H

#define DL_CACHE_START RAM_G_SIZE - 0xFFFF

// Uncomment the following to disable the DL caching mechanism
// (note, this will keep the LCD status message from working)
//#define DL_CACHE_DISABLED

#if defined(LCD_800x480)
using namespace FTDI_LCD_800x480;
#elif defined(LCD_480x272)
using namespace FTDI_LCD_480x272;
#endif

using namespace FTDI;

typedef const __FlashStringHelper *progmem_str;

#define FORCEDINLINE __attribute__((always_inline))

class CLCD {
  private:
    static void spiInit (void);
    static void spiSelect (void);
    static void spiDeselect (void);
    static uint8_t spiTransfer (uint8_t spiOutByte);
    static void Mem_ReadAddr (uint32_t Reg_Address);
    static void Mem_WriteAddr (uint32_t Reg_Address);
    static void Mem_Read_Bulk (uint32_t Reg_Address, uint8_t *Data, uint16_t Len);
    static void Mem_Write_Bulk (uint32_t Reg_Address, const void *Data, uint16_t Len, uint8_t Padding = 0);
    static void Mem_Write_Bulk (uint32_t Reg_Address, progmem_str Str, uint16_t Len, uint8_t Padding = 0);

  public:
    static uint8_t Mem_Read8 (uint32_t Reg_Address);
    static uint16_t Mem_Read16 (uint32_t Reg_Address);
    static uint32_t Mem_Read32 (uint32_t Reg_Address);
    static void Mem_Write8 (uint32_t Reg_Address, uint8_t W_Data);
    static void Mem_Write16 (uint32_t Reg_Address, uint16_t W_Data);
    static void Mem_Write32 (uint32_t Reg_Address, uint32_t W_Data);

    static inline uint32_t pack_rgb(uint8_t r, uint8_t g, uint8_t b);

  public:
    typedef struct {
      const uint8_t  format;
      const uint16_t linestride;
      const uint8_t  filter;
      const uint8_t  wrapx;
      const uint8_t  wrapy;
      const uint32_t RAMG_addr;
      const uint16_t width;
      const uint16_t height;
    } bitmap_info_t;

    class CommandFifo;
    class SoundPlayer;
    class DLCache;

  public:
    static void Init (void);
    static void Turn_On_Backlight (void);
    static void Reset (void);
    static void Test_Pulse(void);
    static void Enable (void);
    static void Disable (void);
    static void Set_Backlight (uint16_t Freq, uint8_t Duty);
    static void Host_Cmd (unsigned char Host_Command, unsigned char Byte2);

    static void Get_Font_Metrics(uint8_t font, struct FontMetrics &fm);
    static void Flash_Write_RGB332_Bitmap(uint32_t Mem_Address, const unsigned char* pRGB332_Array, uint16_t Num_Bytes);

    static uint8_t Get_Tag() {return Mem_Read8(REG_TOUCH_TAG);}
};

/********************************* FT800/810 Commands *********************************/

#define MULTIPLE_OF_4(val) ((((val)+3)>>2)<<2)

void CLCD::Enable (void) {
  Mem_Write8(REG_PCLK, Pclk);
}

void CLCD::Disable (void) {
  Mem_Write8(REG_PCLK, 0x00);
}

void CLCD::Set_Backlight (uint16_t Freq, uint8_t Duty) {
  Mem_Write16(REG_PWM_HZ, Freq);
  Mem_Write8(REG_PWM_DUTY, Duty);
}

void CLCD::Turn_On_Backlight (void) {
  Set_Backlight(0x00FA, 128);
}

struct FontMetrics {
  uint8_t   char_widths[128];
  uint32_t  font_format;
  uint32_t  font_stride;
  uint32_t  font_width;
  uint32_t  font_height;
  uint32_t  font_ptr;
};

void CLCD::Get_Font_Metrics(uint8_t font, struct FontMetrics &fm) {
  Mem_Read_Bulk(ROM_FONT_ADDR + 148 * (font - 16), (uint8_t*) &fm, 148);
}

// HOST COMMAND FUNCTION

void CLCD::Host_Cmd (unsigned char Host_Command, unsigned char Byte2) {  // Sends 24-Bit Host Command to LCD
  if(Host_Command != ACTIVE) {
    Host_Command |= 0x40;
  }

  spiSelect();

  spiTransfer(Host_Command);                                       // Write Byte 1

  spiTransfer(Byte2);                                              // Write Byte 2

  spiTransfer(0x00);                                               // Write Byte 3

  spiDeselect();
}

void CLCD::Flash_Write_RGB332_Bitmap(uint32_t Mem_Address, const unsigned char* pRGB332_Array, uint16_t Num_Bytes)
{
  unsigned char Flash_Byte;

  for(unsigned int i = 0; i < Num_Bytes; i++) {
    Flash_Byte = pgm_read_byte_near(pRGB332_Array + i);

    Mem_Write8((Mem_Address + i), Flash_Byte);
  }
}

/******************* FT800/810 Graphic Commands *********************************/

class CLCD::CommandFifo {
  protected:
    static uint32_t getRegCmdWrite();
    static uint32_t getRegCmdRead();

    #if defined(LCD_IS_FT800)
      static uint32_t command_write_ptr;
      template <class T> void _write_unaligned(T data, uint16_t len);
    #else
      uint32_t getRegCmdBSpace();
    #endif
    void Cmd_Start(void);

  public:
    template <class T> void write(T data, uint16_t len);

  public:
    CommandFifo() {Cmd_Start();}

    static void Cmd_Reset (void);
    static bool Cmd_Is_Idle();
    static void Cmd_Wait_Until_Idle();

    void Cmd_Execute(void);

    void Cmd (uint32_t cmd32);
    void Cmd (void* data, uint16_t len);
    void Cmd_Str (const char * const data);
    void Cmd_Str (progmem_str data);
    void Cmd_Clear_Color (uint32_t rgb);
    void Cmd_Clear (bool Clr, bool Stl, bool Tag);
    void Cmd_Color (uint32_t rgb);
    void Cmd_Set_Foreground_Color (uint32_t rgb);
    void Cmd_Set_Background_Color (uint32_t rgb);
    void Cmd_Set_Tag (uint8_t Tag);
    void Cmd_Bitmap_Source (uint32_t RAM_G_Addr);

    // The following functions *must* be inlined since we are relying on the compiler to do
    // substitution of the constants from the data structure rather than actually storing
    // it in PROGMEM (which would fail, since we are not using pgm_read_near to read them).
    // Plus, by inlining, all the equations are evaluated at compile-time as everything
    // should be a constant.

    FORCEDINLINE void Cmd_Bitmap_Source (const bitmap_info_t& info) {Cmd_Bitmap_Source (info.RAMG_addr);};
    FORCEDINLINE void Cmd_Bitmap_Layout (const bitmap_info_t& info) {Cmd_Bitmap_Layout (info.format, info.linestride, info.height);};
    FORCEDINLINE void Cmd_Bitmap_Size(const bitmap_info_t& info)    {Cmd_Bitmap_Size (info.filter, info.wrapx, info.wrapy, info.width, info.height);}
    FORCEDINLINE void Cmd_Draw_Button_Icon(int16_t x, int16_t y, int16_t w, int16_t h, const bitmap_info_t& info, const float scale = 1) {
      Cmd_Begin(BEGIN_BITMAPS);
      if(scale != 1) {
        Cmd(BITMAP_TRANSFORM_A | uint32_t(float(256)/scale) & 0xFFFF);
        Cmd(BITMAP_TRANSFORM_E | uint32_t(float(256)/scale) & 0xFFFF);
      }
      Cmd_Bitmap_Size(info.filter, info.wrapx, info.wrapy, info.width*scale, info.height*scale);
      Cmd_Vertex_2F((x + w/2 - info.width*scale/2)*16, (y + h/2 - info.height*scale/2)*16);
      if(scale != 1) {
        Cmd(BITMAP_TRANSFORM_A | 256);
        Cmd(BITMAP_TRANSFORM_E | 256);
      }
    }
    template<typename T> FORCEDINLINE void Cmd_Draw_Button_Text(int16_t x, int16_t y, int16_t w, int16_t h, T text, int16_t font) {
      Cmd_Draw_Text(x + w/2, y + h/2, text, font, OPT_CENTER);
    }

    void Cmd_Bitmap_Layout (uint8_t format, uint16_t linestride, uint16_t height);
    void Cmd_Bitmap_Size(uint8_t filter, uint8_t wrapx, uint8_t wrapy, uint16_t width, uint16_t height);
    void Cmd_Bitmap_Handle (uint16_t Handle);
    void Cmd_Begin (uint32_t Primitive);
    void Cmd_Vertex_2F  (uint16_t X_Coord, uint16_t Y_Coord);
    void Cmd_Vertex_2II (uint16_t X_Coord, uint16_t Y_Coord, uint8_t B_Handle, uint8_t Cell);

    template<typename T> void Cmd_Draw_Button(int16_t x, int16_t y, int16_t w, int16_t h, T text, int16_t font,  uint16_t option);
    template<typename T> void Cmd_Draw_Text(int16_t x, int16_t y, T text, int16_t font,  uint16_t options);

    void Cmd_Draw_Clock (int16_t x, int16_t y, int16_t r, uint16_t option, int16_t h, int16_t m, int16_t s, int16_t ms);
    void Cmd_Draw_Progress_Bar (int16_t x, int16_t y, int16_t w, int16_t h, int16_t val, int16_t range);
    void Cmd_Draw_Slider (int16_t x, int16_t y, int16_t w, int16_t h, uint16_t options, uint16_t val, uint16_t range);
    void Cmd_Mem_Cpy (uint32_t dst, uint32_t src, uint32_t size);
    void Cmd_Append (uint32_t ptr, uint32_t size);
};

#if defined(LCD_IS_FT800)
uint32_t CLCD::CommandFifo::command_write_ptr = 0xFFFFFFFFul;
#endif

inline uint32_t CLCD::pack_rgb(uint8_t r, uint8_t g, uint8_t b) {
  return (uint32_t(r) << 16) | (uint32_t(g) << 8) | uint32_t(b);
}

void CLCD::CommandFifo::Cmd_Clear_Color (uint32_t rgb)   // DL Command - Set Clear Screen Color
{
  Cmd(CLEAR_COLOR_RGB | rgb);
}

void CLCD::CommandFifo::Cmd_Clear (bool Clr, bool Stl, bool Tag)             // DL Command - Clear Color-Stencil-Tag Buffer(s)
{
  Cmd( CLEAR |
    (Clr ? 0b00000001 : 0) |
    (Stl ? 0b00000010 : 0) |
    (Tag ? 0b00000100 : 0)
  );
}

void CLCD::CommandFifo::Cmd_Color (uint32_t rgb)         // DL Command - Set Current Color
{
  Cmd(COLOR_RGB | rgb);
}

void CLCD::CommandFifo::Cmd_Set_Foreground_Color (uint32_t rgb)   // Co-Processor Command - Set Foreground Color for Widgets
{
  Cmd(CMD_FGCOLOR);
  Cmd(rgb);
}

void CLCD::CommandFifo::Cmd_Set_Background_Color (uint32_t rgb)   // Co-Processor Command - Set Background Color for Widgets
{
  Cmd(CMD_BGCOLOR);
  Cmd(rgb);
}

void CLCD::CommandFifo::Cmd_Set_Tag (uint8_t Tag)
{
  Cmd(TAG | Tag);
}

void CLCD::CommandFifo::Cmd_Bitmap_Source (uint32_t RAM_G_Addr)
{
  Cmd(BITMAP_SOURCE | (RAM_G_Addr & 0x000FFFFF));
}

void CLCD::CommandFifo::Cmd_Bitmap_Layout (uint8_t format, uint16_t linestride, uint16_t height)
{
  Cmd( BITMAP_LAYOUT |
    (uint32_t(height)     << 0)  |
    (uint32_t(linestride) << 9)  |
    (uint32_t(format)     << 19)
  );
}

void CLCD::CommandFifo::Cmd_Bitmap_Size(uint8_t filter, uint8_t wrapx, uint8_t wrapy, uint16_t width, uint16_t height)
{
  Cmd( BITMAP_SIZE |
    (uint32_t(height)  << 0)  |
    (uint32_t(width)   << 9)  |
    (uint32_t(wrapy)   << 18) |
    (uint32_t(wrapx)   << 19) |
    (uint32_t(filter)  << 20)
  );
}

void CLCD::CommandFifo::Cmd_Bitmap_Handle (uint16_t Handle)
{
  Cmd( BITMAP_HANDLE | Handle);
}


void CLCD::CommandFifo::Cmd_Begin (uint32_t Primitive)
{
  Cmd(BEGIN + Primitive);
}

void CLCD::CommandFifo::Cmd_Vertex_2II (uint16_t X_Coord, uint16_t Y_Coord, uint8_t B_Handle, uint8_t Cell)
{
  Cmd( VERTEX2II |
    (uint32_t(Cell)     << 0)  |
    (uint32_t(B_Handle) << 7)  |
    (uint32_t(Y_Coord)  << 12) |
    (uint32_t(X_Coord)  << 21)
  );
}

void CLCD::CommandFifo::Cmd_Vertex_2F (uint16_t X_Coord, uint16_t Y_Coord)
{
  Cmd( VERTEX2F |
    (uint32_t(Y_Coord)  << 0) |
    (uint32_t(X_Coord)  << 15)
  );
}

template<typename T> void CLCD::CommandFifo::Cmd_Draw_Button(int16_t x, int16_t y, int16_t w, int16_t h, T text, int16_t font,  uint16_t option)
{
  struct {
    int32_t type = CMD_BUTTON;
    int16_t x;
    int16_t y;
    int16_t w;
    int16_t h;
    int16_t font;
    uint16_t option;
  } cmd_data;

  cmd_data.x      = x;
  cmd_data.y      = y;
  cmd_data.w      = w;
  cmd_data.h      = h;
  cmd_data.font   = font;
  cmd_data.option = option;

  Cmd( &cmd_data, sizeof(cmd_data) );
  Cmd_Str(text);
}

template<typename T> void CLCD::CommandFifo::Cmd_Draw_Text(int16_t x, int16_t y, T text, int16_t font,  uint16_t options)
{
  struct {
    int32_t type = CMD_TEXT;
    int16_t x;
    int16_t y;
    int16_t font;
    uint16_t options;
    } cmd_data;

  cmd_data.x       = x;
  cmd_data.y       = y;
  cmd_data.font    = font;
  cmd_data.options = options;

  Cmd( &cmd_data, sizeof(cmd_data) );
  Cmd_Str(text);
}

void CLCD::CommandFifo::Cmd_Draw_Clock (int16_t x, int16_t y, int16_t r, uint16_t option, int16_t h, int16_t m, int16_t s, int16_t ms)
{
  struct {
    int32_t type = CMD_CLOCK;
    int16_t x;
    int16_t y;
    int16_t r;
    uint16_t option;
    int16_t h;
    int16_t m;
    int16_t s;
    int16_t ms;
  } cmd_data;

  cmd_data.x      = x;
  cmd_data.y      = y;
  cmd_data.r      = r;
  cmd_data.option = option;
  cmd_data.h      = h;
  cmd_data.m      = m;
  cmd_data.s      = s;
  cmd_data.ms     = ms;

  Cmd( &cmd_data, sizeof(cmd_data) );
}

void CLCD::CommandFifo::Cmd_Draw_Progress_Bar (int16_t x, int16_t y, int16_t w, int16_t h, int16_t val, int16_t range)
{
  struct {
    int32_t  type = CMD_PROGRESS;
    int16_t  x;
    int16_t  y;
    int16_t  w;
    uint16_t h;
    uint16_t val;
    uint16_t range;
  } cmd_data;

  cmd_data.x      = x;
  cmd_data.y      = y;
  cmd_data.w      = w;
  cmd_data.h      = h;
  cmd_data.val    = val;
  cmd_data.range  = range;

  Cmd( &cmd_data, sizeof(cmd_data) );
}

void CLCD::CommandFifo::Cmd_Draw_Slider (int16_t x, int16_t y, int16_t w, int16_t h, uint16_t options, uint16_t val, uint16_t range)
{
  struct {
    int32_t type = CMD_SLIDER;
    int16_t x;
    int16_t y;
    int16_t w;
    int16_t h;
    uint16_t options;
    uint16_t val;
    uint16_t range;
  } cmd_data;

  cmd_data.x       = x;
  cmd_data.y       = y;
  cmd_data.w       = w;
  cmd_data.h       = h;
  cmd_data.options = options;
  cmd_data.val     = val;
  cmd_data.range   = range;

  Cmd( &cmd_data, sizeof(cmd_data) );
}

void CLCD::CommandFifo::Cmd_Mem_Cpy (uint32_t dst, uint32_t src, uint32_t size)
{
  struct {
    uint32_t  type = CMD_MEMCPY;
    uint32_t  dst;
    uint32_t  src;
    uint32_t  size;
  } cmd_data;

  cmd_data.dst    = dst;
  cmd_data.src    = src;
  cmd_data.size   = size;

  Cmd( &cmd_data, sizeof(cmd_data) );
}

void CLCD::CommandFifo::Cmd_Append (uint32_t ptr, uint32_t size)
{
  struct {
    uint32_t  type = CMD_APPEND;
    uint32_t  ptr;
    uint32_t  size;
  } cmd_data;

  cmd_data.ptr    = ptr;
  cmd_data.size   = size;

  Cmd( &cmd_data, sizeof(cmd_data) );
}

/******************* DISPLAY LIST CACHE MANAGEMENT ************************/

/* The Display List Cache mechanism stores the display list corresponding
 * to a menu into RAM_G so that on subsequent calls drawing the menu does
 * not require as much SPI traffic. Dynamic content, such as indicators,
 * should not be cached.
 *
 * The DLCache can be used like so:
 *
 *   void some_function() {
 *     CLCD::DLCache dlcache(UNIQUE_ID);
 *
 *     if(dlcache.hasData()) {
 *        dlcache.append();
 *     } else {
 *        // Add stuff to the DL
 *        dlcache.store();
 *     }
 *
 *
 * Layout of Cache memory:
 *
 * The cache memory begins with a table at
 * DL_CACHE_START: each table entry contains
 * an address and size for a cached DL slot.
 *
 * Immediately following the table is the
 * DL_FREE_ADDR, which points to free cache
 * space; following this is occupied DL space,
 * and after that free space that is yet to
 * be used.
 *
 *  location        data        sizeof
 *
 *  DL_CACHE_START  slot0_addr     4
 *                  slot0_size     4
 *                  slot1_addr     4
 *                  slot1_size     4
 *                      ...
 *                  slotN_addr     4
 *                  slotN_size     4
 *  DL_FREE_ADDR    dl_free_ptr    4
 *                  cached data
 *                      ...
 *  dl_free_ptr     empty space
 *                      ...
 */

class CLCD::DLCache {
  private:
    uint8_t  dl_slot;
    uint32_t dl_addr;
    uint16_t dl_size;

    void load_slot();
    static void save_slot(uint8_t dl_slot, uint32_t dl_addr, uint32_t dl_size);

  public:
    static void init();

    DLCache(uint8_t slot) {
      dl_slot = slot;
      load_slot();
    }

    bool hasData();
    bool store(uint32_t num_bytes = 0);
    void append();
};

#define DL_CACHE_SLOTS   250
#define DL_FREE_ADDR     DL_CACHE_START + DL_CACHE_SLOTS * 8

// The init function ensures all cache locatios are marked as empty

void CLCD::DLCache::init() {
  Mem_Write32(DL_FREE_ADDR, DL_FREE_ADDR + 4);
  for(uint8_t slot = 0; slot < DL_CACHE_SLOTS; slot++) {
    save_slot(slot, 0, 0);
  }
}

bool CLCD::DLCache::hasData() {
  #if !defined(DL_CACHE_DISABLED)
  return dl_size != 0;
  #else
  return false;
  #endif
}

/* This caches the current display list in RAMG so
 * that it can be appended later. The memory is
 * dynamically allocated following DL_FREE_ADDR.
 *
 * If num_bytes is provided, then that many bytes
 * will be reserved so that the cache may be re-written
 * later with potentially a bigger DL.
 */

bool CLCD::DLCache::store(uint32_t num_bytes /* = 0*/) {
  #if !defined(DL_CACHE_DISABLED)
  CLCD::CommandFifo cmd;

  // Execute any commands already in the FIFO
  cmd.Cmd_Execute();
  cmd.Cmd_Wait_Until_Idle();

  // Figure out how long the display list is
  uint32_t new_dl_size = Mem_Read32(REG_CMD_DL) & 0x1FFF;
  uint32_t free_space  = 0;
  uint32_t dl_alloc    = 0;

  if(dl_addr == 0) {
    // If we are allocating new space...
    dl_addr     = Mem_Read32(DL_FREE_ADDR);
    free_space  = RAM_G_SIZE - dl_addr;
    dl_alloc    = num_bytes ? num_bytes : new_dl_size;
    dl_size     = new_dl_size;
  } else {
    // Otherwise, we can only store as much space
    // as was previously allocated.
    free_space  = num_bytes ? num_bytes : dl_size;
    dl_alloc    = 0;
    dl_size     = new_dl_size;
  }

  if(dl_size > free_space) {
    // Not enough memory to cache the display list.
    #if defined(UI_FRAMEWORK_DEBUG)
      #if defined (SERIAL_PROTOCOLLNPAIR)
        SERIAL_PROTOCOLPAIR("Not enough space in GRAM to cache display list, free space: ", free_space);
        SERIAL_PROTOCOLLNPAIR(" Required: ", dl_size);
      #else
        Serial.print(F("Not enough space in GRAM to cache display list, free space:"));
        Serial.print(free_space);
        Serial.print(F(" Required: "));
        Serial.println(dl_size);
      #endif
    #endif
    return false;
  } else {
    #if defined(UI_FRAMEWORK_DEBUG)
      #if defined (SERIAL_PROTOCOLLNPAIR)
        SERIAL_PROTOCOLPAIR("Saving DL to RAMG cache, bytes: ", dl_size);
        SERIAL_PROTOCOLPAIR(" (Free space: ", free_space);
        SERIAL_PROTOCOLLNPGM(")");
      #else
        Serial.print(F("Saving DL to RAMG cache, bytes: "));
        Serial.println(dl_size);
        Serial.print(F(" (Free space: "));
        Serial.println(free_space);
        Serial.print(F(")"));
      #endif
    #endif
    cmd.Cmd_Mem_Cpy(dl_addr, RAM_DL, dl_size);
    cmd.Cmd_Execute();
    save_slot(dl_slot, dl_addr, dl_size);
    if(dl_alloc > 0) {
      // If we allocated space dynamically, then adjust dl_free_addr.
      Mem_Write32(DL_FREE_ADDR, dl_addr + dl_alloc);
    }
    return true;
  }
  #endif
}

void CLCD::DLCache::save_slot(uint8_t dl_slot, uint32_t dl_addr, uint32_t dl_size) {
  Mem_Write32(DL_CACHE_START + dl_slot * 8 + 0, dl_addr);
  Mem_Write32(DL_CACHE_START + dl_slot * 8 + 4, dl_size);
}

void CLCD::DLCache::load_slot() {
  dl_addr  = Mem_Read32(DL_CACHE_START + dl_slot * 8 + 0);
  dl_size  = Mem_Read32(DL_CACHE_START + dl_slot * 8 + 4);
}

void CLCD::DLCache::append() {
  CLCD::CommandFifo cmd;
  #if !defined(DL_CACHE_DISABLED)
  cmd.Cmd_Append(dl_addr, dl_size);
  #endif
  #if defined(UI_FRAMEWORK_DEBUG)
    cmd.Cmd_Execute();
    cmd.Cmd_Wait_Until_Idle();
    #if defined (SERIAL_PROTOCOLLNPAIR)
      SERIAL_PROTOCOLPAIR("Appending to DL from RAMG cache, bytes: ", dl_size);
      SERIAL_PROTOCOLPAIR(" (REG_CMD_DL: ", Mem_Read32(REG_CMD_DL));
      SERIAL_PROTOCOLLNPGM(")");
    #else
      Serial.print(F("Appending to DL from RAMG cache, bytes: "));
      Serial.print(dl_size);
      Serial.print(F(" (REG_CMD_DL: "));
      Serial.print(Mem_Read32(REG_CMD_DL));
      Serial.println(F(")"));
    #endif
  #endif
}

/******************* LCD INITIALIZATION ************************/

void CLCD::Init (void) {
  spiInit();                                  // Set Up I/O Lines for SPI and FT800/810 Control
  delay(50);

  Reset();                                    // Power Down the FT800/810 for 50 ms
  delay(50);

/*
 *  If driving the 4D Systems 4DLCD-FT843 Board, the following Init sequence is needed for its FT800 Driver
 */

#ifdef LCD_IS_FT800                                    // Use External Crystal and 48 MHz System Clock
  Host_Cmd(CLKEXT, 0);

  delay(20);
  Host_Cmd(CLK48M, 0);
#else
  Host_Cmd(CLKINT, 0);
  delay(20);
  Host_Cmd(CLKSEL, Clksel);                     // Use Internal RC Oscillator and 48 MHz System Clock
#endif

  delay(20);

  Host_Cmd(ACTIVE, 0);                        // Activate the System Clock
  delay(50);

  delay(400);
  uint8_t Device_ID = Mem_Read8(REG_ID);            // Read Device ID, Should Be 0x7C;
  #if defined(UI_FRAMEWORK_DEBUG)
  if(Device_ID != 0x7C) {
    #if defined (SERIAL_PROTOCOLLNPAIR)
      SERIAL_PROTOCOLLNPAIR("Incorrect device ID, should be 7C, got ", Device_ID);
    #else
      Serial.print(F("Incorrect device ID, should be 7C, got "));
      Serial.println(Device_ID, HEX);
    #endif
  } else {
    #if defined (SERIAL_PROTOCOLLNPGM)
      SERIAL_PROTOCOLLNPGM("Device is correct ");
    #else
      Serial.println(F("Device is correct "));
    #endif
  }
  #endif
  delay(400);

  Mem_Write8(REG_GPIO, 0x00);                 // Turn OFF Display Enable (GPIO Bit 7);
  Mem_Write8(REG_PCLK, 0x00);                 // Turn OFF LCD PCLK
  Set_Backlight(0x00FA, 0);

  /*
   *  Configure the FT800/810 Registers
   */

  Mem_Write16(REG_HCYCLE, Hcycle);
  Mem_Write16(REG_HOFFSET, Hoffset);
  Mem_Write16(REG_HSYNC0, Hsync0);
  Mem_Write16(REG_HSYNC1, Hsync1);
  Mem_Write16(REG_VCYCLE, Vcycle);
  Mem_Write16(REG_VOFFSET, Voffset);
  Mem_Write16(REG_VSYNC0, Vsync0);
  Mem_Write16(REG_VSYNC1, Vsync1);
  Mem_Write16(REG_HSIZE, Hsize);
  Mem_Write16(REG_VSIZE, Vsize);
  Mem_Write8(REG_SWIZZLE, Swizzle);
  Mem_Write8(REG_PCLK_POL, Pclkpol);
  Mem_Write8(REG_CSPREAD, 1);

  #if   defined(LCD_PORTRAIT)  &&  defined(LCD_UPSIDE_DOWN)
  Mem_Write8(REG_ROTATE, 3);
  #elif defined(LCD_PORTRAIT)  && !defined(LCD_UPSIDE_DOWN)
  Mem_Write8(REG_ROTATE, 2);
  #elif !defined(LCD_PORTRAIT) &&  defined(LCD_UPSIDE_DOWN)
  Mem_Write8(REG_ROTATE, 1);
  #else !defined(LCD_PORTRAIT) && !defined(LCD_UPSIDE_DOWN)
  Mem_Write8(REG_ROTATE, 0);
  #endif

  Mem_Write8(REG_TOUCH_MODE, 0x03);           // Configure the Touch Screen
  Mem_Write8(REG_TOUCH_ADC_MODE, 0x01);
  Mem_Write8(REG_TOUCH_OVERSAMPLE, 0x0F);
  Mem_Write16(REG_TOUCH_RZTHRESH, 5000);
  Mem_Write8(REG_VOL_SOUND, 0x00);            // Turn Synthesizer Volume Off
  Mem_Write8(REG_DLSWAP, 0x02);               // Swap on New Frame

  /*
   *  Turn on the Display         (set DISP high)
   *  Turn on the Audio Amplifier (set GP0 high; on the AO CLCD board, this is tied to the amplifier control)
   */
  #if defined(LCD_IS_FT800)
    Mem_Write8(REG_GPIO_DIR,   GPIO_DISP  | GPIO_GP0);
    Mem_Write8(REG_GPIO,       GPIO_DISP  | GPIO_GP0);
  #else
    Mem_Write16(REG_GPIOX_DIR, GPIOX_DISP | GPIOX_GP0);
    Mem_Write16(REG_GPIOX,     GPIOX_DISP | GPIOX_GP0);
  #endif

  Enable();                                   // Turns on Clock by setting PCLK Register to 5
  delay(50);

  CommandFifo::Cmd_Reset();
  delay(50);

  // Set Initial Values for Touch Transform Registers

  CLCD::Mem_Write32(REG_TOUCH_TRANSFORM_A, default_transform_a);
  CLCD::Mem_Write32(REG_TOUCH_TRANSFORM_B, default_transform_b);
  CLCD::Mem_Write32(REG_TOUCH_TRANSFORM_C, default_transform_c);
  CLCD::Mem_Write32(REG_TOUCH_TRANSFORM_D, default_transform_d);
  CLCD::Mem_Write32(REG_TOUCH_TRANSFORM_E, default_transform_e);
  CLCD::Mem_Write32(REG_TOUCH_TRANSFORM_F, default_transform_f);
}

/******************* SOUND HELPER CLASS ************************/

/* tiny_interval() downsamples a 32-bit millis() value
   into a 8-bit value which can record periods of
   up to 4.096 seconds with a rougly 16 millisecond
   resolution. This allows us to measure small
   intervals without needing to use four-byte counters.
 */
inline uint8_t tiny_interval(uint32_t ms) {
  return uint8_t(ms / 64);
}

class CLCD::SoundPlayer {
  public:
    struct sound_t {
      effect_t  effect;      // The sound effect number
      note_t    note;        // The MIDI note value
      uint16_t  sixteenths;  // Duration of note, in sixteeths of a second, or zero to play to completion
    };

    const uint8_t WAIT = 0;

  private:
    const sound_t *sequence;
    uint8_t       next;

  public:
    static void setVolume(uint8_t volume);
    static void play(effect_t effect, note_t note = NOTE_C4);
    static bool soundPlaying();

    void play(const sound_t* seq);

    void onIdle();

    bool hasMoreNotes() {return sequence != 0;};
};

void CLCD::SoundPlayer::setVolume(uint8_t vol) {
  CLCD::Mem_Write8(REG_VOL_SOUND, vol);
}

void CLCD::SoundPlayer::play(effect_t effect, note_t note) {
  CLCD::Mem_Write16(REG_SOUND, (note << 8) | effect);
  CLCD::Mem_Write8( REG_PLAY,  1);

  #if defined(UI_FRAMEWORK_DEBUG)
    #if defined (SERIAL_PROTOCOLLNPAIR)
      SERIAL_PROTOCOLPAIR("Playing note ", note);
      SERIAL_PROTOCOLLNPAIR(", instrument ", effect);
    #endif
  #endif
}

void CLCD::SoundPlayer::play(const sound_t* seq) {
  sequence = seq;
  next     = tiny_interval(millis()) + 1;
}

bool CLCD::SoundPlayer::soundPlaying() {
  return CLCD::Mem_Read8( REG_PLAY ) & 0x1;
}

void CLCD::SoundPlayer::onIdle() {
  if(!sequence) return;

  const uint8_t tiny_millis = tiny_interval(millis());
  const bool readyForNextNote = (next == WAIT) ? !soundPlaying() : (tiny_millis > next);

  if(readyForNextNote) {
    const effect_t fx = effect_t(pgm_read_byte_near(&sequence->effect));
    const note_t   nt =   note_t(pgm_read_byte_near(&sequence->note));
    const uint16_t ms = uint32_t(pgm_read_byte_near(&sequence->sixteenths)) * 1000 / 16;

    if(ms == 0 && fx == SILENCE && nt == 0) {
      sequence = 0;
    } else {
      #if defined(UI_FRAMEWORK_DEBUG)
        #if defined (SERIAL_PROTOCOLLNPAIR)
          SERIAL_PROTOCOLLNPAIR("Scheduling note in ", ms);
        #endif
      #endif
      next =   (ms == WAIT) ? 0       : (tiny_millis + tiny_interval(ms));
      play(fx, (nt == 0)    ? NOTE_C4 : nt);
      sequence++;
    }
  }
}

#endif // _AO_FT810_FUNC_H





