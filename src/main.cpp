#include <EEPROM.h>
#include "midiController.h"
#include "ledController.h"
#include "midiControllerBle.h"
#include "encoderController.h"
#include "config.h"
#include "ota.h"

#define EEPROM_SIZE 100
#define EEPROM_ADD_LETTER 0

#define ALIVE_INFO_INTERVAL_MILLIS 5000

#define FREE_RUN_START_MILLIS 60000

#define HELLO_PHASE_MILLIS 1500

static unsigned long aliveTime = 0;

static unsigned long startupTime = 0;
static bool isHelloPhaseFinished = false;

static MidiData midiData;
static Config config;
static State encoderState;

static unsigned long freeRunSetTime = 0;

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

    // delay for debugging to start serial monitor
    //delay(4000);

    EEPROM.begin(EEPROM_SIZE);

    // 0 for R, 1 for S, 2 for G
    //initConfig(0);

    config = readConfig();

    LEDC_init(&config);
    MIDIC_init(config.MIDI_CHANNEL, &midiData);
    MIDICBLE_init(config.MIDI_CHANNEL, config.LETTER, &midiData);
    ENCODER_init(&encoderState);
    OTA_init(config.LETTER, config.IP);

    printMemoryStatus();

    startupTime = millis();
}

void handleFreeRun() {
    //activate free run after 60secs or after E-2 NoteOn
    if (midiData.noteOn[FREE_RUN] == 0 &&
        (midiData.noteOn[FREE_RUN_START] != 0 || millis() - MIDICBLE_lastNoteOn() > FREE_RUN_START_MILLIS)
    ) {
        Serial.println("start free run");
        freeRunSetTime = millis();
        midiData.noteOn[FREE_RUN] = 255;
    }
    // reset free run if any note has changed
    if (midiData.noteOn[FREE_RUN] != 0 && MIDICBLE_lastNoteOn() > freeRunSetTime) {
        Serial.println("stop free run");
        midiData.noteOn[FREE_RUN] = 0;
    }
}

void handleHelloPhase() {
    // activate notes for hello phase
    if (!isHelloPhaseFinished) {
        if (millis() - startupTime < HELLO_PHASE_MILLIS) {
            midiData.noteOn[ALL_ON_COLOR_1] = 120;
            midiData.noteOn[PUMP] = 255;
        } else {
            Serial.println("hello phase finished");
            isHelloPhaseFinished = true;
            midiData.noteOn[ALL_ON_COLOR_1] = 0;
            midiData.noteOn[PUMP] = 0;
        }
    }
}

void handleEncoderState() {
    midiData.noteOn[CONFIG_MODE_TEST] = encoderState.mode == TEST ? 255 : 0;
    midiData.noteOn[CONFIG_MODE_BRIGHTNESS] = encoderState.mode == BRIGHTNESS ? 255 : 0;
    midiData.noteOn[CONFIG_MODE_LINE] = encoderState.mode == LINE ? 255 : 0;

    if (encoderState.mode == BRIGHTNESS) {
        midiData.controls[CONTROLLER_GLOBAL_BRIGHTNESS_TRIM] = 255 - encoderState.value;
    } else if (encoderState.mode == LINE) {
        midiData.controls[CONTROLLER_LINE_INDEX] = encoderState.value;
    }
}

void loop() {
    printAlive();

    OTA_loop();
    MIDICBLE_loop();
    ENCODER_loop();

    MIDICBLE_read();
    //MIDIC_read();

    handleFreeRun();
    handleHelloPhase();
    handleEncoderState();

    LEDC_updateStripe(midiData.noteOn, midiData.controls);
}
