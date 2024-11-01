#include <EEPROM.h>
#include "midiController.h"
#include "ledController.h"
#include "midiControllerBle.h"
#include "config.h"
#include "AiEsp32RotaryEncoder.h"

#define EEPROM_SIZE 100
#define EEPROM_ADD_LETTER 0

#define ALIVE_INFO_INTERVAL_MILLIS 5000

AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(23, 22, 25, -1, 4);

static unsigned long aliveTime = 0;
static MidiData *midiData;
static Config config;
static bool isTestMode = false;

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

void IRAM_ATTR readEncoderISR() {
    rotaryEncoder.readEncoder_ISR();
}

void setupEncoder() {
    pinMode(22, INPUT_PULLUP);
    pinMode(23, INPUT_PULLUP);
    rotaryEncoder.areEncoderPinsPulldownforEsp32 = false;
    rotaryEncoder.begin();
    rotaryEncoder.setup(readEncoderISR);
    rotaryEncoder.setBoundaries(0, 100, false);
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
    setupEncoder();
}

void handleEncoder() {
    if (rotaryEncoder.encoderChanged()) {
        Serial.printf("encoder value: %d\n", rotaryEncoder.readEncoder());
    }
    if (rotaryEncoder.isEncoderButtonClicked()) {
        Serial.printf("button pressed!\n");
        isTestMode = !isTestMode;
    }
}

void loop() {
    printAlive();
    handleEncoder();
    midiData = MIDICBLE_read();
    midiData->noteOn[TEST_MODE] = isTestMode ? 255 : 0;
    LEDC_updateStripe(midiData->noteOn, midiData->controls);
}
