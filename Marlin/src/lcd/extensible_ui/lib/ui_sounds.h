/*****************
 * ui_sounds.cpp *
 *****************/

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

#ifndef _UI_SOUNDS_H_
#define _UI_SOUNDS_H_

/******************* TINY INTERVAL CLASS ***********************/

/* Helpful Reference:
 *
 *  https://arduino.stackexchange.com/questions/12587/how-can-i-handle-the-millis-rollover
 */

/* tiny_interval_t downsamples a 32-bit millis() value
   into a 8-bit value which can record periods of
   a few seconds with a rougly 1/16th of second
   resolution. This allows us to measure small
   intervals without needing to use four-byte counters.
 */
class tiny_time_t {
  private:
    friend class tiny_timer_t;
    uint8_t _duration;

    static uint8_t tiny_time(uint32_t ms) {return ceil(float(ms) / 64);};
    //static uint8_t tiny_time(uint32_t ms) {return ms >> 6;}; // This doesn't work.

  public:
    tiny_time_t()            : _duration(0) {}
    tiny_time_t(uint32_t ms) : _duration(tiny_time(ms)) {}
    tiny_time_t & operator=   (uint32_t ms) {_duration = tiny_time(ms); return *this;}
    bool          operator == (uint32_t ms) {return _duration == tiny_time(ms);}
};

class tiny_timer_t {
  private:
    uint8_t _start;

  public:
    void start();
    bool elapsed(tiny_time_t interval);
};

/******************* SOUND HELPER CLASS ************************/

namespace FTDI {
  typedef enum {
    PLAY_ASYNCHRONOUS,
    PLAY_SYNCHRONOUS
  } play_mode_t;

  class SoundPlayer {
    public:
      struct sound_t {
        effect_t  effect;      // The sound effect number
        note_t    note;        // The MIDI note value
        uint16_t  sixteenths;  // Duration of note, in sixteeths of a second, or zero to play to completion
      };

      const uint8_t WAIT = 0;

    private:
      const sound_t   *sequence;
      tiny_timer_t     timer;
      tiny_time_t      wait;

      note_t frequency_to_midi_note(const uint16_t frequency);

    public:
      static void set_volume(uint8_t volume);
      static uint8_t get_volume();

      static void play(effect_t effect, note_t note = NOTE_C4);
      static bool is_sound_playing();

      void play(const sound_t* seq, play_mode_t mode = PLAY_SYNCHRONOUS);
      void play_tone(const uint16_t frequency_hz, const uint16_t duration_ms);
      bool has_more_notes() {return sequence != 0;};

      void onIdle();
  };

  extern SoundPlayer sound;

  /* A sound sequence consists of an array of the following:

      struct sound_t {
        effect_t effect;      // The sound effect number
        note_t   note;        // The MIDI note value, or C4 if 0
        uint8_t  sixteenths;  // Note duration in 1/16th of a sec;
                              // If 0, play until sample is finished.
      };

     Constants are defined in "AO_FT810_Constants.h".

     Both note and sixteenths are optional. If omitted, the compiler
     will fill them in with 0 which will be interpreted by play C4
     for the duration of the sample.

     The sequence must be terminated by "{SILENCE, END_SONG, WAIT}", i.e
     all zeros.
   */

  const PROGMEM SoundPlayer::sound_t silence[] = {
    {SILENCE,      END_SONG, 0}
  };

  const PROGMEM SoundPlayer::sound_t chimes[] = {
    {CHIMES,       NOTE_G3,  5},
    {CHIMES,       NOTE_E4,  5},
    {CHIMES,       NOTE_C4,  5},
    {SILENCE,      END_SONG, 0}
  };

  const PROGMEM SoundPlayer::sound_t sad_trombone[] = {
    {TRUMPET,      NOTE_A3S, 10},
    {TRUMPET,      NOTE_A3 , 10},
    {TRUMPET,      NOTE_G3S, 10},
    {TRUMPET,      NOTE_G3,  20},
    {SILENCE,      END_SONG, 0}
  };

  const PROGMEM SoundPlayer::sound_t twinkle[] = {
    {GLOCKENSPIEL, NOTE_C4,  1},
    {GLOCKENSPIEL, NOTE_E4,  1},
    {GLOCKENSPIEL, NOTE_G4,  16},
    {SILENCE,      END_SONG, 0}
  };

  const PROGMEM SoundPlayer::sound_t fanfare[] = {
    {TRUMPET,      NOTE_A3,  4},
    {SILENCE,      REST,     1},
    {TRUMPET,      NOTE_A3,  2},
    {SILENCE,      REST,     1},
    {TRUMPET,      NOTE_A3,  2},
    {SILENCE,      REST,     1},
    {TRUMPET,      NOTE_E4,  10},
    {SILENCE,      END_SONG, 0}
  };

  const PROGMEM SoundPlayer::sound_t media_inserted[] = {
    {MUSIC_BOX,    NOTE_C4,  2},
    {MUSIC_BOX,    NOTE_E4,  2},
    {SILENCE,      END_SONG, 0}
  };

  const PROGMEM SoundPlayer::sound_t media_removed[] = {
    {MUSIC_BOX,    NOTE_E4,  2},
    {MUSIC_BOX,    NOTE_C4,  2},
    {SILENCE,      END_SONG, 0}
  };

  const PROGMEM SoundPlayer::sound_t js_bach_toccata[] = {
    {ORGAN,        NOTE_A4,  2},
    {ORGAN,        NOTE_G4,  2},
    {ORGAN,        NOTE_A4,  35},
    {SILENCE,      REST,     12},
    {ORGAN,        NOTE_G4,  4},
    {ORGAN,        NOTE_F4,  4},
    {ORGAN,        NOTE_E4,  4},
    {ORGAN,        NOTE_D4,  4},
    {ORGAN,        NOTE_C4S, 16},
    {ORGAN,        NOTE_D4,  32},
    {SILENCE,      REST,     42},

    {ORGAN,        NOTE_A3,  2},
    {ORGAN,        NOTE_G3,  2},
    {ORGAN,        NOTE_A3,  35},
    {SILENCE,      REST,     9},
    {ORGAN,        NOTE_E3,  8},
    {ORGAN,        NOTE_F3,  8},
    {ORGAN,        NOTE_C3S, 16},
    {ORGAN,        NOTE_D3,  27},
    {SILENCE,      REST,     42},

    {ORGAN,        NOTE_A2,  2},
    {ORGAN,        NOTE_G2,  2},
    {ORGAN,        NOTE_A2,  35},
    {SILENCE,      REST,     12},
    {ORGAN,        NOTE_G2,  4},
    {ORGAN,        NOTE_F2,  4},
    {ORGAN,        NOTE_E2,  4},
    {ORGAN,        NOTE_D2,  4},
    {ORGAN,        NOTE_C2S, 16},
    {ORGAN,        NOTE_D2,  32},
    {SILENCE,      REST,     52},

    //{ORGAN,        NOTE_D1,  28},
    {ORGAN,        NOTE_C3S, 9},
    {ORGAN,        NOTE_E3,  9},
    {ORGAN,        NOTE_G3,  9},
    {ORGAN,        NOTE_A3S, 9},
    {ORGAN,        NOTE_C4S, 9},
    {ORGAN,        NOTE_E4,  9},
    {ORGAN,        NOTE_D4,  20},
    {SILENCE,      REST,     30},

    {ORGAN,        NOTE_C4S, 4},
    {ORGAN,        NOTE_D4,  2},
    {ORGAN,        NOTE_E4,  2},

    {ORGAN,        NOTE_C4S, 2},
    {ORGAN,        NOTE_D4,  2},
    {ORGAN,        NOTE_E4,  2},

    {ORGAN,        NOTE_C4S, 2},
    {ORGAN,        NOTE_D4,  2},
    {ORGAN,        NOTE_E4,  2},

    {ORGAN,        NOTE_C4S, 2},
    {ORGAN,        NOTE_D4,  4},
    {ORGAN,        NOTE_E4,  4},
    {ORGAN,        NOTE_F4,  2},
    {ORGAN,        NOTE_G4,  2},

    {ORGAN,        NOTE_E4,  2},
    {ORGAN,        NOTE_F4,  2},
    {ORGAN,        NOTE_G4,  2},

    {ORGAN,        NOTE_E4,  2},
    {ORGAN,        NOTE_F4,  2},
    {ORGAN,        NOTE_G4,  2},

    {ORGAN,        NOTE_E4,  2},
    {ORGAN,        NOTE_F4,  4},
    {ORGAN,        NOTE_G4,  4},
    {ORGAN,        NOTE_A4,  2},
    {ORGAN,        NOTE_A4S, 2},

    {ORGAN,        NOTE_G4,  2},
    {ORGAN,        NOTE_A4,  2},
    {ORGAN,        NOTE_A4S, 2},

    {ORGAN,        NOTE_G4,  2},
    {ORGAN,        NOTE_A4,  2},
    {ORGAN,        NOTE_A4S, 2},

    {ORGAN,        NOTE_G4,  2},
    {ORGAN,        NOTE_A4,  4},
    {SILENCE,      REST,     36},


    {ORGAN,        NOTE_C5S, 4},
    {ORGAN,        NOTE_D5,  2},
    {ORGAN,        NOTE_E5,  2},

    {ORGAN,        NOTE_C5S, 2},
    {ORGAN,        NOTE_D5,  2},
    {ORGAN,        NOTE_E5,  2},

    {ORGAN,        NOTE_C5S, 2},
    {ORGAN,        NOTE_D5,  2},
    {ORGAN,        NOTE_E5,  2},

    {ORGAN,        NOTE_C5S, 2},
    {ORGAN,        NOTE_D5,  4},
    {ORGAN,        NOTE_E5,  4},
    {ORGAN,        NOTE_F5,  2},
    {ORGAN,        NOTE_G5,  2},

    {ORGAN,        NOTE_E5,  2},
    {ORGAN,        NOTE_F5,  2},
    {ORGAN,        NOTE_G5,  2},

    {ORGAN,        NOTE_E5,  2},
    {ORGAN,        NOTE_F5,  2},
    {ORGAN,        NOTE_G5,  2},

    {ORGAN,        NOTE_E5,  2},
    {ORGAN,        NOTE_F5,  4},
    {ORGAN,        NOTE_G5,  4},
    {ORGAN,        NOTE_A5,  2},
    {ORGAN,        NOTE_A5S, 2},

    {ORGAN,        NOTE_G5,  2},
    {ORGAN,        NOTE_A5,  2},
    {ORGAN,        NOTE_A5S, 2},

    {ORGAN,        NOTE_G5,  2},
    {ORGAN,        NOTE_A5,  2},
    {ORGAN,        NOTE_A5S, 2},

    {ORGAN,        NOTE_G5,  2},
    {ORGAN,        NOTE_A5,  4},
    {SILENCE,      REST,     32},

    {ORGAN,        NOTE_A5,  4},
    {ORGAN,        NOTE_G5,  2},
    {ORGAN,        NOTE_A5S, 2},

    {ORGAN,        NOTE_E5,  2},
    {ORGAN,        NOTE_G5,  2},
    {ORGAN,        NOTE_A5S, 2},

    {ORGAN,        NOTE_E5,  2},
    {ORGAN,        NOTE_F5,  2},
    {ORGAN,        NOTE_A5,  2},

    {ORGAN,        NOTE_D5,  2},
    {ORGAN,        NOTE_F5,  2},
    {ORGAN,        NOTE_G5,  2},

    {ORGAN,        NOTE_D5,  2},
    {ORGAN,        NOTE_E5,  2},
    {ORGAN,        NOTE_A5,  2},

    {ORGAN,        NOTE_C5,  2},
    {ORGAN,        NOTE_E5,  2},
    {ORGAN,        NOTE_A5,  2},

    {ORGAN,        NOTE_C5,  2},
    {ORGAN,        NOTE_D5,  2},
    {ORGAN,        NOTE_F5,  2},

    {ORGAN,        NOTE_A4S, 2},
    {ORGAN,        NOTE_D5,  2},
    {ORGAN,        NOTE_E5,  2},

    {ORGAN,        NOTE_A4S, 2},
    {ORGAN,        NOTE_C5,  2},
    {ORGAN,        NOTE_E5,  2},
    {SILENCE,      END_SONG, 0}
  };

  const PROGMEM SoundPlayer::sound_t js_bach_joy[] = {
    {PIANO,        NOTE_G3,  4},
    {PIANO,        NOTE_A3,  4},
    {PIANO,        NOTE_B3,  4},
    {PIANO,        NOTE_D4,  3},
    {SILENCE,      REST,     1},

    {PIANO,        NOTE_C4,  3},
    {SILENCE,      REST,     1},
    {PIANO,        NOTE_C4,  4},
    {PIANO,        NOTE_E4,  3},
    {SILENCE,      REST,     1},
    {PIANO,        NOTE_D4,  2},
    {SILENCE,      REST,     2},

    {PIANO,        NOTE_D4,  4},
    {PIANO,        NOTE_G4 , 3},
    {SILENCE,      REST,     1},
    {PIANO,        NOTE_F4S, 4},
    {PIANO,        NOTE_G4,  4},

    {PIANO,        NOTE_D4,  2},
    {SILENCE,      REST,     2},
    {PIANO,        NOTE_B3,  3},
    {SILENCE,      REST,     1},
    {PIANO,        NOTE_G3,  4},
    {PIANO,        NOTE_A3,  2},
    {SILENCE,      REST,     2},

    {PIANO,        NOTE_B3,  2},
    {SILENCE,      REST,     2},
    {PIANO,        NOTE_C4,  4},
    {PIANO,        NOTE_D4,  2},
    {SILENCE,      REST,     2},
    {PIANO,        NOTE_E4,  2},
    {SILENCE,      REST,     2},

    {PIANO,        NOTE_D4,  4},
    {PIANO,        NOTE_C4,  2},
    {SILENCE,      REST,     2},
    {PIANO,        NOTE_B3,  2},
    {SILENCE,      REST,     2},
    {PIANO,        NOTE_A3,  4},

    {PIANO,        NOTE_B3,  2},
    {SILENCE,      REST,     2},
    {PIANO,        NOTE_G3,  2},
    {SILENCE,      REST,     2},
    {PIANO,        NOTE_G3,  8},
    {SILENCE,      END_SONG, 0}
  };
  
  const PROGMEM SoundPlayer::sound_t big_band[] = {
    {XYLOPHONE,    NOTE_F4,  3},
    {XYLOPHONE,    NOTE_G4,  3},
    {XYLOPHONE,    NOTE_F4,  3},
    {XYLOPHONE,    NOTE_D4,  3},
    {XYLOPHONE,    NOTE_A3S, 3},
    {SILENCE, REST,     3},
    
    {TRUMPET,    NOTE_F4,  3},
    {TRUMPET,    NOTE_G4,  3},
    {TRUMPET,    NOTE_F4,  3},
    {TRUMPET,    NOTE_D4,  3},
    {TRUMPET,    NOTE_A3S, 3},
    {SILENCE, REST,     3},
    
    {TUBA,    NOTE_A2S, 6},
    {TUBA,    NOTE_A2S, 6},
    {TUBA,    NOTE_A2S, 4},
    {TUBA,    NOTE_A2S, 6},
    {TUBA,    NOTE_A2S, 6},
    {SILENCE, END_SONG, 0}
  };

  const PROGMEM SoundPlayer::sound_t beats[] = {
    {SILENCE,      REST,     8},
    {NOTCH,        NOTE_C4,  8},
    {KICKDRUM,     NOTE_C4,  8},
    {HIHAT,        NOTE_C4,  8},
    {COWBELL,      NOTE_C4,  8},
    {SILENCE,      REST,     8},
    {NOTCH,        NOTE_C4,  8},
    {KICKDRUM,     NOTE_C4,  8},
    {HIHAT,        NOTE_C4,  8},
    {COWBELL,      NOTE_C4,  8},
    {SILENCE,      REST,     8},
    {NOTCH,        NOTE_C4,  8},
    {KICKDRUM,     NOTE_C4,  8},
    {HIHAT,        NOTE_C4,  8},
    {COWBELL,      NOTE_C4,  8},
    {SILENCE,      END_SONG, 0}
  };

  const PROGMEM SoundPlayer::sound_t beeping[] = {
    {BEEPING,      NOTE_C4,  64},
    {SILENCE,      END_SONG, 0}
  };

  const PROGMEM SoundPlayer::sound_t alarm[] = {
    {ALARM,        NOTE_C4,  64},
    {SILENCE,      END_SONG, 0}
  };

  const PROGMEM SoundPlayer::sound_t warble[] = {
    {WARBLE,       NOTE_C4,  64},
    {SILENCE,      END_SONG, 0}
  };

  const PROGMEM SoundPlayer::sound_t carousel[] = {
    {CAROUSEL,     NOTE_C4,  64},
    {SILENCE,      END_SONG, 0}
  };

  const PROGMEM SoundPlayer::sound_t all_instruments[] = {
    {HARP},
    {XYLOPHONE},
    {TUBA},
    {GLOCKENSPIEL},
    {ORGAN},
    {TRUMPET},
    {PIANO},
    {CHIMES},
    {MUSIC_BOX},
    {BELL},
    {CLICK},
    {SWITCH},
    {COWBELL},
    {NOTCH},
    {HIHAT},
    {KICKDRUM},
    {SWITCH},
    {POP},
    {CLACK},
    {CHACK},
    {SILENCE, END_SONG, 0}
  };
}

#endif // _UI_SOUNDS_H_
