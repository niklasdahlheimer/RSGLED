
#include "midiController.h"
#include "ledController.h"



MidiData* midiData;

void setup() {
    Serial.begin(9600); // Software-Serial for second Arduino
    //serialPrintf(&logSerial, "Hello from Arduino 1!");

    LEDC_init();
    MIDIC_init();
}

void loop() {
    midiData = MIDIC_read();
    if(TEST_MODE){
        midiData->noteOn[RAINBOW] = true;
    }
    LEDC_updateStripe(midiData->noteOn, midiData->controls);
    //maybeDisplayCriticalRam(&logSerial);
}
