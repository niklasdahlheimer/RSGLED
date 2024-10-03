#include "ledController.h"
#include "helper.h"

SoftwareSerial *ledLogSerial;

CRGB leds[LED_NUM];

static CRGBSet g1 = CRGBSet(leds, LED_GROUP_INDEX_1_START, LED_GROUP_INDEX_1_END);
static CRGBSet g2 = CRGBSet(leds, LED_GROUP_INDEX_2_START, LED_GROUP_INDEX_2_END);
static CRGBSet g3 = CRGBSet(leds, LED_GROUP_INDEX_3_START, LED_GROUP_INDEX_3_END);
static CRGBSet g4 = CRGBSet(leds, LED_GROUP_INDEX_4_START, LED_GROUP_INDEX_4_END);
static CRGBSet g5 = CRGBSet(leds, LED_GROUP_INDEX_5_START, LED_GROUP_INDEX_5_END);
static CRGBSet g6 = CRGBSet(leds, LED_GROUP_INDEX_6_START, LED_GROUP_INDEX_6_END);
static CRGBSet g7 = CRGBSet(leds, LED_GROUP_INDEX_7_START, LED_GROUP_INDEX_7_END);
static CRGBSet g8 = CRGBSet(leds, LED_GROUP_INDEX_8_START, LED_GROUP_INDEX_8_END);
static CRGBSet g9 = CRGBSet(leds, LED_GROUP_INDEX_9_START, LED_GROUP_INDEX_9_END);
static CRGBSet g10 = CRGBSet(leds, LED_GROUP_INDEX_10_START, LED_GROUP_INDEX_10_END);
static CRGBSet groupAll = CRGBSet(leds, LED_GROUP_INDEX_1_START, LED_NUM - 1);

static CRGBSet *groupLevel1[] = {&g1, &g10};
static CRGBSet *groupLevel2[] = {&g1, &g10, &g2, &g9};
static CRGBSet *groupLevel3[] = {&g1, &g10, &g2, &g9, &g3, &g8};
static CRGBSet *groupLevel4[] = {&g1, &g10, &g2, &g9, &g3, &g8, &g4, &g7};
static CRGBSet *groupLevel5[] = {
        &g1, &g10, &g2, &g9, &g3, &g8, &g4, &g7, &g5, &g6
};

unsigned int tempo = 120;

unsigned long timestamp = 0;

unsigned long strobeStartMillis = 0;

unsigned long breathStartMillis = 0;
double breathBrightnessFactor = 0.5;

double noiseCurrentVal = 0.5;
unsigned long noiseLastUpdateMillis = 0;

unsigned int rainbowStartHue = 0;
unsigned long rainbowStartMillis = 0;

unsigned long pumpStartMillis = 0;

CRGB *selectedColor = &COLOR_9;

unsigned int getBeatLengthInMillis(unsigned int divider = 4, boolean isTriplet = false, boolean isDotted = false,
                                   unsigned int t = tempo);

void maybeSetEffectStartTime(boolean isNoteOn, unsigned long *startTimeRef, const unsigned long *curr);

void LED_arrayOn(CRGBSet *group[], size_t size);

void LED_on(CRGBSet *group, const CRGB *color = &COLOR_1);

void LED_off(CRGBSet *group);

void LED_FX_strobe();

void LED_FX_breath();

void LED_FX_noise();

void LED_FX_rainbow();

void LED_FX_levelPump();

void LEDC_init(SoftwareSerial *serial) {
    ledLogSerial = serial;

    // sanity check delay - allows reprogramming if accidentally blowing power w/leds
    //delay(1000);
    FastLED.addLeds<LED_CHIP, LED_DATA_PIN, LED_COLOR_ORDER>(leds, LED_NUM); // GRB ordering is typical
    FastLED.setBrightness(LED_BRIGHTNESS_MAX);
    FastLED.clear(true);
}

void LEDC_updateStripe(const bool *note, const unsigned long millis) {
    // fix time reference for all calculations
    timestamp = millis;

    FastLED.clear();
    FastLED.setBrightness(LED_BRIGHTNESS_MAX);

    maybeSetEffectStartTime(note[NOTE_BREATH], &breathStartMillis, &timestamp);
    maybeSetEffectStartTime(note[NOTE_STROBE], &strobeStartMillis, &timestamp);
    maybeSetEffectStartTime(note[NOTE_RAINBOW], &rainbowStartMillis, &timestamp);
    maybeSetEffectStartTime(note[NOTE_PUMP], &pumpStartMillis, &timestamp);

    // Effects
    if (note[NOTE_STROBE]) {
        LED_FX_strobe();
    } else if (note[NOTE_BREATH]) {
        LED_FX_breath();
    } else if (note[NOTE_NOISE]) {
        LED_FX_noise();
    } else if (note[NOTE_RAINBOW]) {
        LED_FX_rainbow();
    } else if (note[NOTE_PUMP]) {
        LED_FX_levelPump();
    }

    // Color for All On
    if (note[NOTE_ALL_COLOR_1]) {
        LED_on(&groupAll, &COLOR_1);
    }
    if (note[NOTE_ALL_COLOR_2]) {
        LED_on(&groupAll, &COLOR_2);
    }
    if (note[NOTE_ALL_COLOR_3]) {
        LED_on(&groupAll, &COLOR_3);
    }
    if (note[NOTE_ALL_COLOR_4]) {
        LED_on(&groupAll, &COLOR_4);
    }
    if (note[NOTE_ALL_COLOR_5]) {
        LED_on(&groupAll, &COLOR_5);
    }
    if (note[NOTE_ALL_COLOR_6]) {
        LED_on(&groupAll, &COLOR_6);
    }
    if (note[NOTE_ALL_COLOR_7]) {
        LED_on(&groupAll, &COLOR_7);
    }
    if (note[NOTE_ALL_COLOR_8]) {
        LED_on(&groupAll, &COLOR_8);
    }
    if (note[NOTE_ALL_COLOR_9]) {
        LED_on(&groupAll, &COLOR_9);
    }
    if (note[NOTE_ALL_COLOR_10]) {
        LED_on(&groupAll, &COLOR_10);
    }
    if (note[NOTE_ALL_COLOR_11]) {
        LED_on(&groupAll, &COLOR_11);
    }
    if (note[NOTE_ALL_COLOR_12]) {
        LED_on(&groupAll, &COLOR_12);
    }

    if (note[NOTE_COLOR_SWITCH_1]) {
        selectedColor = &COLOR_1;
    }
    if (note[NOTE_COLOR_SWITCH_2]) {
        selectedColor = &COLOR_2;
    }
    if (note[NOTE_COLOR_SWITCH_3]) {
        selectedColor = &COLOR_3;
    }
    if (note[NOTE_COLOR_SWITCH_4]) {
        selectedColor = &COLOR_4;
    }
    if (note[NOTE_COLOR_SWITCH_5]) {
        selectedColor = &COLOR_5;
    }
    if (note[NOTE_COLOR_SWITCH_6]) {
        selectedColor = &COLOR_6;
    }
    if (note[NOTE_COLOR_SWITCH_7]) {
        selectedColor = &COLOR_7;
    }
    if (note[NOTE_COLOR_SWITCH_8]) {
        selectedColor = &COLOR_8;
    }
    if (note[NOTE_COLOR_SWITCH_9]) {
        selectedColor = &COLOR_9;
    }
    if (note[NOTE_COLOR_SWITCH_10]) {
        selectedColor = &COLOR_10;
    }
    if (note[NOTE_COLOR_SWITCH_11]) {
        selectedColor = &COLOR_11;
    }
    if (note[NOTE_COLOR_SWITCH_12]) {
        selectedColor = &COLOR_12;
    }


    // Segments On
    if (note[NOTE_GROUP_1]) {
        LED_on(&g1, selectedColor);
    }
    if (note[NOTE_GROUP_2]) {
        LED_on(&g2, selectedColor);
    }
    if (note[NOTE_GROUP_3]) {
        LED_on(&g3, selectedColor);
    }
    if (note[NOTE_GROUP_4]) {
        LED_on(&g4, selectedColor);
    }
    if (note[NOTE_GROUP_5]) {
        LED_on(&g5, selectedColor);
    }
    if (note[NOTE_GROUP_6]) {
        LED_on(&g6, selectedColor);
    }
    if (note[NOTE_GROUP_7]) {
        LED_on(&g7, selectedColor);
    }
    if (note[NOTE_GROUP_8]) {
        LED_on(&g8, selectedColor);
    }
    if (note[NOTE_GROUP_9]) {
        LED_on(&g9, selectedColor);
    }
    if (note[NOTE_GROUP_10]) {
        LED_on(&g10, selectedColor);
    }

    // level Meter
    if (note[NOTE_LEVEL_1]) {
        LED_arrayOn(groupLevel1, sizeof(groupLevel1) / sizeof(CRGBSet *));
    }
    if (note[NOTE_LEVEL_2]) {
        LED_arrayOn(groupLevel2, sizeof(groupLevel2) / sizeof(CRGBSet *));
    }
    if (note[NOTE_LEVEL_3]) {
        LED_arrayOn(groupLevel3, sizeof(groupLevel3) / sizeof(CRGBSet *));
    }
    if (note[NOTE_LEVEL_4]) {
        LED_arrayOn(groupLevel4, sizeof(groupLevel4) / sizeof(CRGBSet *));
    }
    if (note[NOTE_LEVEL_5]) {
        LED_arrayOn(groupLevel5, sizeof(groupLevel5) / sizeof(CRGBSet *));
    }

    FastLED.show();
}

// private

void LED_arrayOn(CRGBSet *group[], size_t size) {
    for (unsigned int i = 0; i < size; i++) {
        LED_on(group[i], selectedColor);
    }
}

void LED_on(CRGBSet *group, const CRGB *color) {
    *group = *color;
}

void LED_off(CRGBSet *group) {
    *group = CRGB::Black;
}

int getRectValue(const unsigned long currentTime, const unsigned int period, const float onFactor) {
    const unsigned long phase = currentTime % period;
    const unsigned long onPeriod = period * onFactor;
    return phase < onPeriod ? 1 : 0;
}

unsigned long getSteppedValue(const unsigned long currentTime, const unsigned int period) {
    return (currentTime / period);
}

unsigned long getSteppedSawValue(const unsigned long currentTime, const unsigned int period,
                                 const unsigned int numOfSteps) {
    return (currentTime / period) % numOfSteps;
}

unsigned int
getBeatLengthInMillis(const unsigned int divider, const boolean isTriplet, const boolean isDotted, const unsigned int t) {
    double beatLengthInMillis = 60000.0 / tempo;
    beatLengthInMillis *= 4.0 / divider;

    if (isTriplet) {
        beatLengthInMillis *= 2.0 / 3.0;
    }
    if (isDotted) {
        beatLengthInMillis *= 1.5;
    }

    return static_cast<unsigned int>(beatLengthInMillis);
}


void maybeSetEffectStartTime(const boolean isNoteOn, unsigned long *startTimeRef, const unsigned long *curr) {
    if (isNoteOn && *startTimeRef == 0) {
        *startTimeRef = *curr;
    } else if (!isNoteOn && *startTimeRef != 0) {
        *startTimeRef = 0;
    }
}

// Effects

void LED_FX_strobe() {
    int state = getRectValue(timestamp - strobeStartMillis, getBeatLengthInMillis(16), STROBE_ON_FACTOR);
    if (state == 1) {
        LED_on(&groupAll, selectedColor); // Turn all LEDs on to the strobe color
    } else {
        FastLED.clear(); // Turn all LEDs off
    }
}

void LED_FX_breath() {
    const unsigned int millisOfCurrentSecond = (timestamp - breathStartMillis);

    breathBrightnessFactor = 0.5 * (1 + sin(2 * M_PI * millisOfCurrentSecond / BREATH_PERIOD_IN_MILLIS - M_PI / 2));
    FastLED.setBrightness(static_cast<uint8_t>(breathBrightnessFactor * LED_BRIGHTNESS_MAX));
    LED_on(&groupAll, selectedColor); // Turn all LEDs on to the strobe color
}

void LED_FX_noise() {
    if (timestamp - noiseLastUpdateMillis > NOISE_PERIOD_IN_MILLIS) {
        noiseLastUpdateMillis = timestamp;
        noiseCurrentVal += (random(0, 10) - 5) * 0.01;
        noiseCurrentVal = noiseCurrentVal > 0.9 ? 0.9 : (noiseCurrentVal < 0.1 ? 0.1 : noiseCurrentVal);
    }
    FastLED.setBrightness(LED_BRIGHTNESS_MAX * noiseCurrentVal);
    LED_on(&groupAll, selectedColor); // Turn all LEDs on to the strobe color
}

void LED_FX_rainbow() {
    fill_rainbow(leds, LED_NUM,
                 ((timestamp - rainbowStartMillis) / RAINBOW_PERIOD_IN_MILLIS) + rainbowStartHue,
                 10);
}

void LED_FX_levelPump() {
    const unsigned int currentStep = getSteppedSawValue(timestamp - pumpStartMillis, PUMP_PERIOD_IN_MILLIS, 6);
    switch (currentStep) {
        case 1:
            LED_arrayOn(groupLevel1, sizeof(groupLevel1) / sizeof(groupLevel1[0]));
            break;
        case 2:
            LED_arrayOn(groupLevel2, sizeof(groupLevel2) / sizeof(groupLevel2[0]));
            break;
        case 3:
            LED_arrayOn(groupLevel3, sizeof(groupLevel3) / sizeof(groupLevel3[0]));
            break;
        case 4:
            LED_arrayOn(groupLevel4, sizeof(groupLevel4) / sizeof(groupLevel4[0]));
            break;
        case 5:
            LED_arrayOn(groupLevel5, sizeof(groupLevel5) / sizeof(groupLevel5[0]));
            break;
        default:
            break;
    }
}
