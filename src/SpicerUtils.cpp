/*
  SpicerUtils - v3
*/


#include <Arduino.h>
#include "SpicerUtils.h"

CCDefinition::CCDefinition(byte _ch, byte _ctl){
  channel = _ch;
  control = _ctl;
};

SpicerInput::SpicerInput(char *_name, int _pin, CCDefinition *_CC){ // digital constructor
    Serial.print("digital input ");
    name = _name;
    Serial.println(name);
    pin = _pin;
    analog = false;
    CC = _CC;
    pinMode(pin, INPUT_PULLUP);
    update();
}
SpicerInput::SpicerInput(char *_name, int _pin, CCDefinition *_CC, int _scaleTo, int _tolerance){ // analog constructor
  Serial.print("analog input ");
  name = _name;
  Serial.println(name);
  pin = _pin;
  analog = true;
  CC = _CC;
  scaleTo = _scaleTo;
  tolerance = _tolerance;
  update();
}

int SpicerInput::update() {
  lastValue = value; 
  if (analog) {
    value = analogRead(pin);
    if (scaleTo > 0) scaled = map(value, 0, 1023, 0, scaleTo);
    int diff = max(lastValue, value) - min(lastValue, value);
    changed = (diff > tolerance); 
    if (changed) {
      Midi::cc(CC->channel, CC->control, scaled);
    }
  } else {
    value = !digitalRead(pin);
    changed = lastValue != value; 
    if (changed && value) {
      Midi::cc(CC->channel, CC->control, 1);
    }
  }
  //input->onChange(input);
  return changed;
}

void SpicerInput::debug(int force){
  if (!changed &&! force) return;
  Serial.print("value of ");
  Serial.print(name);
  Serial.print(" is: ");
  Serial.print(value);
  if (scaleTo) {
    Serial.print(" (");
    Serial.print(scaled);
    Serial.print(")");
  }
  if (changed) Serial.print(" *CHANGED*");
  Serial.println();
}

