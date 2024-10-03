#include <SoftwareSerial.h>
#include "helper.h"
#include "midiController.h"
#include "ledController.h"

SoftwareSerial logSerial(4, 5); // RX, TX

MidiData* midiData;

void setup() {
    Serial.begin(9600); // Software-Serial for second Arduino
    serialPrintf(&logSerial, "Hello from Arduino 1!");

    LEDC_init(&logSerial);
    MIDIC_init(&logSerial);
}

void loop() {
    midiData = MIDIC_read();
    if(TEST_MODE){
        midiData->noteOn[NOTE_RAINBOW] = true;
    }
    LEDC_updateStripe(midiData->noteOn, midiData->controls, midiData->tempo);
    maybeDisplayCriticalRam(&logSerial);
}
