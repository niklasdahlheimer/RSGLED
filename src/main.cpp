#include <SoftwareSerial.h>
#include "helper.h"
#include "midiController.h"
#include "ledController.h"

SoftwareSerial logSerial(4, 5); // RX, TX

void setup() {
    logSerial.begin(9600); // Software-Serial for second Arduino
    serialPrintf(&logSerial, "Hello from Arduino 1!");

    LEDC_init(&logSerial);
    MIDIC_init(&logSerial);
}

void loop() {
    MIDIC_read();
    LEDC_updateStripe(MIDIC_getNoteOnArray(), millis());
    maybeDisplayCriticalRam(&logSerial);
}
