#include <SoftwareSerial.h>
#include "helper.h"

#include "midiController.h"
#include "ledController.h"

LEDController *ledController;

SoftwareSerial logSerial(4, 5); // RX, TX

void setup() {
    logSerial.begin(9600); // Software-Serial for second Arduino
    serialPrintf(&logSerial, "Hello from Arduino 1!");

    ledController = new LEDController(&logSerial);
    MIDIC_init(&logSerial);
}

void loop() {
    MIDIC_read();
    ledController->updateLEDStripe(MIDIC_getNoteOnArray(), millis());
    maybeDisplayCriticalRam(&logSerial);
}
