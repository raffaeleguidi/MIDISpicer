/*
  MIDISpicer - v3
*/

#include "MIDIUSB.h"

// First parameter is the event type (0x09 = note on, 0x08 = note off).
// Second parameter is note-on/note-off, combined with the channel.
// Channel can be anything between 0-15. Typically reported to the user as 1-16.
// Third parameter is the note number (48 = middle C).
// Fourth parameter is the velocity (64 = normal, 127 = fastest).

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t cc = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(cc);
}

void programChange(byte channel, byte program) {
  midiEventPacket_t pc = {0x0C, 0xC0 | channel, program, 0};
  MidiUSB.sendMIDI(pc);
}

struct MidiCC {
  byte channel;
  byte control;
  MidiCC(byte _ch, byte _ctl) {
    channel = _ch;
    control = _ctl;
  }
};

struct SpicerInput {
  int pin;
  int analog = false;
  int value = -1;
  int lastValue = -1;
  int scaleTo = 0;
  int scaled = 0;
  int tolerance = 10;
  char *name;
  int changed = false;
  MidiCC *CC;

  SpicerInput(char *_name, int _pin, MidiCC *_CC){ // digital constructor
    Serial.print("digital input ");
    name = _name;
    Serial.println(name);
    pin = _pin;
    analog = false;
    CC = _CC;
    pinMode(pin, INPUT_PULLUP);
    update();
  }
  SpicerInput(char *_name, int _pin, MidiCC *_CC, int _scaleTo, int _tolerance){ // analog constructor
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
  int update() {
    lastValue = value; 
    if (analog) {
      value = analogRead(pin);
      if (scaleTo > 0) scaled = map(value, 0, 1023, 0, scaleTo);
      int diff = max(lastValue, value) - min(lastValue, value);
      changed = (diff > tolerance); 
      if (changed) {
        controlChange(CC->channel, CC->control, scaled);
      }
    } else {
      value = !digitalRead(pin);
      changed = lastValue != value; 
      if (changed && value) {
        controlChange(CC->channel, CC->control, 1);
      }
    }
    //input->onChange(input);
    return changed;
  }
  void debug(int force){
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
};

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
  Push1 = new SpicerInput("Push1", 22, new MidiCC(0, 127));
  Trimmer1 = new SpicerInput("Trimmer1", A5, new MidiCC(0, 80), 127, 10);
  Jack1 = new SpicerInput("Jack1", A4, new MidiCC(0, 7), 127, 20);
}

void setup() {
  Serial.begin(9600);
  Serial.println("MIDISpicer started");
  Serial.println("******************");
  setupInputs();
  setupLED();
}

void loop() {
  
    int changed = 
      Push1->update() ||
      Trimmer1->update() || 
      Jack1->update();

  if (changed ) {
 /* if (Push1->changed && Push1->value) controlChange(0, 127, 127); // Set the value of controller 10 on channel 0 to 65
    if (Trimmer1->changed) controlChange(0, 1, Trimmer1->scaled); // Set the value of controller 10 on channel 0 to 65
    if (Jack1->changed) controlChange(0, 7, Jack1->scaled); // Set the value of controller 10 on channel 0 to 65*/
/*    Push1->debug(Push1->changed);
    Trimmer1->debug(Push1->changed);
    Jack1->debug(Push1->changed);
    Serial.println();*/
    MidiUSB.flush();
  }
  delay(10);
}
