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

void primeConfig(const byte value) {
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
    // Ermittelten freien Heap-Speicher
    uint32_t freeHeap = ESP.getFreeHeap();
    uint32_t totalHeap = ESP.getHeapSize();
    uint32_t minFreeHeap = ESP.getMinFreeHeap();
    uint32_t maxAllocHeap = ESP.getMaxAllocHeap();

    // Ausgabe des Speicherstatus über die serielle Schnittstelle
    Serial.println("=== Speicherstatus des ESP32 ===");
    Serial.print("Gesamter Heap-Speicher: ");
    Serial.print(totalHeap);
    Serial.println(" Bytes");

    Serial.print("Freier Heap-Speicher: ");
    Serial.print(freeHeap);
    Serial.println(" Bytes");

    Serial.print("Minimale freie Heap-Speichergröße: ");
    Serial.print(minFreeHeap);
    Serial.println(" Bytes");

    Serial.print("Maximale Allokierbare Speichergröße: ");
    Serial.print(maxAllocHeap);
    Serial.println(" Bytes");

    Serial.println("===============================\n");
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
    if (millis() > aliveTime + ALIVE_INFO_INTERVAL_MILLIS) {
        aliveTime = millis();
        Serial.println(".");
    }

    midiData = MIDICBLE_read();
    LEDC_updateStripe(midiData->noteOn, midiData->controls);
}
