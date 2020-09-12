/*
  MIDISpicer - v3
*/

#include <Arduino.h>
#include "SpicerUtils.h"
#include "MIDIHelpers.h"

void blinkFast(int times){
  for(int n=0;n<times; n++){
    digitalWrite(LED_BUILTIN, HIGH);    // turn the LED off by making the voltage LOW
    delay(100);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(100);
  }
}

void setupLED(){
  pinMode(LED_BUILTIN, OUTPUT);
  blinkFast(3);
}

SpicerInput *Push1; 
SpicerInput *Trimmer1;
SpicerInput *Jack1; 

void setupInputs(){
  //SpicerInput *Test = new SpicerInput();
  Push1 = new SpicerInput("Push1", 22, new CCMessage(0, 1));
  Trimmer1 = new SpicerInput("Trimmer1", A5, new CCMessage(0, 80), 127, 10);
  Jack1 = new SpicerInput("Jack1", A4, new CCMessage(0, 7), 127, 20);
}

void setup() {
  Serial.begin(9600);
  Serial.println("MIDISpicer_v3 started");
  Serial.println("*********************");
  setupInputs();
  setupLED();
}

void loop() {
  
  int changed = 
    Push1->update() ||
    Trimmer1->update() || 
    Jack1->update();

  if (Push1->changed && Push1->value) {
  }
  if (changed) {
    if (Push1->changed && Push1->value) {
      Push1->debug(Push1->changed);
      Serial.println();
    }
    Midi::flush();
  }
  delay(10);
}
