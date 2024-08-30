#include <SoftwareSerial.h>
#include "helper.h"

#include "midiController.h"
#include "ledController.h"

MidiController *midiController;
LEDController *ledController;

SoftwareSerial logSerial(4, 5); // RX, TX

void setup() {
    logSerial.begin(9600); // Software-Serial for second Arduino
    serialPrintf(&logSerial, "Hello from Arduino 1!");

    ledController = new LEDController(&logSerial);
    midiController = new MidiController(&logSerial);
}

void loop() {
    midiController->read();
    ledController->updateLEDStripe(midiController->getNoteOnArray(), millis());
    maybeDisplayCriticalRam(&logSerial);
}
