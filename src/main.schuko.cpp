#include <EEPROM.h>
#include "midiController.h"
#include "midiControllerBle.h"
#include "relayController.h"
#include "config.h"
#include "ota.h"

#define EEPROM_SIZE 100
#define EEPROM_ADD_LETTER 0

#define ALIVE_INFO_INTERVAL_MILLIS 1500

#define DEBUG_ENABLE true

static unsigned long aliveTime = 0;
static unsigned long startupTime = 0;

static MidiData midiData;
static Config config;

void initConfig(const byte value) {
    delay(5000);
    EEPROM.write(EEPROM_ADD_LETTER, value);
    EEPROM.commit();
    Serial.printf("written config value %d\n", value);
}

Config readConfig() {
    const byte letter = EEPROM.read(EEPROM_ADD_LETTER);
    Serial.printf("read config for letter %d\n", letter);
    return getConfig(letter);
}

void printMemoryStatus() {
    Serial.printf("=== Speicherstatus des ESP32 ===\n");
    Serial.printf("Gesamter Heap-Speicher: %d Bytes\n", ESP.getHeapSize());
    Serial.printf("Freier Heap-Speicher: %d Bytes\n", ESP.getFreeHeap());
    Serial.printf("Minimale freie Heap-Speichergröße: %d Bytes\n", ESP.getMinFreeHeap());
    Serial.printf("Maximale Allokierbare Speichergröße: %d Bytes\n", ESP.getMaxAllocHeap());
}

void printAlive() {
    if (millis() > aliveTime + ALIVE_INFO_INTERVAL_MILLIS) {
        aliveTime = millis();
        Serial.println(".");
    }
}

void setup() {
    if (DEBUG_ENABLE) {
        delay(5000); // time to start serial console
        Serial.begin(115200);
    }

    Serial.println("start init");

    EEPROM.begin(EEPROM_SIZE);

    // 0 for R, 1 for S, 2 for G
    //initConfig(3);

    config = readConfig();

    MIDIC_init(config.MIDI_CHANNEL, &midiData);
    MIDICBLE_init(config.MIDI_CHANNEL, config.LETTER, &midiData);
    RELAYC_init(&config);
    OTA_init(config.LETTER, config.IP);

    printMemoryStatus();

    startupTime = millis();
}


void loop() {
    printAlive();

    OTA_loop();
    MIDICBLE_loop();

    RELAYC_update(midiData.noteOn, midiData.controls);
}
