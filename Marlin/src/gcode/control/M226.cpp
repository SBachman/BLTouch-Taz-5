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

#include "../gcode.h"
#include "../../Marlin.h" // for pin_is_protected and idle()
#include "../../module/stepper.h"

#if defined(LULZBOT_M226_PINS_WORKAROUND)
 #include "../../module/endstops.h"
 bool _digitalRead(const int pin_number) {
   LULZBOT_ENABLE_PROBE_PINS(true);
   delayMicroseconds(10);
   bool val;
   switch(pin_number) {
    #if HAS_Z_MIN
     case Z_MIN_PIN: val = READ(Z_MIN_PIN) != Z_MIN_ENDSTOP_INVERTING; break;
    #endif
    #if HAS_Z_MIN_PROBE_PIN
     case Z_MIN_PROBE_PIN: val = READ(Z_MIN_PROBE_PIN) != Z_MIN_PROBE_ENDSTOP_INVERTING;  break;
    #endif
    default: val = digitalRead(pin_number); break;
   }
   LULZBOT_ENABLE_PROBE_PINS(false);
   return val;
 }
 #define digitalRead _digitalRead
#endif

/**
 * M226: Wait until the specified pin reaches the state required (M226 P<pin> S<state>)
 */
void GcodeSuite::M226() {
  if (parser.seen('P')) {
    const int pin_number = PARSED_PIN_INDEX('P', 0),
              pin_state = parser.intval('S', -1); // required pin state - default is inverted
    const pin_t pin = GET_PIN_MAP_PIN(pin_number);

    if (WITHIN(pin_state, -1, 1) && pin > -1) {
      #if !defined(LULZBOT_NO_PIN_PROTECTION_ON_M226)
      if (pin_is_protected(pin))
        protected_pin_err();
      else
      #endif
      {
        int target = LOW;
        planner.synchronize();
        #if !defined(LULZBOT_NO_PIN_PROTECTION_ON_M226)
        // Don't switch pin mode. Since we are disabling protection,
        // we should only poll pins that already are inputs.
        pinMode(pin, INPUT);
        #endif
        switch (pin_state) {
          case 1: target = HIGH; break;
          case 0: target = LOW; break;
          case -1: target = !digitalRead(pin); break;
        }
        while (digitalRead(pin) != target) idle();
      }
    } // pin_state -1 0 1 && pin > -1
  } // parser.seen('P')
}
