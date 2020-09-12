/*
  SpicerUtils - v3
*/

#pragma once

#include <Arduino.h>
#include "MIDIHelpers.h"

class CCMessage {
  public:
  byte channel;
  byte control;
  CCMessage(byte, byte);
};

class SpicerInput {
  public:
  int pin;
  int analog = false;
  int value = -1;
  int lastValue = -1;
  int scaleTo = 0;
  int scaled = 0;
  int tolerance = 10;
  char *name;
  int changed = false;
  CCMessage *CC;

  SpicerInput(char *, int, CCMessage *);
  SpicerInput(char *, int, CCMessage *, int, int);
  int update();
  void debug(int force);
};



