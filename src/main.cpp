#include <FastLED.h>
#include <MIDI.h>
#include <SoftwareSerial.h>
#include "helper.h"
#include "midiConsts.h"

#define LED_CHIP WS2811
#define COLOR_ORDER BRG
#define NUM_LED 30
#define DATA_PIN 3
#define BRIGHTNESS 100

//LED stuff
CRGB leds[NUM_LED];
CRGB COLOR_RSG_BLUE = CRGB(83, 192, 219);

// Config for "R"
CRGBSet group1(leds, 18, 19);
CRGBSet group2(leds, 16, 17);
CRGBSet group3(leds, 14, 15);
CRGBSet group4(leds, 12, 13);
CRGBSet group5(leds, 10, 11);
CRGBSet group6(leds, 8, 9);
CRGBSet group7(leds, 6, 7);
CRGBSet group8(leds, 4, 5);
CRGBSet group9(leds, 2, 3);
CRGBSet group10(leds, 0, 1);
CRGBSet groupAll(leds, 0, 19);

SoftwareSerial midiSerial(5, 4); // RX, TX
MIDI_CREATE_INSTANCE(SoftwareSerial, midiSerial, MIDI);


void fastLEDSetup() {
    // sanity check delay - allows reprogramming if accidently blowing power w/leds
    //delay(1000);
    FastLED.addLeds<LED_CHIP, DATA_PIN, COLOR_ORDER>(leds, NUM_LED); // GRB ordering is typical
    FastLED.setBrightness(BRIGHTNESS);
    groupAll = CRGB::Black;
    FastLED.clear(true);
}

void groupOn(CRGBSet *group, const CRGB *color = &COLOR_RSG_BLUE) {
    *group = *color;
}

void groupOff(CRGBSet *group) {
    *group = CRGB::Black;
}

void spin() {
    for (int i = 0; i < NUM_LED; i++) {
        leds[i] = COLOR_RSG_BLUE;
        FastLED.show();
        // Wait a little bit
        delay(30);
        leds[i] = CRGB::Black;
    }

    for (int i = NUM_LED; i >= 0; i--) {
        leds[i] = COLOR_RSG_BLUE;
        FastLED.show();
        // Wait a little bit
        delay(30);
        leds[i] = CRGB::Black;
    }
}

void breath() {
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

void handleNoteOn(byte channel, byte note, byte velocity) {
    serialPrintf("NoteOn: Ch= %d, Note=%d, Velocity=%d", channel, note, velocity);
    switch (note) {
        case Note_C2:
            groupOn(&group1);
            break;
        case Note_Cis2:
            groupOn(&group2);
            break;
        case Note_D2:
            groupOn(&group3);
        break;
        case Note_Dis2:
            groupOn(&group4);
        break;
        case Note_E2:
            groupOn(&group5);
        break;
        case Note_F2:
            groupOn(&group6);
        break;
        case Note_Fis2:
            groupOn(&group7);
        break;
        case Note_G2:
            groupOn(&group8);
        break;
        case Note_Gis2:
            groupOn(&group9);
        break;
        case Note_A2:
            breath();
        break;
        default:
            serialPrintf("unhandled NoteOn: Ch= %d, Note=%d, Velocity=%d", channel, note, velocity);
    }
}

void handleNoteOff(byte channel, byte note, byte velocity) {
    serialPrintf("NoteOff: Ch= %d, Note=%d, Velocity=%d", channel, note, velocity);
    switch (note) {
        case Note_C2:
            groupOff(&group1);
            break;
        case Note_Cis2:
            groupOff(&group2);
            break;
        case Note_D2:
            groupOff(&group3);
        break;
        case Note_Dis2:
            groupOff(&group4);
        break;
        case Note_E2:
            groupOff(&group5);
        break;
        case Note_F2:
            groupOff(&group6);
        break;
        case Note_Fis2:
            groupOff(&group7);
        break;
        case Note_G2:
            groupOff(&group8);
        break;
        case Note_Gis2:
            groupOff(&group9);
        break;
        case Note_A2:
            groupOff(&group10);
        break;
        default:
            serialPrintf("unhandled noteOff: Ch= %d, Note=%d, Velocity=%d", channel, note, velocity);
    }
}


void midiSetup() {
    // Create and bind the MIDI interface to the default hardware Serial port
    midiSerial.begin(31250); // MIDI Baudrate für den SoftwareSerial Port
    MIDI.begin(1); // Listen to all incoming messages
    MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleNoteOff(handleNoteOff);
}

void setup() {
    Serial.begin(9600);
    fastLEDSetup();
    midiSetup();
}


// This function runs over and over, and is where you do the magic to light
// your LEDs.
void loop() {
    maybeDisplayCriticalRam();
    if(MIDI.read()) {
        FastLED.show();
    }
}
