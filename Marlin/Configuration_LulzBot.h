#ifndef CONFIGURATION_LULZBOT
#define CONFIGURATION_LULZBOT

/*

   If compiling from the command line:

      build-all-taz-toolheads.sh

   Else, if compiling using Arduino IDE, edit the "#define" lines below.

   Printer Model Choices:
      Gladiola_Mini                     // Lulzbot Mini (Gladiola)
      Gladiola_MiniLCD                  // Lulzbot Mini (Gladiola w/ LCD)
      Hibiscus_Mini2                    // Lulzbot Mini 2 (Hibiscus)
      Juniper_TAZ5                      // Lulzbot TAZ 5 (Juniper, Juniperberry, Juniperbush)
      Oliveoil_TAZ6                     // Lulzbot TAZ 6 (Oliveoil)
      Quiver_TAZ7                       // Lulzbot TAZ 7 (Quiver)

   Mini Toolhead Choices:
      Gladiola_SingleExtruder           // Standard Single Extruder (Gladiola)
      Albatross_Flexystruder            // Flexystruder (Albatross)
      Finch_Aerostruder                 // Aerostruder v1 (Finch)

   TAZ Toolhead Choices:
      Tilapia_SingleExtruder            // Standard Single Extruder (Tilapia)
      Kanyu_Flexystruder                // Flexystruder (Kanyu)
      Opah_Moarstruder                  // Moarstruder (Opah)
      Javelin_DualExtruderV2            // Dual Extruder v2 (Javelin)
      Longfin_FlexyDually               // FlexyDually (Longfin)
      Yellowfin_DualExtruderV3          // Dual Extruder v3 (Yellowfin)
      Angelfish_Aerostruder             // Aerostruder v1 (Angelfish)

   Universal Toolheads (Mini/Mini 2/TAZ):
      CecropiaSilk_SingleExtruderAeroV2 // SE  | 0.5 mm (Aero v2)
      AchemonSphinx_SmallLayer          // SL  | 0.25 mm (Micro)
      BandedTiger_HardenedSteel         // HS  | 0.8 mm
      DingyCutworm_HardenedSteelPlus    // HS+ | 1.2 mm

*/


// If compiling using the Arduino IDE, remove the "//" from the following lines and
// set printer model and toolhead after the "LULZBOT_" and "TOOLHEAD_" respectively:

#define LULZBOT_Juniper_TAZ5
#define TOOLHEAD_Tilapia_SingleExtruder

#endif /* CONFIGURATION_LULZBOT */