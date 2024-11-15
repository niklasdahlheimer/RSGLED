#include <EEPROM.h>
#include "midiController.h"
#include "ledController.h"
#include "midiControllerBle.h"
#include "config.h"
#include "AiEsp32RotaryEncoder.h"
#include "ota.h"

#define EEPROM_SIZE 100
#define EEPROM_ADD_LETTER 0

#define ALIVE_INFO_INTERVAL_MILLIS 5000

#define HELLO_PHASE_MILLIS 1000

#define ENCODER_PIN_A 22
#define ENCODER_PIN_B 23
#define ENCODER_BUTTON_PIN 33

AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ENCODER_PIN_A, ENCODER_PIN_B, ENCODER_BUTTON_PIN, -1, 4);

static unsigned long aliveTime = 0;
static unsigned long startupTime = 0;
static MidiData midiData;
static Config config;
static bool isTestMode = false;

static bool isHelloPhaseFinished = false;

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
    pinMode(ENCODER_PIN_A, INPUT_PULLUP);
    pinMode(ENCODER_PIN_B, INPUT_PULLUP);
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
    //initConfig(0);

    config = readConfig();

    LEDC_init(&config);
    MIDIC_init(config.MIDI_CHANNEL, &midiData);
    MIDICBLE_init(config.MIDI_CHANNEL, config.LETTER, &midiData);

    printMemoryStatus();
    setupEncoder();

    OTA_init(config.LETTER);

    startupTime = millis();
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
    OTA_loop();
    MIDICBLE_loop();
    handleEncoder();

    MIDICBLE_read();
    MIDIC_read();

    midiData.noteOn[TEST_MODE] = isTestMode ? 255 : 0;

    if (!isHelloPhaseFinished) {
        if (millis() - startupTime < HELLO_PHASE_MILLIS) {
            midiData.noteOn[ALL_ON_COLOR_1] = 120;
            midiData.noteOn[PUMP] = 255;
        } else {
            isHelloPhaseFinished = true;
            midiData.noteOn[ALL_ON_COLOR_1] = 0;
            midiData.noteOn[PUMP] = 0;
        }
    }

    LEDC_updateStripe(midiData.noteOn, midiData.controls);
}
