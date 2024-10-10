#include "ledController.h"

CRGB leds[LED_NUM];

// global vars
byte tempo = DEFAULT_TEMPO;
byte globalBrightness = LED_BRIGHTNESS_MAX;
CRGB *globalColor = &COLOR_1;
unsigned long timestamp = 0;

// FX vars
unsigned long strobeStartMillis = 0;

unsigned long breathStartMillis = 0;
double breathBrightnessFactor = 0.5;

double noiseCurrentVal = 0.5;
unsigned long noiseLastUpdateMillis = 0;

unsigned int rainbowStartHue = 0;
unsigned long rainbowStartMillis = 0;

unsigned long pumpStartMillis = 0;

unsigned long rotateStartMillis = 0;

unsigned long gradientWalkStartMillis = 0;

CRGB gradientLEDs[LED_NUM/2];

static CRGBSet g[] = {
    CRGBSet(leds, LED_GROUP_INDEX_1_START, LED_NUM - 1), // groupAll
    CRGBSet(leds, LED_GROUP_INDEX_1_START, LED_GROUP_INDEX_1_END),
    CRGBSet(leds, LED_GROUP_INDEX_2_START, LED_GROUP_INDEX_2_END),
    CRGBSet(leds, LED_GROUP_INDEX_3_START, LED_GROUP_INDEX_3_END),
    CRGBSet(leds, LED_GROUP_INDEX_4_START, LED_GROUP_INDEX_4_END),
    CRGBSet(leds, LED_GROUP_INDEX_5_START, LED_GROUP_INDEX_5_END),
    CRGBSet(leds, LED_GROUP_INDEX_6_START, LED_GROUP_INDEX_6_END),
    CRGBSet(leds, LED_GROUP_INDEX_7_START, LED_GROUP_INDEX_7_END),
    CRGBSet(leds, LED_GROUP_INDEX_8_START, LED_GROUP_INDEX_8_END),
    CRGBSet(leds, LED_GROUP_INDEX_9_START, LED_GROUP_INDEX_9_END),
    CRGBSet(leds, LED_GROUP_INDEX_10_START, LED_GROUP_INDEX_10_END)
};

static CRGBSet *level1[] = {&g[1], &g[10]};
static CRGBSet *level2[] = {&g[1], &g[10], &g[2], &g[9]};
static CRGBSet *level3[] = {&g[1], &g[10], &g[2], &g[9], &g[3], &g[8]};
static CRGBSet *level4[] = {&g[1], &g[10], &g[2], &g[9], &g[3], &g[8], &g[4], &g[7]};
static CRGBSet *level5[] = {
    &g[1], &g[10], &g[2], &g[9], &g[3], &g[8], &g[4], &g[7], &g[5], &g[6]
};

static CRGB groupColor[] = {
    *globalColor, *globalColor, *globalColor,
    *globalColor, *globalColor, *globalColor,
    *globalColor, *globalColor, *globalColor,
    *globalColor, *globalColor
};

// Forward declarations
static unsigned int
getBeatLenInMillis(unsigned int tempo, unsigned int div = 4, boolean isTrip = false, boolean isDot = false);

void maybeSetGroupColor(const byte *note);

void maybeSetGlobalBrightness(const byte *brightnessTrimValue);

void maybeSetTempo(const byte *tempoValue);

static void LED_on(CRGBSet *group, const CRGB *color = globalColor, byte brightness = 255);

static void
LED_on(CRGBSet *groupArray[], size_t size, const CRGB *color = globalColor, byte brightness = 255); // For Arrays

//FX
void maybeSetEffectStartTime(byte noteValue, unsigned long *startTimeRef, const unsigned long *curr);

void LED_FX_strobe(byte velo);

void LED_FX_breath(byte velo);

void LED_FX_noise(byte velo);

void LED_FX_rainbow(byte velo);

void LED_FX_levelPump(byte velo);

void LED_FX_rotate(byte velo);

void LED_FX_fill_gradient(byte velo, CRGB *color1, CRGB *color2);

// Definitions

void LEDC_init() {

    // sanity check delay - allows reprogramming if accidentally blowing power w/leds
    delay(2000);
    //FastLED.setMaxPowerInMilliWatts( 250*1000);
    FastLED.addLeds<LED_CHIP, LED_DATA_PIN, LED_COLOR_ORDER>(leds, LED_NUM); // GRB ordering is typical
    FastLED.clear(true);
}

void reset() {
    FastLED.clear();
    tempo = DEFAULT_TEMPO;
    globalBrightness = LED_BRIGHTNESS_MAX;
    timestamp = 0;
    strobeStartMillis = 0;
    breathStartMillis = 0;
    breathBrightnessFactor = 0.5;
    noiseCurrentVal = 0.5;
    noiseLastUpdateMillis = 0;
    rainbowStartHue = 0;
    rainbowStartMillis = 0;
    pumpStartMillis = 0;
    rotateStartMillis = 0;
    globalColor = &COLOR_1;
    for (int i = 0; i < 10; i++) {
        groupColor[i] = *globalColor;
    }
}

void LEDC_updateStripe(const byte *note, const byte *controller) {
    // fix time reference for all calculations
    timestamp = millis();

    if (note[TOTAL_RESET]) {
        reset();
    }

    maybeSetGroupColor(note);
    maybeSetGlobalBrightness(&note[GLOBAL_BRIGHTNESS_TRIM]);
    maybeSetTempo(&note[TEMPO]);

    FastLED.clear();
    FastLED.setBrightness(globalBrightness);

    maybeSetEffectStartTime(note[BREATH], &breathStartMillis, &timestamp);
    maybeSetEffectStartTime(note[STROBE], &strobeStartMillis, &timestamp);
    maybeSetEffectStartTime(note[RAINBOW], &rainbowStartMillis, &timestamp);
    maybeSetEffectStartTime(note[PUMP], &pumpStartMillis, &timestamp);
    maybeSetEffectStartTime(note[ROTATE], &rotateStartMillis, &timestamp);
    maybeSetEffectStartTime(note[GRADIENT], &gradientWalkStartMillis, &timestamp);

    // Global Color Switch
    if (note[GLOBAL_COLOR_1]) {
        globalColor = &COLOR_1;
    }
    if (note[GLOBAL_COLOR_2]) {
        globalColor = &COLOR_2;
    }
    if (note[GLOBAL_COLOR_3]) {
        globalColor = &COLOR_3;
    }
    if (note[GLOBAL_COLOR_4]) {
        globalColor = &COLOR_4;
    }
    if (note[GLOBAL_COLOR_5]) {
        globalColor = &COLOR_5;
    }
    if (note[GLOBAL_COLOR_6]) {
        globalColor = &COLOR_6;
    }
    if (note[GLOBAL_COLOR_7]) {
        globalColor = &COLOR_7;
    }
    if (note[GLOBAL_COLOR_8]) {
        globalColor = &COLOR_8;
    }
    if (note[GLOBAL_COLOR_9]) {
        globalColor = &COLOR_9;
    }
    if (note[GLOBAL_COLOR_10]) {
        globalColor = &COLOR_10;
    }
    if (note[GLOBAL_COLOR_11]) {
        globalColor = &COLOR_11;
    }
    if (note[GLOBAL_COLOR_12]) {
        globalColor = &COLOR_12;
    }

    // Effects
    if (note[STROBE]) {
        LED_FX_strobe(note[STROBE]);
    } else if (note[BREATH]) {
        LED_FX_breath(note[BREATH]);
    } else if (note[NOISE]) {
        LED_FX_noise(note[NOISE]);
    } else if (note[RAINBOW]) {
        LED_FX_rainbow(note[RAINBOW]);
    } else if (note[PUMP]) {
        LED_FX_levelPump(note[PUMP]);
    } else if (note[ROTATE]) {
        LED_FX_rotate(note[ROTATE]);
    } else if (note[GRADIENT]) {
        LED_FX_fill_gradient(note[GRADIENT], &COLOR_1, &COLOR_6);
    }

    // All On
    if (note[ALL_ON_COLOR_1]) {
        LED_on(&g[0], &COLOR_1, note[ALL_ON_COLOR_1]);
    }
    if (note[ALL_ON_COLOR_2]) {
        LED_on(&g[0], &COLOR_2, note[ALL_ON_COLOR_2]);
    }
    if (note[ALL_ON_COLOR_3]) {
        LED_on(&g[0], &COLOR_3, note[ALL_ON_COLOR_3]);
    }
    if (note[ALL_ON_COLOR_4]) {
        LED_on(&g[0], &COLOR_4, note[ALL_ON_COLOR_4]);
    }
    if (note[ALL_ON_COLOR_5]) {
        LED_on(&g[0], &COLOR_5, note[ALL_ON_COLOR_5]);
    }
    if (note[ALL_ON_COLOR_6]) {
        LED_on(&g[0], &COLOR_6, note[ALL_ON_COLOR_6]);
    }
    if (note[ALL_ON_COLOR_7]) {
        LED_on(&g[0], &COLOR_7, note[ALL_ON_COLOR_7]);
    }
    if (note[ALL_ON_COLOR_8]) {
        LED_on(&g[0], &COLOR_8, note[ALL_ON_COLOR_8]);
    }
    if (note[ALL_ON_COLOR_9]) {
        LED_on(&g[0], &COLOR_9, note[ALL_ON_COLOR_9]);
    }
    if (note[ALL_ON_COLOR_10]) {
        LED_on(&g[0], &COLOR_10, note[ALL_ON_COLOR_10]);
    }
    if (note[ALL_ON_COLOR_11]) {
        LED_on(&g[0], &COLOR_11, note[ALL_ON_COLOR_11]);
    }
    if (note[ALL_ON_COLOR_12]) {
        LED_on(&g[0], &COLOR_12, note[ALL_ON_COLOR_12]);
    }

    // Groups/Segments On
    if (note[GROUP_ON_1]) {
        LED_on(&g[1], &groupColor[1], note[GROUP_ON_1]);
    }
    if (note[GROUP_ON_2]) {
        LED_on(&g[2], &groupColor[2], note[GROUP_ON_2]);
    }
    if (note[GROUP_ON_3]) {
        LED_on(&g[3], &groupColor[3], note[GROUP_ON_3]);
    }
    if (note[GROUP_ON_4]) {
        LED_on(&g[4], &groupColor[4], note[GROUP_ON_4]);
    }
    if (note[GROUP_ON_5]) {
        LED_on(&g[5], &groupColor[5], note[GROUP_ON_5]);
    }
    if (note[GROUP_ON_6]) {
        LED_on(&g[6], &groupColor[6], note[GROUP_ON_6]);
    }
    if (note[GROUP_ON_7]) {
        LED_on(&g[7], &groupColor[7], note[GROUP_ON_7]);
    }
    if (note[GROUP_ON_8]) {
        LED_on(&g[8], &groupColor[8], note[GROUP_ON_8]);
    }
    if (note[GROUP_ON_9]) {
        LED_on(&g[9], &groupColor[9], note[GROUP_ON_9]);
    }
    if (note[GROUP_ON_10]) {
        LED_on(&g[10], &groupColor[10], note[GROUP_ON_10]);
    }

    // Horizontal Segment Blocks (for Level Meter etc.)
    if (note[LEVEL_ON_1]) {
        LED_on(level1, sizeof(level1) / sizeof(CRGBSet *), globalColor, note[LEVEL_ON_1]);
    }
    if (note[LEVEL_ON_2]) {
        LED_on(level2, sizeof(level2) / sizeof(CRGBSet *), globalColor, note[LEVEL_ON_2]);
    }
    if (note[LEVEL_ON_3]) {
        LED_on(level3, sizeof(level3) / sizeof(CRGBSet *), globalColor, note[LEVEL_ON_3]);
    }
    if (note[LEVEL_ON_4]) {
        LED_on(level4, sizeof(level4) / sizeof(CRGBSet *), globalColor, note[LEVEL_ON_4]);
    }
    if (note[LEVEL_ON_5]) {
        LED_on(level5, sizeof(level5) / sizeof(CRGBSet *), globalColor, note[LEVEL_ON_5]);
    }

    // Finally push all changes to Stripe
    FastLED.show();
}

void maybeSetGroupColor(const byte *note) {
    if (note[GROUP_HUE_1]) {
        groupColor[1] = CHSV(note[GROUP_HUE_1], 255 - note[GROUP_COLOR_TRIM_SATURATION],
                             255 - note[GROUP_COLOR_TRIM_BRIGHTNESS]);
    }
    if (note[GROUP_HUE_2]) {
        groupColor[2] = CHSV(note[GROUP_HUE_2], 255 - note[GROUP_COLOR_TRIM_SATURATION],
                             255 - note[GROUP_COLOR_TRIM_BRIGHTNESS]);
    }
    if (note[GROUP_HUE_3]) {
        groupColor[3] = CHSV(note[GROUP_HUE_3], 255 - note[GROUP_COLOR_TRIM_SATURATION],
                             255 - note[GROUP_COLOR_TRIM_BRIGHTNESS]);
    }
    if (note[GROUP_HUE_4]) {
        groupColor[4] = CHSV(note[GROUP_HUE_4], 255 - note[GROUP_COLOR_TRIM_SATURATION],
                             255 - note[GROUP_COLOR_TRIM_BRIGHTNESS]);
    }
    if (note[GROUP_HUE_5]) {
        groupColor[5] = CHSV(note[GROUP_HUE_5], 255 - note[GROUP_COLOR_TRIM_SATURATION],
                             255 - note[GROUP_COLOR_TRIM_BRIGHTNESS]);
    }
    if (note[GROUP_HUE_6]) {
        groupColor[6] = CHSV(note[GROUP_HUE_6], 255 - note[GROUP_COLOR_TRIM_SATURATION],
                             255 - note[GROUP_COLOR_TRIM_BRIGHTNESS]);
    }
    if (note[GROUP_HUE_7]) {
        groupColor[7] = CHSV(note[GROUP_HUE_7], 255 - note[GROUP_COLOR_TRIM_SATURATION],
                             255 - note[GROUP_COLOR_TRIM_BRIGHTNESS]);
    }
    if (note[GROUP_HUE_8]) {
        groupColor[8] = CHSV(note[GROUP_HUE_8], 255 - note[GROUP_COLOR_TRIM_SATURATION],
                             255 - note[GROUP_COLOR_TRIM_BRIGHTNESS]);
    }
    if (note[GROUP_HUE_9]) {
        groupColor[9] = CHSV(note[GROUP_HUE_9], 255 - note[GROUP_COLOR_TRIM_SATURATION],
                             255 - note[GROUP_COLOR_TRIM_BRIGHTNESS]);
    }
    if (note[GROUP_HUE_10]) {
        groupColor[10] = CHSV(note[GROUP_HUE_10], 255 - note[GROUP_COLOR_TRIM_SATURATION],
                              255 - note[GROUP_COLOR_TRIM_BRIGHTNESS]);
    }
}

// PRIVATE

// LED control helpers

static void LED_on(CRGBSet *groupArray[], size_t size, const CRGB *color, byte brightness) {
    for (unsigned int i = 0; i < size; i++) {
        LED_on(groupArray[i], color, brightness);
    }
}

static void LED_on(CRGBSet *group, const CRGB *color, byte brightness) {
    *group = *color;
    *group->nscale8_video(brightness);
}

// timing helpers

static int getRectValue(const unsigned long currentTime, const unsigned int period, const float onFactor) {
    const unsigned long phase = currentTime % period;
    const unsigned long onPeriod = period * onFactor;
    return phase < onPeriod ? 1 : 0;
}

static unsigned long getSteppedValue(const unsigned long currentTime, const unsigned int period) {
    return (currentTime / period);
}

/**
 * Returns periodic stepped value from 0 to numOfSteps-1
 */
static unsigned long getSteppedSawValue(const unsigned long currentTime, const unsigned int period,
                                        const unsigned int numOfSteps) {
    return getSteppedValue(currentTime, period) % numOfSteps;
}

static unsigned int
getBeatLenInMillis(unsigned int tempo, unsigned int div, boolean isTrip,
                   boolean isDot) {
    double beatLengthInMillis = 60000.0 / tempo;
    beatLengthInMillis *= 4.0 / div; // a quarter is equivalent to 1 "beat" for simplicity

    if (isTrip) {
        beatLengthInMillis *= 2.0 / 3.0;
    }
    if (isDot) {
        beatLengthInMillis *= 1.5;
    }

    return static_cast<unsigned int>(beatLengthInMillis);
}

// Global param setters

void maybeSetGlobalBrightness(const byte *brightnessTrimValue) {
    if (*brightnessTrimValue == 0 && globalBrightness == LED_BRIGHTNESS_MAX) {
        return;
    }
    globalBrightness = LED_BRIGHTNESS_MAX - *brightnessTrimValue * 2;
}

void maybeSetTempo(const byte *tempoValue) {
    // ignore null value or equal tempo
    if (*tempoValue == 0 || *tempoValue + TEMPO_OFFSET == tempo) {
        return;
    }
    tempo = *tempoValue + TEMPO_OFFSET;
}

// Effects

static void maybeSetEffectStartTime(const byte noteValue, unsigned long *startTimeRef, const unsigned long *curr) {
    if (noteValue && *startTimeRef == 0) {
        *startTimeRef = *curr;
    } else if (!noteValue && *startTimeRef != 0) {
        *startTimeRef = 0;
    }
}

void LED_FX_strobe(byte velo) {
    int state = getRectValue(timestamp - strobeStartMillis, getBeatLenInMillis(tempo, 16), STROBE_ON_FACTOR);
    if (state == 1) {
        LED_on(&g[0], globalColor, velo); // Turn all LEDs on to the strobe color
    } else {
        FastLED.clear(); // Turn all LEDs off
    }
}

void LED_FX_breath(byte velo) {
    double timeFactor = (1 +
                         sin(2 * M_PI * (double) (timestamp - breathStartMillis) / BREATH_PERIOD_IN_MILLIS - M_PI / 2));
    double currBrightness = (velo / 127.0) * timeFactor * LED_BRIGHTNESS_MAX;
    LED_on(&g[0], globalColor, (byte) currBrightness); // Turn all LEDs on to the strobe color
}

void LED_FX_noise(byte velo) {
    if (timestamp - noiseLastUpdateMillis > NOISE_PERIOD_IN_MILLIS) {
        noiseLastUpdateMillis = timestamp;
        noiseCurrentVal += ((double) random(0, 10) - 5) * 0.01;
        noiseCurrentVal = noiseCurrentVal > 0.9 ? 0.9 : (noiseCurrentVal < 0.1 ? 0.1 : noiseCurrentVal);
    }
    LED_on(&g[0], globalColor,
           LED_BRIGHTNESS_MAX * noiseCurrentVal * (velo / 127.0)); // Turn all LEDs on to the strobe color
}

void LED_FX_rainbow(byte velo) {
    fill_rainbow(leds, LED_NUM,
                 ((timestamp - rainbowStartMillis) / RAINBOW_PERIOD_IN_MILLIS) + rainbowStartHue,
                 (uint8_t) (10 * (velo / 127.0)));
}

void LED_FX_levelPump(byte velo) {
    const unsigned int currentStep = getSteppedSawValue(timestamp - pumpStartMillis, PUMP_PERIOD_IN_MILLIS, 6);
    switch (currentStep) {
        case 0:
            break; // all off
        case 1:
            LED_on(level1, sizeof(level1) / sizeof(level1[0]), globalColor, velo);
            break;
        case 2:
            LED_on(level2, sizeof(level2) / sizeof(level2[0]), globalColor, velo);
            break;
        case 3:
            LED_on(level3, sizeof(level3) / sizeof(level3[0]), globalColor, velo);
            break;
        case 4:
            LED_on(level4, sizeof(level4) / sizeof(level4[0]), globalColor, velo);
            break;
        case 5:
            LED_on(level5, sizeof(level5) / sizeof(level5[0]), globalColor, velo);
            break;
        default:
            break;
    }
}

void LED_FX_rotate(byte velo) {
    const unsigned int currentStep = getSteppedSawValue(timestamp - pumpStartMillis,
                                                        getBeatLenInMillis(tempo, 16),
                                                        10);
    LED_on(&g[currentStep], globalColor, velo);
}

void LED_FX_fill_gradient(byte velo, CRGB *color1, CRGB *color2) {
    if (gradientWalkStartMillis == timestamp) {
        // fill preset array
        fill_gradient_RGB(gradientLEDs, LED_NUM/2, *color1, *color2);
    }
    const unsigned int step = getSteppedSawValue(timestamp - gradientWalkStartMillis,
                                                 getBeatLenInMillis(tempo, 32),
                                                 LED_NUM);
    // circling offset
    for (int i = 0; i < LED_NUM; i++) {
        leds[(i + step) % LED_NUM] = gradientLEDs[i/2];
        g[0].nscale8_video(velo);
    }
}
