#include "relayController.h"

void reset() {
    Serial.println("relay controller: reset");
    digitalWrite(RELAY1_PIN, LOW);
    digitalWrite(RELAY2_PIN, LOW);
    digitalWrite(RELAY3_PIN, LOW);
}

void RELAYC_init() {
    pinMode(RELAY1_PIN, OUTPUT);
    pinMode(RELAY2_PIN, OUTPUT);
    pinMode(RELAY3_PIN, OUTPUT);
    reset();
}

void RELAYC_update(const byte *note, const byte *controller) {
    if (note[TOTAL_RESET]) reset();

    digitalWrite(RELAY1_PIN, note[RELAY_1_ON] ? HIGH : LOW);
    digitalWrite(RELAY2_PIN, note[RELAY_2_ON] ? HIGH : LOW);
    digitalWrite(RELAY3_PIN, note[RELAY_3_ON] ? HIGH : LOW);
}
