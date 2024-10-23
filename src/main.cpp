#include <EEPROM.h>

#include "midiController.h"
#include "ledController.h"
#include "midiControllerBle.h"

#define EEPROM_SIZE 100
#define EEPROM_ADD_LETTER 0

static MidiData *midiData;
static Config config;

static unsigned long aliveTime = 0;
static unsigned long ALIVE_INFO_INTERVAL_MILLIS = 5000;


void primeConfig(const byte value) {
    delay(5000);
    EEPROM.write(EEPROM_ADD_LETTER, value);
    EEPROM.commit();
    Serial.printf("written config value %d\n", value);
}

Config readConfig() {
    const byte letter =  EEPROM.read(EEPROM_ADD_LETTER);
    Serial.printf("read config for letter %d\n", letter);
    return getConfig(letter);
}

void setup() {
    EEPROM.begin(EEPROM_SIZE);
    Serial.begin(115200);
    Serial.println("start init");
    
    // 0 for R, 1 for S, 2 for G
    //primeConfig(0);

    config = readConfig();

    LEDC_init(&config);
    MIDIC_init(config.MIDI_CHANNEL);
    MIDICBLE_init(config.MIDI_CHANNEL);
}

void loop() {
    if(millis() > aliveTime+ALIVE_INFO_INTERVAL_MILLIS) {
        aliveTime = millis();
        Serial.println(".-");
    }

    midiData = MIDICBLE_read();
    /*for (int i = 0; i < 254; ++i) {
        if(midiData->noteOn[i] != 0) {
            Serial.printf("note on %d %d", i, midiData->noteOn[i]);
        }
    }*/

    LEDC_updateStripe(midiData->noteOn, midiData->controls);
}
