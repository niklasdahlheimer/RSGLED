#include <EEPROM.h>
#include "midiController.h"
#include "ledController.h"
#include "midiControllerBle.h"
#include "config.h"

#define EEPROM_SIZE 100
#define EEPROM_ADD_LETTER 0
#define ALIVE_INFO_INTERVAL_MILLIS 5000

static unsigned long aliveTime = 0;
static MidiData *midiData;
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
    Serial.begin(115200);
    Serial.println("start init");

    EEPROM.begin(EEPROM_SIZE);

    // 0 for R, 1 for S, 2 for G
    //primeConfig(0);

    config = readConfig();

    LEDC_init(&config);
    MIDIC_init(config.MIDI_CHANNEL);
    MIDICBLE_init(config.MIDI_CHANNEL);

    printMemoryStatus();
}

void loop() {
    printAlive();
    midiData = MIDICBLE_read();
    LEDC_updateStripe(midiData->noteOn, midiData->controls);
}
