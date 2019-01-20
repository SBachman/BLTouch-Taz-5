# LulzBot Marlin firmware

This is the development branch of Marlin for LulzBot printers.

The source on this branch can compile firmware for the TAZ and Mini series, as well as all the current toolheads. This firmware also supports some internal R&D prototypes and toolheads.

# Safety and warnings:

**This repository may contain untested software.** It has not been extensively tested and may damage your printer and present other hazards. Use at your own risk. Do not operate your printer while unattended and be sure to power it off when leaving the room. Please consult the documentation that came with your printer for additional safety and warning information.

# Compilation from the command line on Linux using "avr-gcc"

Run the "build-lulzbot-firmware.sh" from the top level directory. This will build the ".hex" files for every printer and toolhead combination. The ".hex" files will be saved in the "build" subdirectory.

# Compilation using Arduino IDE

To select what firmware to build, modify the lines starting with "//#define" towards the bottom of the "Marlin/Configuration_LulzBot.h" file. Remove the leading "//" and modify the text after "LULZBOT_" and "TOOLHEAD_" so that it specifies the desired printer model or the desired toolhead, as listed in the top of the file.

For example, to compile for the Mini 2, modify the lines such that they read:

  #define LULZBOT_Hibiscus_Mini2
  #define TOOLHEAD_Finch_AerostruderV2

To compile for a TAZ using a standard toolhead, modify the lines such that they read:

  #define LULZBOT_Oliveoil_TAZ6
  #define TOOLHEAD_Tilapia_SingleExtruder

Then, open the "Marlin.ino" file from the "Marlin" subdirectory in the Arduino IDE. Select the board "Arduino/Genuino Mega or Mega 2560" from the "Board" submenu menu of the "Tools" menu and the port to which your printer is connected from the "Port" submenu from the "Tools" menu.

To compile and upload the firmware to your printer, select "Upload" from the "Sketch" menu.
