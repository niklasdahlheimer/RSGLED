#include <FastLED.h>
#include <MIDI.h>
#include <SoftwareSerial.h>
#include "helper.h"
#include "midiConsts.h"
// LED header

SoftwareSerial logSerial(4, 5); // RX, TX

#define LED_CHIP WS2811
#define LED_COLOR_ORDER BRG
#define LED_NUM 10
#define LED_DATA_PIN 3
#define LED_BRIGHTNESS_MAX 100

CRGB COLOR_RSG_BLUE = CRGB::SkyBlue;
CRGB COLOR_RSG_WHITE = CRGB::White;
CRGB COLOR_RSG_GREEN = CRGB::Green;
CRGB COLOR_RSG_PINK = CRGB::Pink;
CRGB COLOR_RSG_RED = CRGB::Pink;
CRGB COLOR_RSG_Yellow = CRGB::Yellow;
CRGB COLOR_RSG_Purple = CRGB::Purple;

CRGB leds[LED_NUM];

CRGBSet group1(leds, 8, 9);
CRGBSet group2(leds, 6, 7);
CRGBSet group3(leds, 4, 5);
CRGBSet group4(leds, 2, 3);
CRGBSet group5(leds, 0, 1);
CRGBSet groupAll(leds, 0, 9);

unsigned long lastTriggerMillis = 0;

#define STROBE_NOTE Note_C3
#define STROBE_PERIOD  100   // Duration in milliseconds the LEDs stay ON
#define STROBE_ON_FACTOR 0.5 // Factor between 0 and 1 how long the LEDs stay ON
unsigned long strobeStartMillis = 0; // Store the last time the LEDs were updated

#define BREATH_NOTE Note_D3
#define BREATH_PERIOD_IN_MILLIS 5000
unsigned long breathStartMillis = 0;
double breathBrightnessFactor;

#define NOISE_NOTE Note_E3
#define NOISE_PERIOD_IN_MILLIS 10
double currentNoiseVal = 0.5;
unsigned long lastNoiseUpdateInMillis = 0;

#define RAINBOW_NOTE Note_F3
#define RAINBOW_PERIOD_IN_MILLIS 10
unsigned int startHue = 0;
unsigned long startRainbowMillis = 0;

bool noteOn[255];

void LED_init();

void LED_on(CRGBSet *group, const CRGB *color = &COLOR_RSG_BLUE);

void LED_off(CRGBSet *group);


unsigned long startMillis = 0;

// MIDI header
MIDI_CREATE_DEFAULT_INSTANCE();
#define MIDI_CHANNEL 1

// MIDI cpp

void handleNoteOn(byte channel, byte note, byte velocity);

void handleNoteOff(byte channel, byte note, byte velocity);

void handleError(int8_t error);

void MIDI_init() {
    // Create and bind the MIDI interface to the default hardware Serial port
    //midiSerial.begin(31250); // MIDI Baudrate für den SoftwareSerial Port
    MIDI.begin(MIDI_CHANNEL);
    MIDI.turnThruOff();

    MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleNoteOff(handleNoteOff);
    MIDI.setHandleError(handleError);

    memset(noteOn, 0, 255);
}

void handleNoteOn(byte channel, byte note, byte velocity) {
    serialPrintf(&logSerial, "NoteOn: %d %d %d", channel, note, velocity);
    noteOn[note] = true;

    switch (note) {
        case BREATH_NOTE:
            breathStartMillis = millis();
            break;
        case STROBE_NOTE:
            strobeStartMillis = millis();
            break;
        case RAINBOW_NOTE:
            startHue = random(0, 5000);
            startRainbowMillis = millis();
            break;
    }
}

void handleNoteOff(byte channel, byte note, byte velocity) {
    serialPrintf(&logSerial, "NoteOff: %d %d %d", channel, note, velocity);
    noteOn[note] = false;

    switch (note) {
        case BREATH_NOTE:
            breathStartMillis = 0;
            break;
        case STROBE_NOTE:
            strobeStartMillis = 0;
            break;
        case RAINBOW_NOTE:
            startRainbowMillis = 0;
            break;
    }
}

void handleError(int8_t error) {
    //serialPrintf(&logSerial,"unhandled error %d", error);
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
    FastLED.addLeds<LED_CHIP, LED_DATA_PIN, LED_COLOR_ORDER>(leds, LED_NUM); // GRB ordering is typical
    FastLED.setBrightness(LED_BRIGHTNESS_MAX);
    FastLED.clear(true);
}

void LED_on(CRGBSet *group, const CRGB *color) {
    *group = *color;
}

void LED_off(CRGBSet *group) {
    *group = CRGB::Black;
}

int getRectValue(unsigned long currentTime, unsigned long period, float onFactor) {
    unsigned long phase = currentTime % period;
    unsigned long onPeriod = period * onFactor;
    return phase < onPeriod ? 1 : 0;
}

void LED_strobe() {
    int state = getRectValue(strobeStartMillis - lastTriggerMillis, STROBE_PERIOD, STROBE_ON_FACTOR);
    if (state == 1) {
        LED_on(&groupAll, &COLOR_RSG_BLUE); // Turn all LEDs on to the strobe color
    } else {
        FastLED.clear(); // Turn all LEDs off
    }
}

void LED_breath() {
    const unsigned int millisOfCurrentSecond = (lastTriggerMillis - breathStartMillis);

    breathBrightnessFactor = 0.5 * (1 + sin(2 * M_PI * millisOfCurrentSecond / BREATH_PERIOD_IN_MILLIS - M_PI / 2));
    FastLED.setBrightness(static_cast<uint8_t>(breathBrightnessFactor * LED_BRIGHTNESS_MAX));
    LED_on(&groupAll, &COLOR_RSG_BLUE); // Turn all LEDs on to the strobe color
}

void LED_noise() {
    if (lastTriggerMillis - lastNoiseUpdateInMillis > NOISE_PERIOD_IN_MILLIS) {
        lastNoiseUpdateInMillis = lastTriggerMillis;
        currentNoiseVal += (random(0, 10) - 5) * 0.01;
        currentNoiseVal = currentNoiseVal > 0.9 ? 0.9 : (currentNoiseVal < 0.1 ? 0.1 : currentNoiseVal);
    }
    FastLED.setBrightness(LED_BRIGHTNESS_MAX * currentNoiseVal);
    LED_on(&groupAll, &COLOR_RSG_BLUE); // Turn all LEDs on to the strobe color
}

void LED_rainbow() {
    fill_rainbow(leds, LED_NUM,
                 ((startRainbowMillis - lastTriggerMillis) / RAINBOW_PERIOD_IN_MILLIS) + startHue,
                 10);
}


void handleSingleNotes() {
    // Effects
    if (noteOn[STROBE_NOTE]) {
        LED_strobe();
    } else if (noteOn[BREATH_NOTE]) {
        LED_breath();
    } else if (noteOn[NOISE_NOTE]) {
        LED_noise();
    } else if (noteOn[RAINBOW_NOTE]) {
        LED_rainbow();
    }

    // Color for All On
    if (noteOn[Note_C1]) {
        LED_on(&groupAll, &COLOR_RSG_PINK);
    }
    if (noteOn[Note_D1]) {
        LED_on(&groupAll, &COLOR_RSG_Purple);
    }
    if (noteOn[Note_E1]) {
        LED_on(&groupAll, &COLOR_RSG_GREEN);
    }
    if (noteOn[Note_F1]) {
        LED_on(&groupAll, &COLOR_RSG_BLUE);
    }
    if (noteOn[Note_G1]) {
        LED_on(&groupAll, &COLOR_RSG_Yellow);
    }
    if (noteOn[Note_A1]) {
        LED_on(&groupAll, &COLOR_RSG_RED);
    }
    if (noteOn[Note_B1]) {
        LED_on(&groupAll, &COLOR_RSG_WHITE);
    }

    // Segments On
    if (noteOn[Note_C2]) {
        LED_on(&group1, &COLOR_RSG_PINK);
    }
    if (noteOn[Note_Cis2]) {
        LED_on(&group2, &COLOR_RSG_WHITE);
    }
    if (noteOn[Note_D2]) {
        LED_on(&group3, &COLOR_RSG_GREEN);
    }
    if (noteOn[Note_Dis2]) {
        LED_on(&group4, &COLOR_RSG_PINK);
    }
    if (noteOn[Note_E2]) {
        LED_on(&group5);
    }
}

unsigned long currentMillis;

void handleActions() {
    lastTriggerMillis = millis();

    // prime values
    FastLED.clear();
    FastLED.setBrightness(LED_BRIGHTNESS_MAX);

    handleSingleNotes();
    FastLED.show();
}


// INIT

void setup() {
    startMillis = millis();
    Serial.begin(115200);

    logSerial.begin(9600); // Software-Serial für den zweiten Arduino
    serialPrintf(&logSerial, "Hello from Arduino 1!");

    LED_init();
    MIDI_init();
}

void loop() {
    MIDI.read();
    handleActions();
    maybeDisplayCriticalRam(&logSerial);
}


// LED
