#include "relayController.h"

void reset() {
    Serial.println("relay controller: reset");
    digitalWrite(RELAY1, LOW);
    digitalWrite(RELAY2, LOW);
}

void RELAYC_init(const Config *config) {
    pinMode(RELAY1, OUTPUT);
    pinMode(RELAY2, OUTPUT);
    reset();
}

void RELAYC_update(const byte *note, const byte *controller) {
    if (note[TOTAL_RESET]) reset();

    digitalWrite(RELAY1, note[RELAY_1_ON] ? HIGH : LOW);
    digitalWrite(RELAY2, note[RELAY_2_ON] ? HIGH : LOW);
}
