#include <FastLED.h>
#include <MIDI.h>
#include <SoftwareSerial.h>
#include "helper.h"
#include "midiConsts.h"
#include "protothreads.h"

// LED header

#define LED_CHIP WS2811
#define COLOR_ORDER BRG
#define NUM_LED 10
#define DATA_PIN 3
#define BRIGHTNESS 100

CRGB COLOR_RSG_BLUE = CRGB::Green;
CRGB COLOR_RSG_WHITE = CRGB::Red;
CRGB COLOR_RSG_GREEN = CRGB::Blue;
CRGB COLOR_RSG_PINK = CRGB::Violet;

pt ptBlink;

CRGB leds[NUM_LED];

CRGBSet group1(leds, 8, 9);
CRGBSet group2(leds, 6, 7);
CRGBSet group3(leds, 4, 5);
CRGBSet group4(leds, 2, 3);
CRGBSet group5(leds, 0, 1);

void LED_init();
void LED_groupOn(CRGBSet *group, const CRGB *color = &COLOR_RSG_BLUE);
void LED_groupOff(CRGBSet *group);
int LED_spin();
void LED_breath();
unsigned long startMillis = 0;

// MIDI header
SoftwareSerial midiSerial(5, 4); // RX, TX
MIDI_CREATE_INSTANCE(SoftwareSerial, midiSerial, MIDI);

// MIDI cpp

void handleNoteOn(byte channel, byte note, byte velocity);
void handleNoteOff(byte channel, byte note, byte velocity);
void handleError(int8_t error);

void MIDI_init() {
    // Create and bind the MIDI interface to the default hardware Serial port
    midiSerial.begin(31250); // MIDI Baudrate für den SoftwareSerial Port
    MIDI.begin(1);
    MIDI.turnThruOff();

    MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleNoteOff(handleNoteOff);
    MIDI.setHandleError(handleError);
}

void MIDI_handle(){
    if (MIDI.read()) {
        FastLED.show();
    }
}

void handleNoteOn(byte channel, byte note, byte velocity) {
    //serialPrintf("NoteOn: Ch= %d, Note=%d, Velocity=%d", channel, note, velocity);
    switch (note) {
        case Note_C2:
            LED_groupOn(&group1, &COLOR_RSG_PINK);
        break;
        case Note_Cis2:
            LED_groupOn(&group2, &COLOR_RSG_WHITE);
        break;
        case Note_D2:
            LED_groupOn(&group3, &COLOR_RSG_GREEN);
        break;
        case Note_Dis2:
            LED_groupOn(&group4, &COLOR_RSG_PINK);
        break;
        case Note_E2:
            LED_groupOn(&group5);
        break;
        case Note_C3:
            PT_SCHEDULE(LED_spin());
        break;
        default:
            serialPrintf("unhandled NoteOn: Ch= %d, Note=%d, Velocity=%d", channel, note, velocity);
    }
}

void handleNoteOff(byte channel, byte note, byte velocity) {
    //serialPrintf("NoteOff: Ch= %d, Note=%d, Velocity=%d", channel, note, velocity);
    switch (note) {
        case Note_C2:
            LED_groupOff(&group1);
        break;
        case Note_Cis2:
            LED_groupOff(&group2);
        break;
        case Note_D2:
            LED_groupOff(&group3);
        break;
        case Note_Dis2:
            LED_groupOff(&group4);
        break;
        case Note_E2:
            LED_groupOff(&group5);
        break;
        default:
            serialPrintf("unhandled noteOff: Ch= %d, Note=%d, Velocity=%d", channel, note, velocity);
    }
}

void handleError(int8_t error) {
    serialPrintf("unhandled error %d", error);
};

void handleSystemExclusive(byte *array, unsigned size);

void handleTimeCodeQuarterFrame(byte data);

void handleSongPosition(unsigned int beats);

void handleSongSelect(byte songnumber);

void handleTuneRequest();



// LED

void LED_init() {
    // sanity check delay - allows reprogramming if accidently blowing power w/leds
    //delay(1000);
    FastLED.addLeds<LED_CHIP, DATA_PIN, COLOR_ORDER>(leds, NUM_LED); // GRB ordering is typical
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.clear(true);
}

void LED_groupOn(CRGBSet *group, const CRGB *color) {
    *group = *color;
}

void LED_groupOff(CRGBSet *group) {
    *group = CRGB::Black;
}

int LED_spin() {
    PT_BEGIN(pt);

    for (int i = 0; i < NUM_LED; i++) {
        leds[i] = COLOR_RSG_BLUE;
        FastLED.show();
        // Wait a little bit
        PT_SLEEP(30);
        leds[i] = CRGB::Black;
    }

    for (int i = NUM_LED; i >= 0; i--) {
        leds[i] = COLOR_RSG_BLUE;
        FastLED.show();
        // Wait a little bit
        PT_SLEEP(30);
        leds[i] = CRGB::Black;
    }

    PT_END(pt);
}

void LED_breath() {
    int hue = 0;
    int divisor = 30;
    int MIN_BRIGHTNESS = BRIGHTNESS / 10;
    int MAX_BRIGHTNESS = BRIGHTNESS;

    for (int i = 0; i < 500; i++) {
        float breathVal = (exp(sin(millis() / 5000.0 * PI)) - 0.36787944) * 108.0;
        breathVal = map(breathVal, 0, 255, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
        FastLED.setBrightness(breathVal);
        fill_rainbow(leds, NUM_LED, (hue++ / divisor));
        if (hue == (255 * divisor)) {
            hue = 0;
        }
        FastLED.show();
        delay(5);
    }
}


// INIT

void setup() {
    startMillis = millis();
    Serial.begin(115200);

    LED_init();
    MIDI_init();

    PT_INIT(&ptBlink);
}

void loop() {
    MIDI_handle();
    //maybeDisplayCriticalRam();
}



// LED



