
#include "midiController.h"
#include "ledController.h"



MidiData* midiData;

void setup() {
    Serial.begin(115200);
    LEDC_init();
    MIDIC_init();
}

void loop() {
    midiData = MIDIC_read();
    if(TEST_MODE){
        midiData->noteOn[RAINBOW] = true;
    }
    midiData->noteOn[RAINBOW] = true;
    LEDC_updateStripe(midiData->noteOn, midiData->controls);
    //maybeDisplayCriticalRam(&logSerial);
    delay(10000);
}
