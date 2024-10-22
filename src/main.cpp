#include "midiController.h"
#include "ledController.h"
#include "midiControllerBle.h"

MidiData *midiData;

void setup() {
    Serial.begin(115200);
    LEDC_init();
    MIDIC_init();
    MIDICBLE_init();
}

void loop() {
    midiData = MIDICBLE_read();
    if(TEST_MODE){
        midiData->noteOn[RAINBOW] = true;
    }
    LEDC_updateStripe(midiData->noteOn, midiData->controls);
}
