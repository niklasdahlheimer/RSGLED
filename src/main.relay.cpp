#include <EEPROM.h>
#include <midiData.h>
#include "midiControllerBle.h"
#include "relayController.h"
#include "config.h"
#include "ezButton.h"
#include "led.h"
#include "ota.h"

#define EEPROM_SIZE 100
#define EEPROM_ADD_LETTER 0

#define ALIVE_INFO_INTERVAL_MILLIS 1000

#define DEBUG_ENABLE true

static unsigned long aliveTime = 0;
static unsigned long startupTime = 0;

static MidiData midiData;
static Config config;

ezButton buttonOnboard(GPIO_NUM_0);
ezButton buttonExtern1(GPIO_NUM_5);
ezButton buttonExtern2(GPIO_NUM_4);

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

void maybePrintAlive() {
    if (millis() > aliveTime + ALIVE_INFO_INTERVAL_MILLIS) {
        aliveTime = millis();
        Serial.println(".");
    }
}

void setup() {
    startupTime = millis();

    if (DEBUG_ENABLE) {
        //delay(5000); // time to start serial console
        Serial.begin(115200);
        Serial.println("start init");
        printMemoryStatus();
    }

    buttonOnboard.setDebounceTime(500);
    buttonExtern1.setDebounceTime(500);
    buttonExtern2.setDebounceTime(500);

    // init and read EEPROM
    EEPROM.begin(EEPROM_SIZE);
    // 3 for 0 (left), 4 for 1 (right)
    //initConfig(3);
    config = readConfig();

    RELAYC_init();
    MIDICBLE_init(config.MIDI_CHANNEL, config.LETTER, &midiData);
    OTA_init(config.LETTER, config.IP);
}


void loop() {
    maybePrintAlive();

    OTA_loop();
    MIDICBLE_loop();

    buttonOnboard.loop();
    buttonExtern1.loop();
    buttonExtern2.loop();

    // LOW = Button gedrückt
    // solange es keine andere sinvolle Funktion für buttonExternal2 gibt ->beide machen dauerhaft an
    const boolean isButtonPressed = buttonOnboard.getState() == LOW || buttonExtern1.getState() == LOW || buttonExtern2.
                                    getState() == LOW;
    //LED_blinkFast(&midiLED);

    RELAYC_update(midiData.noteOn, midiData.controls, isButtonPressed);
}
