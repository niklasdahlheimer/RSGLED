#include "ledController.h"
#include "helper.h"

SoftwareSerial *ledLogSerial;

CRGB leds[LED_NUM];

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

static CRGBSet *gl1[] = {&g[1], &g[10]};
static CRGBSet *gl2[] = {&g[1], &g[10], &g[2], &g[9]};
static CRGBSet *gl3[] = {&g[1], &g[10], &g[2], &g[9], &g[3], &g[8]};
static CRGBSet *gl4[] = {&g[1], &g[10], &g[2], &g[9], &g[3], &g[8], &g[4], &g[7]};
static CRGBSet *gl5[] = {
        &g[1], &g[10], &g[2], &g[9], &g[3], &g[8], &g[4], &g[7], &g[5], &g[6]
};

unsigned int tempo = DEFAULT_TEMPO;
unsigned int globalBrightness = LED_BRIGHTNESS_MAX;

unsigned long timestamp = 0;

unsigned long strobeStartMillis = 0;

unsigned long breathStartMillis = 0;
double breathBrightnessFactor = 0.5;

double noiseCurrentVal = 0.5;
unsigned long noiseLastUpdateMillis = 0;

unsigned int rainbowStartHue = 0;
unsigned long rainbowStartMillis = 0;

unsigned long pumpStartMillis = 0;

CRGB *globalColor = &COLOR_9;

static unsigned int getBeatLengthInMillis(unsigned int tempo, unsigned int divider = 4, boolean isTriplet = false,
                                          boolean isDotted = false);

void maybeSetEffectStartTime(byte noteValue, unsigned long *startTimeRef, const unsigned long *curr) ;

void maybeSetGlobalBrightness(const byte *brightnessTrimValue);

void maybeSetTempo(const byte *tempoValue);

static void LED_on(CRGBSet *groupArray[], size_t size, byte brightness = 255); // For Arrays

static void LED_on(CRGBSet *group, const CRGB *color = &COLOR_1, byte brightness = 255);

void LED_FX_strobe();

void LED_FX_breath();

void LED_FX_noise();

void LED_FX_rainbow();

void LED_FX_levelPump();

void LEDC_init(SoftwareSerial *serial) {
    ledLogSerial = serial;

    // sanity check delay - allows reprogramming if accidently blowing power w/leds
    delay(2000);
    //FastLED.setMaxPowerInVoltsAndMilliamps( 12, MAX_POWER_MILLIAMPS);
    FastLED.addLeds<LED_CHIP, LED_DATA_PIN, LED_COLOR_ORDER>(leds, LED_NUM); // GRB ordering is typical
    FastLED.clear(true);
}

void LEDC_updateStripe(const byte *note, const byte *controller) {
    // fix time reference for all calculations
    timestamp = millis();

    // Controller
    maybeSetGlobalBrightness(&controller[CONTROLLER_INDEX_BRIGHTNESS_TRIM]);
    maybeSetTempo(&controller[CONTROLLER_INDEX_TEMPO]);

    FastLED.clear();
    FastLED.setBrightness(globalBrightness);

    maybeSetEffectStartTime(note[NOTE_BREATH], &breathStartMillis, &timestamp);
    maybeSetEffectStartTime(note[NOTE_STROBE], &strobeStartMillis, &timestamp);
    maybeSetEffectStartTime(note[NOTE_RAINBOW], &rainbowStartMillis, &timestamp);
    maybeSetEffectStartTime(note[NOTE_PUMP], &pumpStartMillis, &timestamp);

    // Global Color Switch
    if (note[NOTE_COLOR_SWITCH_1]) {
        globalColor = &COLOR_1;
    }
    if (note[NOTE_COLOR_SWITCH_2]) {
        globalColor = &COLOR_2;
    }
    if (note[NOTE_COLOR_SWITCH_3]) {
        globalColor = &COLOR_3;
    }
    if (note[NOTE_COLOR_SWITCH_4]) {
        globalColor = &COLOR_4;
    }
    if (note[NOTE_COLOR_SWITCH_5]) {
        globalColor = &COLOR_5;
    }
    if (note[NOTE_COLOR_SWITCH_6]) {
        globalColor = &COLOR_6;
    }
    if (note[NOTE_COLOR_SWITCH_7]) {
        globalColor = &COLOR_7;
    }
    if (note[NOTE_COLOR_SWITCH_8]) {
        globalColor = &COLOR_8;
    }
    if (note[NOTE_COLOR_SWITCH_9]) {
        globalColor = &COLOR_9;
    }
    if (note[NOTE_COLOR_SWITCH_10]) {
        globalColor = &COLOR_10;
    }
    if (note[NOTE_COLOR_SWITCH_11]) {
        globalColor = &COLOR_11;
    }
    if (note[NOTE_COLOR_SWITCH_12]) {
        globalColor = &COLOR_12;
    }


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

    // All On
    if (note[NOTE_ALL_COLOR_1]) {
        LED_on(&g[0], &COLOR_1, note[NOTE_ALL_COLOR_1]);
    }
    if (note[NOTE_ALL_COLOR_2]) {
        LED_on(&g[0], &COLOR_2, note[NOTE_ALL_COLOR_2]);
    }
    if (note[NOTE_ALL_COLOR_3]) {
        LED_on(&g[0], &COLOR_3, note[NOTE_ALL_COLOR_3]);
    }
    if (note[NOTE_ALL_COLOR_4]) {
        LED_on(&g[0], &COLOR_4, note[NOTE_ALL_COLOR_4]);
    }
    if (note[NOTE_ALL_COLOR_5]) {
        LED_on(&g[0], &COLOR_5, note[NOTE_ALL_COLOR_5]);
    }
    if (note[NOTE_ALL_COLOR_6]) {
        LED_on(&g[0], &COLOR_6, note[NOTE_ALL_COLOR_6]);
    }
    if (note[NOTE_ALL_COLOR_7]) {
        LED_on(&g[0], &COLOR_7, note[NOTE_ALL_COLOR_7]);
    }
    if (note[NOTE_ALL_COLOR_8]) {
        LED_on(&g[0], &COLOR_8, note[NOTE_ALL_COLOR_8]);
    }
    if (note[NOTE_ALL_COLOR_9]) {
        LED_on(&g[0], &COLOR_9, note[NOTE_ALL_COLOR_9]);
    }
    if (note[NOTE_ALL_COLOR_10]) {
        LED_on(&g[0], &COLOR_10, note[NOTE_ALL_COLOR_10]);
    }
    if (note[NOTE_ALL_COLOR_11]) {
        LED_on(&g[0], &COLOR_11, note[NOTE_ALL_COLOR_11]);
    }
    if (note[NOTE_ALL_COLOR_12]) {
        LED_on(&g[0], &COLOR_12, note[NOTE_ALL_COLOR_12]);
    }

    // Segments On
    if (note[NOTE_GROUP_1]) {
        LED_on(&g[1], globalColor, note[NOTE_GROUP_1]);
    }
    if (note[NOTE_GROUP_2]) {
        LED_on(&g[2], globalColor, note[NOTE_GROUP_2]);
    }
    if (note[NOTE_GROUP_3]) {
        LED_on(&g[3], globalColor, note[NOTE_GROUP_3]);
    }
    if (note[NOTE_GROUP_4]) {
        LED_on(&g[4], globalColor, note[NOTE_GROUP_4]);
    }
    if (note[NOTE_GROUP_5]) {
        LED_on(&g[5], globalColor, note[NOTE_GROUP_5]);
    }
    if (note[NOTE_GROUP_6]) {
        LED_on(&g[6], globalColor, note[NOTE_GROUP_6]);
    }
    if (note[NOTE_GROUP_7]) {
        LED_on(&g[7], globalColor, note[NOTE_GROUP_7]);
    }
    if (note[NOTE_GROUP_8]) {
        LED_on(&g[8], globalColor, note[NOTE_GROUP_8]);
    }
    if (note[NOTE_GROUP_9]) {
        LED_on(&g[9], globalColor, note[NOTE_GROUP_9]);
    }
    if (note[NOTE_GROUP_10]) {
        LED_on(&g[10], globalColor, note[NOTE_GROUP_10]);
    }

    // Horizontal Segment Blocks (for Level Meter etc.)
    if (note[NOTE_LEVEL_1]) {
        LED_on(gl1, sizeof(gl1) / sizeof(CRGBSet *), note[NOTE_LEVEL_1]);
    }
    if (note[NOTE_LEVEL_2]) {
        LED_on(gl2, sizeof(gl2) / sizeof(CRGBSet *), note[NOTE_LEVEL_2]);
    }
    if (note[NOTE_LEVEL_3]) {
        LED_on(gl3, sizeof(gl3) / sizeof(CRGBSet *), note[NOTE_LEVEL_3]);
    }
    if (note[NOTE_LEVEL_4]) {
        LED_on(gl4, sizeof(gl4) / sizeof(CRGBSet *), note[NOTE_LEVEL_4]);
    }
    if (note[NOTE_LEVEL_5]) {
        LED_on(gl5, sizeof(gl5) / sizeof(CRGBSet *), note[NOTE_LEVEL_5]);
    }

    // Finally push all changes to Stripe
    FastLED.show();
}

// PRIVATE

// LED control helpers

static void LED_on(CRGBSet *groupArray[], size_t size, byte brightness) {
    for (unsigned int i = 0; i < size; i++) {
        LED_on(groupArray[i], globalColor, brightness);
    }
}

static void LED_on(CRGBSet *group, const CRGB *color, byte brightness) {
    *group = *color;
    *group->nscale8(brightness);
}

// timing helpers

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

static unsigned int
getBeatLengthInMillis(const unsigned int t, const unsigned int divider, const boolean isTriplet,
                      const boolean isDotted) {
    double beatLengthInMillis = 60000.0 / t;
    beatLengthInMillis *= 4.0 / divider; // a quarter is equivalent to 1 "beat" for simplicity

    if (isTriplet) {
        beatLengthInMillis *= 2.0 / 3.0;
    }
    if (isDotted) {
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

void maybeSetEffectStartTime(byte noteValue, unsigned long *startTimeRef, const unsigned long *curr) {
    bool noteOn = noteValue > 0;
    if (noteOn && *startTimeRef == 0) {
        *startTimeRef = *curr;
    } else if (!noteOn && *startTimeRef != 0) {
        *startTimeRef = 0;
    }
}

void LED_FX_strobe() {
    int state = getRectValue(timestamp - strobeStartMillis, getBeatLengthInMillis(tempo, 16), STROBE_ON_FACTOR);
    if (state == 1) {
        LED_on(&g[0], globalColor); // Turn all LEDs on to the strobe color
    } else {
        FastLED.clear(); // Turn all LEDs off
    }
}

void LED_FX_breath() {
    const unsigned int millisOfCurrentSecond = (timestamp - breathStartMillis);

    breathBrightnessFactor = 0.5 * (1 + sin(2 * M_PI * millisOfCurrentSecond / BREATH_PERIOD_IN_MILLIS - M_PI / 2));
    FastLED.setBrightness(static_cast<uint8_t>(breathBrightnessFactor * LED_BRIGHTNESS_MAX));
    LED_on(&g[0], globalColor); // Turn all LEDs on to the strobe color
}

void LED_FX_noise() {
    if (timestamp - noiseLastUpdateMillis > NOISE_PERIOD_IN_MILLIS) {
        noiseLastUpdateMillis = timestamp;
        noiseCurrentVal += (random(0, 10) - 5) * 0.01;
        noiseCurrentVal = noiseCurrentVal > 0.9 ? 0.9 : (noiseCurrentVal < 0.1 ? 0.1 : noiseCurrentVal);
    }
    FastLED.setBrightness(LED_BRIGHTNESS_MAX * noiseCurrentVal);
    LED_on(&g[0], globalColor); // Turn all LEDs on to the strobe color
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
            LED_on(gl1, sizeof(gl1) / sizeof(gl1[0]));
            break;
        case 2:
            LED_on(gl2, sizeof(gl2) / sizeof(gl2[0]));
            break;
        case 3:
            LED_on(gl3, sizeof(gl3) / sizeof(gl3[0]));
            break;
        case 4:
            LED_on(gl4, sizeof(gl4) / sizeof(gl4[0]));
            break;
        case 5:
            LED_on(gl5, sizeof(gl5) / sizeof(gl5[0]));
            break;
        default:
            break;
    }
}
