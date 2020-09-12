/*
  MIDIHelpers - v3
*/

#include <Arduino.h>
#include "MidiHelpers.h"
#include "MIDIUSB.h"

namespace Midi {
  void noteOn(byte channel, byte pitch, byte velocity) {
    midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
    MidiUSB.sendMIDI(noteOn);
  }
  void noteOff(byte channel, byte pitch, byte velocity) {
    midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
    MidiUSB.sendMIDI(noteOff);
  }
  void cc(byte channel, byte control, int value) {
    midiEventPacket_t cc = {0x0B, 0xB0 | channel, control, value};
    MidiUSB.sendMIDI(cc);
  }
  void pc(byte channel, byte program) {
    midiEventPacket_t pc = {0x0C, 0xC0 | channel, program, 0};
    MidiUSB.sendMIDI(pc);
  }
  void flush() {
    MidiUSB.flush();
  }

};
