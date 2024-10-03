#include <SoftwareSerial.h>
#include "helper.h"
#include "midiController.h"
#include "ledController.h"

#define TEST_MODE false

SoftwareSerial logSerial(4, 5); // RX, TX

void setup() {
    Serial.begin(9600); // Software-Serial for second Arduino
    serialPrintf(&logSerial, "Hello from Arduino 1!");

    LEDC_init(&logSerial);
    MIDIC_init(&logSerial);
}

void loop() {
    MIDIC_read();
    if(TEST_MODE){
        MIDIC_getNoteOnArray()[NOTE_RAINBOW] = true;
    }
    LEDC_updateStripe(MIDIC_getNoteOnArray(), millis());
    maybeDisplayCriticalRam(&logSerial);
}
