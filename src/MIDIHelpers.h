/*
  MIDIHelpers - v3
*/

#pragma once

#include <Arduino.h>

namespace Midi {
  void noteOn(byte channel, byte pitch, byte velocity);
  void noteOff(byte channel, byte pitch, byte velocity);
  void cc(byte channel, byte control, int value);
  void pc(byte channel, byte program);
  void flush();
};
