#include "ledController.h"

typedef struct {
    int LED_NUM = MAX_LED_NUM;
    CRGB LEDs[MAX_LED_NUM]{};
    CRGB gradientLEDs[MAX_LED_NUM]{};
    CRGBSet *groups[11]{};
    CRGBSet **levels[5]{};
    byte levelsSize[5]{};
    CRGB groupColor[11]{};
} LEDConfig;

static LEDConfig ledConfig;

// global vars
static byte tempo = DEFAULT_TEMPO;
static byte globBrightness = LED_BRIGHTNESS_MAX;
static CRGB *globalColor = &COLOR_1;
static unsigned long timestamp = 0;

// palettes
static CRGBPalette16 palettes[] = {
    OceanColors_p, RainbowColors_p, PartyColors_p, HeatColors_p, LavaColors_p, CloudColors_p
};

// FX vars
static unsigned long strobeStartMillis = 0;
static unsigned long breathStartMillis = 0;
static double breathBrightnessFactor = 0.5;
static double noiseCurrentVal = 0.5;
static unsigned long noiseLastUpdateMillis = 0;
static unsigned int rainbowStartHue = 0;
static unsigned long rainbowStartMillis = 0;
static unsigned long pumpStartMillis = 0;
static unsigned long rotateStartMillis = 0;
static unsigned long gradientWalkStartMillis = 0;
static unsigned long paletteWalkStartMillis = 0;
static byte currentPalette = 0;

// Forward declarations

void reset();

static unsigned int
getBeatLenInMillis(unsigned int tempo, unsigned int div = 4, boolean isTrip = false, boolean isDot = false);

void maybeSetGroupColor(const byte *note, const byte *controller);

void maybeSetGlobalBrightness(const byte *brightnessTrimValue);

void maybeSetTempo(byte tempoValue);

static void LED_on(CRGBSet *group, const CRGB *color = globalColor, byte brightness = 255);

static void
LED_on(CRGBSet *groupArray[], size_t size, const CRGB *color = globalColor, byte brightness = 255); // For Arrays

//FX
static void maybeSetEffectStartTime(byte noteValue, unsigned long *startTimeRef, const unsigned long *curr,
                                    byte *increaseVal = nullptr);

void LED_FX_strobe(byte velo);

void LED_FX_breath(byte velo);

void LED_FX_noise(byte velo);

void LED_FX_rainbow(byte velo);

void LED_FX_levelPump(byte velo);

void LED_FX_rotate(byte velo);

void LED_FX_sparkle(byte velo);

void LED_FX_palette(byte velocity, const CRGBPalette16 *pal);

void LED_FX_fill_gradient(byte velo, CRGB *color1, CRGB *color2);

// Definitions

void LEDC_init(const Config *config) {
    ledConfig.LED_NUM = config->LED_NUM;

    ledConfig.groups[0] = new CRGBSet(ledConfig.LEDs, ledConfig.LED_NUM - 1);
    ledConfig.groups[1] = new CRGBSet(ledConfig.LEDs, config->LED_GROUP_INDEX_1_START, config->LED_GROUP_INDEX_1_END);
    ledConfig.groups[2] = new CRGBSet(ledConfig.LEDs, config->LED_GROUP_INDEX_2_START, config->LED_GROUP_INDEX_2_END);
    ledConfig.groups[3] = new CRGBSet(ledConfig.LEDs, config->LED_GROUP_INDEX_3_START, config->LED_GROUP_INDEX_3_END);
    ledConfig.groups[4] = new CRGBSet(ledConfig.LEDs, config->LED_GROUP_INDEX_4_START, config->LED_GROUP_INDEX_4_END);
    ledConfig.groups[5] = new CRGBSet(ledConfig.LEDs, config->LED_GROUP_INDEX_5_START, config->LED_GROUP_INDEX_5_END);
    ledConfig.groups[6] = new CRGBSet(ledConfig.LEDs, config->LED_GROUP_INDEX_6_START, config->LED_GROUP_INDEX_6_END);
    ledConfig.groups[7] = new CRGBSet(ledConfig.LEDs, config->LED_GROUP_INDEX_7_START, config->LED_GROUP_INDEX_7_END);
    ledConfig.groups[8] = new CRGBSet(ledConfig.LEDs, config->LED_GROUP_INDEX_8_START, config->LED_GROUP_INDEX_8_END);
    ledConfig.groups[9] = new CRGBSet(ledConfig.LEDs, config->LED_GROUP_INDEX_9_START, config->LED_GROUP_INDEX_9_END);
    ledConfig.groups[10] = new CRGBSet(ledConfig.LEDs, config->LED_GROUP_INDEX_10_START,
                                       config->LED_GROUP_INDEX_10_END);

    ledConfig.levels[0] = new CRGBSet *[2]{ledConfig.groups[1], ledConfig.groups[10]};
    ledConfig.levelsSize[0] = 2;
    ledConfig.levels[1] = new CRGBSet *[4]{
        ledConfig.groups[1], ledConfig.groups[10], ledConfig.groups[2], ledConfig.groups[9]
    };
    ledConfig.levelsSize[1] = 4;
    ledConfig.levels[2] = new CRGBSet *[6]{
        ledConfig.groups[1], ledConfig.groups[10], ledConfig.groups[2], ledConfig.groups[9], ledConfig.groups[3],
        ledConfig.groups[8]
    };
    ledConfig.levelsSize[2] = 6;
    ledConfig.levels[3] = new CRGBSet *[8]{
        ledConfig.groups[1], ledConfig.groups[10], ledConfig.groups[2], ledConfig.groups[9], ledConfig.groups[3],
        ledConfig.groups[8], ledConfig.groups[4], ledConfig.groups[7]
    };
    ledConfig.levelsSize[3] = 8;
    ledConfig.levels[4] = new CRGBSet *[10]{
        ledConfig.groups[1], ledConfig.groups[10], ledConfig.groups[2], ledConfig.groups[9], ledConfig.groups[3],
        ledConfig.groups[8], ledConfig.groups[4], ledConfig.groups[7], ledConfig.groups[5], ledConfig.groups[6]
    };
    ledConfig.levelsSize[4] = 10;

    //FastLED.setMaxPowerInMilliWatts( 250*1000);
    FastLED.addLeds<LED_CHIP, LED_DATA_PIN, LED_COLOR_ORDER>(ledConfig.LEDs, ledConfig.LED_NUM);
    // GRB ordering is typical
    reset();
}

void reset() {
    FastLED.clear(true);
    tempo = DEFAULT_TEMPO;
    globBrightness = LED_BRIGHTNESS_MAX;
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
    gradientWalkStartMillis = 0;
    paletteWalkStartMillis = 0;
    globalColor = &COLOR_1;
    for (auto &i: ledConfig.groupColor) {
        i = *globalColor;
    }
}

void LEDC_updateStripe(const byte *note, const byte *controller) {
    // fix time reference for all calculations
    timestamp = millis();

    if (note[TOTAL_RESET]) {
        reset();
    }

    maybeSetGroupColor(note, controller);
    maybeSetGlobalBrightness(&note[GLOBAL_BRIGHTNESS_TRIM]);
    maybeSetTempo(note[TEMPO] / 2);

    FastLED.clear();
    FastLED.setBrightness(globBrightness);

    maybeSetEffectStartTime(note[BREATH], &breathStartMillis, &timestamp);
    maybeSetEffectStartTime(note[STROBE], &strobeStartMillis, &timestamp);
    maybeSetEffectStartTime(note[RAINBOW], &rainbowStartMillis, &timestamp);
    maybeSetEffectStartTime(note[PUMP], &pumpStartMillis, &timestamp);
    maybeSetEffectStartTime(note[ROTATE], &rotateStartMillis, &timestamp);
    maybeSetEffectStartTime(note[GRADIENT], &gradientWalkStartMillis, &timestamp);
    maybeSetEffectStartTime(note[PALETTE], &paletteWalkStartMillis, &timestamp, &currentPalette);

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
    } else if (note[SPARKLE]) {
        LED_FX_sparkle(note[SPARKLE]);
    } else if (note[PALETTE]) {
        LED_FX_palette(note[PALETTE], &palettes[currentPalette % 6]);
    } else if (note[GRADIENT]) {
        LED_FX_fill_gradient(note[GRADIENT], &COLOR_1, &COLOR_6);
    }

    // All On
    if (note[ALL_ON_COLOR_1]) {
        LED_on(ledConfig.groups[0], &COLOR_1, note[ALL_ON_COLOR_1]);
    }
    if (note[ALL_ON_COLOR_2]) {
        LED_on(ledConfig.groups[0], &COLOR_2, note[ALL_ON_COLOR_2]);
    }
    if (note[ALL_ON_COLOR_3]) {
        LED_on(ledConfig.groups[0], &COLOR_3, note[ALL_ON_COLOR_3]);
    }
    if (note[ALL_ON_COLOR_4]) {
        LED_on(ledConfig.groups[0], &COLOR_4, note[ALL_ON_COLOR_4]);
    }
    if (note[ALL_ON_COLOR_5]) {
        LED_on(ledConfig.groups[0], &COLOR_5, note[ALL_ON_COLOR_5]);
    }
    if (note[ALL_ON_COLOR_6]) {
        LED_on(ledConfig.groups[0], &COLOR_6, note[ALL_ON_COLOR_6]);
    }
    if (note[ALL_ON_COLOR_7]) {
        LED_on(ledConfig.groups[0], &COLOR_7, note[ALL_ON_COLOR_7]);
    }
    if (note[ALL_ON_COLOR_8]) {
        LED_on(ledConfig.groups[0], &COLOR_8, note[ALL_ON_COLOR_8]);
    }
    if (note[ALL_ON_COLOR_9]) {
        LED_on(ledConfig.groups[0], &COLOR_9, note[ALL_ON_COLOR_9]);
    }
    if (note[ALL_ON_COLOR_10]) {
        LED_on(ledConfig.groups[0], &COLOR_10, note[ALL_ON_COLOR_10]);
    }
    if (note[ALL_ON_COLOR_11]) {
        LED_on(ledConfig.groups[0], &COLOR_11, note[ALL_ON_COLOR_11]);
    }
    if (note[ALL_ON_COLOR_12]) {
        LED_on(ledConfig.groups[0], &COLOR_12, note[ALL_ON_COLOR_12]);
    }

    // Groups/Segments On
    if (note[GROUP_ON_1]) {
        LED_on(ledConfig.groups[1], &ledConfig.groupColor[1], note[GROUP_ON_1]);
    }
    if (note[GROUP_ON_2]) {
        LED_on(ledConfig.groups[2], &ledConfig.groupColor[2], note[GROUP_ON_2]);
    }
    if (note[GROUP_ON_3]) {
        LED_on(ledConfig.groups[3], &ledConfig.groupColor[3], note[GROUP_ON_3]);
    }
    if (note[GROUP_ON_4]) {
        LED_on(ledConfig.groups[4], &ledConfig.groupColor[4], note[GROUP_ON_4]);
    }
    if (note[GROUP_ON_5]) {
        LED_on(ledConfig.groups[5], &ledConfig.groupColor[5], note[GROUP_ON_5]);
    }
    if (note[GROUP_ON_6]) {
        LED_on(ledConfig.groups[6], &ledConfig.groupColor[6], note[GROUP_ON_6]);
    }
    if (note[GROUP_ON_7]) {
        LED_on(ledConfig.groups[7], &ledConfig.groupColor[7], note[GROUP_ON_7]);
    }
    if (note[GROUP_ON_8]) {
        LED_on(ledConfig.groups[8], &ledConfig.groupColor[8], note[GROUP_ON_8]);
    }
    if (note[GROUP_ON_9]) {
        LED_on(ledConfig.groups[9], &ledConfig.groupColor[9], note[GROUP_ON_9]);
    }
    if (note[GROUP_ON_10]) {
        LED_on(ledConfig.groups[10], &ledConfig.groupColor[10], note[GROUP_ON_10]);
    }
    if (note[GROUP_ON_ALL]) {
        LED_on(ledConfig.groups[1], &ledConfig.groupColor[1], note[GROUP_ON_ALL]);
        LED_on(ledConfig.groups[2], &ledConfig.groupColor[2], note[GROUP_ON_ALL]);
        LED_on(ledConfig.groups[3], &ledConfig.groupColor[3], note[GROUP_ON_ALL]);
        LED_on(ledConfig.groups[4], &ledConfig.groupColor[4], note[GROUP_ON_ALL]);
        LED_on(ledConfig.groups[5], &ledConfig.groupColor[5], note[GROUP_ON_ALL]);
        LED_on(ledConfig.groups[6], &ledConfig.groupColor[6], note[GROUP_ON_ALL]);
        LED_on(ledConfig.groups[7], &ledConfig.groupColor[7], note[GROUP_ON_ALL]);
        LED_on(ledConfig.groups[8], &ledConfig.groupColor[8], note[GROUP_ON_ALL]);
        LED_on(ledConfig.groups[9], &ledConfig.groupColor[9], note[GROUP_ON_ALL]);
        LED_on(ledConfig.groups[10], &ledConfig.groupColor[10], note[GROUP_ON_ALL]);
    }

    // Horizontal Segment Blocks (for Level Meter etc.)
    if (note[LEVEL_ON_1]) {
        LED_on(ledConfig.levels[0], ledConfig.levelsSize[0], globalColor, note[LEVEL_ON_1]);
    }
    if (note[LEVEL_ON_2]) {
        LED_on(ledConfig.levels[1], ledConfig.levelsSize[1], globalColor, note[LEVEL_ON_2]);
    }
    if (note[LEVEL_ON_3]) {
        LED_on(ledConfig.levels[2], ledConfig.levelsSize[2], globalColor, note[LEVEL_ON_3]);
    }
    if (note[LEVEL_ON_4]) {
        LED_on(ledConfig.levels[3], ledConfig.levelsSize[3], globalColor, note[LEVEL_ON_4]);
    }
    if (note[LEVEL_ON_5]) {
        LED_on(ledConfig.levels[4], ledConfig.levelsSize[4], globalColor, note[LEVEL_ON_5]);
    }

    // Finally push all changes to Stripe
    FastLED.show();
}


void maybeSetGroupColor(const byte *note, const byte *controller) {
    if (note[GROUP_HUE_1]) {
        ledConfig.groupColor[1] = CHSV(note[GROUP_HUE_1], 255 - controller[CONTROLLER_HUE_GROUP],
                                       255 - controller[CONTROLLER_BRIGHTNESS_GROUP]);
    }
    if (note[GROUP_HUE_2]) {
        ledConfig.groupColor[2] = CHSV(note[GROUP_HUE_2], 255 - controller[CONTROLLER_HUE_GROUP],
                                       255 - controller[CONTROLLER_BRIGHTNESS_GROUP]);
    }
    if (note[GROUP_HUE_3]) {
        ledConfig.groupColor[3] = CHSV(note[GROUP_HUE_3], 255 - controller[CONTROLLER_HUE_GROUP],
                                       255 - controller[CONTROLLER_BRIGHTNESS_GROUP]);
    }
    if (note[GROUP_HUE_4]) {
        ledConfig.groupColor[4] = CHSV(note[GROUP_HUE_4], 255 - controller[CONTROLLER_HUE_GROUP],
                                       255 - controller[CONTROLLER_BRIGHTNESS_GROUP]);
    }
    if (note[GROUP_HUE_5]) {
        ledConfig.groupColor[5] = CHSV(note[GROUP_HUE_5], 255 - controller[CONTROLLER_HUE_GROUP],
                                       255 - controller[CONTROLLER_BRIGHTNESS_GROUP]);
    }
    if (note[GROUP_HUE_6]) {
        ledConfig.groupColor[6] = CHSV(note[GROUP_HUE_6], 255 - controller[CONTROLLER_HUE_GROUP],
                                       255 - controller[CONTROLLER_BRIGHTNESS_GROUP]);
    }
    if (note[GROUP_HUE_7]) {
        ledConfig.groupColor[7] = CHSV(note[GROUP_HUE_7], 255 - controller[CONTROLLER_HUE_GROUP],
                                       255 - controller[CONTROLLER_BRIGHTNESS_GROUP]);
    }
    if (note[GROUP_HUE_8]) {
        ledConfig.groupColor[8] = CHSV(note[GROUP_HUE_8], 255 - controller[CONTROLLER_HUE_GROUP],
                                       255 - controller[CONTROLLER_BRIGHTNESS_GROUP]);
    }
    if (note[GROUP_HUE_9]) {
        ledConfig.groupColor[9] = CHSV(note[GROUP_HUE_9], 255 - controller[CONTROLLER_HUE_GROUP],
                                       255 - controller[CONTROLLER_BRIGHTNESS_GROUP]);
    }
    if (note[GROUP_HUE_10]) {
        ledConfig.groupColor[10] = CHSV(note[GROUP_HUE_10], 255 - controller[CONTROLLER_HUE_GROUP],
                                        255 - controller[CONTROLLER_BRIGHTNESS_GROUP]);
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
    group->nscale8_video(brightness);
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
getBeatLenInMillis(unsigned int tempo, unsigned int div, boolean isTrip, boolean isDot) {
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
    if (*brightnessTrimValue == 0 && globBrightness == LED_BRIGHTNESS_MAX) {
        return;
    }
    globBrightness = LED_BRIGHTNESS_MAX - *brightnessTrimValue * 2;
    Serial.printf("set globBrightness to %d\n", globBrightness);
}

void maybeSetTempo(const byte tempoValue) {
    // ignore null value or equal tempo
    if (tempoValue == 0 || tempoValue + TEMPO_OFFSET == tempo) {
        return;
    }
    tempo = tempoValue + TEMPO_OFFSET;
    Serial.printf("set tempo to %d bpm (value was %d)\n", tempo, tempoValue);
}

// Effects

static void maybeSetEffectStartTime(const byte noteValue, unsigned long *startTimeRef, const unsigned long *curr,
                                    byte *increaseVal) {
    if (noteValue && *startTimeRef == 0) {
        *startTimeRef = *curr;
        if (increaseVal != nullptr) {
            (*increaseVal)++;
        }
    } else if (!noteValue && *startTimeRef != 0) {
        *startTimeRef = 0;
    }
}

void LED_FX_strobe(byte velo) {
    int state = getRectValue(timestamp - strobeStartMillis, getBeatLenInMillis(tempo, 16), STROBE_ON_FACTOR);
    if (state == 1) {
        LED_on(ledConfig.groups[0], globalColor, velo); // Turn all LEDs on to the strobe color
    } else {
        FastLED.clear(); // Turn all LEDs off
    }
}

void LED_FX_breath(byte velo) {
    double timeFactor = (1 +
                         sin(2 * M_PI * (double) (timestamp - breathStartMillis) / BREATH_PERIOD_IN_MILLIS - M_PI / 2));
    double currBrightness = (velo / 127.0) * timeFactor * LED_BRIGHTNESS_MAX;
    LED_on(ledConfig.groups[0], globalColor, (byte) currBrightness); // Turn all LEDs on to the strobe color
}

void LED_FX_noise(byte velo) {
    if (timestamp - noiseLastUpdateMillis > NOISE_PERIOD_IN_MILLIS) {
        noiseLastUpdateMillis = timestamp;
        noiseCurrentVal += ((double) random(0, 10) - 5) * 0.01;
        noiseCurrentVal = noiseCurrentVal > 0.9 ? 0.9 : (noiseCurrentVal < 0.1 ? 0.1 : noiseCurrentVal);
    }
    LED_on(ledConfig.groups[0], globalColor,
           LED_BRIGHTNESS_MAX * noiseCurrentVal * (velo / 127.0)); // Turn all LEDs on to the strobe color
}

void LED_FX_rainbow(byte velo) {
    fill_rainbow(ledConfig.LEDs, ledConfig.LED_NUM,
                 ((timestamp - rainbowStartMillis) / RAINBOW_PERIOD_IN_MILLIS) + rainbowStartHue,
                 (uint8_t) (10 * (velo / 127.0)));
}

void LED_FX_levelPump(byte velo) {
    const unsigned int currentStep = getSteppedSawValue(timestamp - pumpStartMillis, PUMP_PERIOD_IN_MILLIS, 6);
    switch (currentStep) {
        case 0:
            break; // all off
        case 1:
            LED_on(ledConfig.levels[0], ledConfig.levelsSize[0], globalColor, velo);
            break;
        case 2:
            LED_on(ledConfig.levels[1], ledConfig.levelsSize[1], globalColor, velo);
            break;
        case 3:
            LED_on(ledConfig.levels[2], ledConfig.levelsSize[2], globalColor, velo);
            break;
        case 4:
            LED_on(ledConfig.levels[3], ledConfig.levelsSize[3], globalColor, velo);
            break;
        case 5:
            LED_on(ledConfig.levels[4], ledConfig.levelsSize[4], globalColor, velo);
            break;
        default:
            break;
    }
}

void LED_FX_rotate(byte velo) {
    const unsigned int currentStep = getSteppedSawValue(timestamp - pumpStartMillis,
                                                        getBeatLenInMillis(tempo, 16),
                                                        10);
    LED_on(ledConfig.groups[currentStep], globalColor, velo);
}

void LED_FX_fill_gradient(byte velo, CRGB *color1, CRGB *color2) {
    if (gradientWalkStartMillis == timestamp) {
        // fill preset array
        fill_gradient_RGB(ledConfig.gradientLEDs, ledConfig.LED_NUM, *color1, *color2);
    }
    const unsigned int step = getSteppedSawValue(timestamp - gradientWalkStartMillis,
                                                 getBeatLenInMillis(tempo, 64),
                                                 ledConfig.LED_NUM);
    // circling offset
    for (int i = 0; i < ledConfig.LED_NUM; i++) {
        ledConfig.LEDs[(i + step) % ledConfig.LED_NUM] = ledConfig.gradientLEDs[i];
        ledConfig.groups[0]->nscale8_video(velo);
    }
}

void LED_FX_palette(byte velocity, const CRGBPalette16 *pal) {
    const unsigned int step = getSteppedSawValue(timestamp - paletteWalkStartMillis,
                                                 getBeatLenInMillis(tempo, 64),
                                                 ledConfig.LED_NUM);
    // circling offset
    for (int i = 0; i < ledConfig.LED_NUM; i++) {
        byte paletteIndex = map(i, 0, ledConfig.LED_NUM, 0, 255);
        ledConfig.LEDs[(i + step) % ledConfig.LED_NUM] = ColorFromPalette(*pal, paletteIndex);
        ledConfig.groups[0]->nscale8_video(velocity);
    }
}

void LED_FX_sparkle(byte velocity) {
    // Define the density and brightness of the sparkles based on the velocity
    byte density = map(velocity, 0, 255, 1, 20); // Adjust sparkle density with velocity
    byte sparkleBrightness = velocity; // Adjust brightness with velocity

    // Iterate through the LEDs and randomly assign sparkles
    for (int i = 0; i < density; i++) {
        int ledIndex = random(ledConfig.LED_NUM); // Randomly select an LED index

        // Choose whether to make the sparkle colorful or white
        bool makeWhite = random(100) < 50; // 30% chance of white sparkles, adjust as needed
        if (makeWhite) {
            ledConfig.LEDs[ledIndex] = CRGB::White;
        } else {
            // Randomly choose a vibrant color using HSV
            ledConfig.LEDs[ledIndex] = CHSV(random(0, 255), 200, sparkleBrightness);
            // High saturation for vibrant colors
        }
    }

    // Fade out all LEDs gradually to create a subtle sparkle effect
    for (int i = 0; i < ledConfig.LED_NUM; i++) {
        ledConfig.LEDs[i].fadeToBlackBy(100); // Adjust fade speed as needed
    }
}
