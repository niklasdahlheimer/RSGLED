#include <EEPROM.h>
#include <midiData.h>
#include "midiControllerBle.h"
#include "relayController.h"
#include "config.h"
#include "ezButton.h"
#include "logging.h"

#define EEPROM_SIZE 100
#define EEPROM_LETTER_ADDRESS 0

#define ALIVE_INFO_INTERVAL_MILLIS 1000

static unsigned long aliveTime = 0;
static unsigned long startupTime = 0;

static MidiData midiData;
static Config config;

ezButton buttonOnboard(GPIO_NUM_0);
ezButton buttonExtern1(GPIO_NUM_5,INTERNAL_PULLUP);
ezButton buttonExtern2(GPIO_NUM_4,INTERNAL_PULLUP);

bool isToggleOn = false;
ezButton pushButtonExtern(GPIO_NUM_2, INTERNAL_PULLUP);

void initConfig(const byte value) {
    delay(5000);
    EEPROM.write(EEPROM_LETTER_ADDRESS, value);
    EEPROM.commit();
    LOGD("written config value %d\n", value);
}

Config readConfig() {
    const byte letter = EEPROM.read(EEPROM_LETTER_ADDRESS);
    LOGD("read config for letter %d\n", letter);
    return getConfig(letter);
}

void printMemoryStatus() {
    LOGD("=== Speicherstatus des ESP32 ===\n");
    LOGD("Gesamter Heap-Speicher: %d Bytes\n", ESP.getHeapSize());
    LOGD("Freier Heap-Speicher: %d Bytes\n", ESP.getFreeHeap());
    LOGD("Minimale freie Heap-Speichergröße: %d Bytes\n", ESP.getMinFreeHeap());
    LOGD("Maximale Allokierbare Speichergröße: %d Bytes\n", ESP.getMaxAllocHeap());
}

void maybePrintAlive() {
    if (millis() > aliveTime + ALIVE_INFO_INTERVAL_MILLIS) {
        aliveTime = millis();
        LOGN(".");
    }
}

void setup() {
    startupTime = millis();

    if (DEBUG_LOG_ENABLE) {
        delay(2000); // time to start serial console
        Serial.begin(115200);
        LOGN("start init");
        printMemoryStatus();
    }

    buttonOnboard.setDebounceTime(100);
    buttonExtern1.setDebounceTime(100);
    buttonExtern2.setDebounceTime(100);
    pushButtonExtern.setDebounceTime(100);

    // init and read EEPROM
    EEPROM.begin(EEPROM_SIZE);
    // 3 for 0 (left), 4 for 1 (right)
    //initConfig(3);
    config = readConfig();

    RELAYC_init();
    MIDICBLE_init(config.MIDI_CHANNEL, config.LETTER, &midiData);
    //OTA_init(config.LETTER, config.IP);
}


void loop() {
    maybePrintAlive();

    //OTA_loop();
    MIDICBLE_loop();

    buttonOnboard.loop();
    buttonExtern1.loop();
    buttonExtern2.loop();
    pushButtonExtern.loop();

    if (pushButtonExtern.isPressed()) {
        LOGN("pushButtonExtern pressed! Toggle flag");
        isToggleOn = !isToggleOn;
    }


    // LOW = "Button gedrückt" da intern pullup und bei Druck auf Masse verbunden wird
    // solange es keine andere sinvolle Funktion für buttonExternal2 gibt ->beide machen dauerhaft an
    const boolean isButtonPressed = buttonOnboard.getState() == LOW ||
                                    buttonExtern1.getState() == LOW ||
                                    buttonExtern2.getState() == LOW ||
                                    isToggleOn;

    //LED_blinkFast(&midiLED);

    RELAYC_update(midiData.noteOn, midiData.controls, isButtonPressed);
}
