#include "ledController.h"
#include <vector>

#include "fxGradientWalk.h"
#include "fxGradientFade.h"
#include "fxStrobe.h"
#include "fxBreath.h"
#include "fxNoise.h"

CRGBPalette16 palettes[] = {
    OceanColors_p, RainbowColors_p, PartyColors_p, HeatColors_p, LavaColors_p, CloudColors_p
};

static LEDConfig ledConfig;

std::vector<Effect*> effects;

// FX vars
static unsigned int rainbowStartHue = 0;
static unsigned long rainbowStartMillis = 0;
static unsigned long pumpStartMillis = 0;
static unsigned long rotateStartMillis = 0;

static unsigned long paletteWalkStartMillis = 0;
static byte currentPalette = 0;

// Forward declarations

void reset();

static unsigned int
getBeatLenInMillis(unsigned int tempo, unsigned int div = 4, boolean isTrip = false, boolean isDot = false);

void maybeSetGroupColor(const byte *note, const byte *controller);

void maybeSetGlobalBrightness(const byte *brightnessTrimValue);

static void maybeSetGlobalColor(const byte *note, const byte *controller);

static void maybeSetFX(const byte *note, const byte *controller);

static void maybeSetAllOn(const byte *note, const byte *controller);

static void maybeSetGroupOn(const byte *note, const byte *controller);

static void maybeSetLevelOn(const byte *note, const byte *controller);

void maybeSetTempo(byte tempoValue);

void maybeSetTempoTrim(const byte *controller);

static void LED_line_on(CRGB *line[], const CRGB *color = DEFAULT_COLOR, byte brightness = 255);

static void LED_group_on(CRGB **group[], const CRGB *color = DEFAULT_COLOR, byte brightness = 255);

static void LED_all_on(const CRGB *color = DEFAULT_COLOR, byte brightness = 255);

//FX
static void maybeSetEffectStartTime(byte noteValue, unsigned long *startTimeRef, const unsigned long *curr,
                                    byte *increaseVal = nullptr);

static void fxRainbow(byte velo);

static void fxLevelPump(byte velo);

static void fxRotate(byte velo);

static void fxSparkle(byte velo);

static void fxPalette(byte velocity, const CRGBPalette16 *pal);

static void fxGradientFade(byte velo, CRGB *color1, CRGB *color2);

// Definitions

void LEDC_init(const Config *config) {
    ledConfig.LED_NUM = config->LED_NUM;

    // assign ledConfig.lines
    for (int i = 0; i < MAX_LINE_NUM; i++) {
        if (!config->lines[i][0]) break;
        ledConfig.LINE_NUM++;
        for (int j = 0; j < MAX_PIXEL_PER_LINE; j++) {
            if (!config->lines[i][j]) break;
            ledConfig.lines[i][j] = &ledConfig.LEDs[config->lines[i][j] - 1];
        }
    }

    // "all" group
    int pos = 0;
    for (int i = 0; i < ledConfig.LINE_NUM; i++) {
        if (!ledConfig.lines[i][0]) break; // should not happen
        ledConfig.groups[0][pos] = ledConfig.lines[i];
        Serial.printf("%d: assigned line %d to group 0\n", pos, i);
        pos++;
    }
    // assign ledConfig.groups
    for (int i = 1; i < MAX_GROUP_COUNT; i++) {
        if (!config->groups[i][0]) break;
        ledConfig.GROUP_NUM++;
        for (int j = 0; j < MAX_LINE_NUM; j++) {
            if (!config->groups[i][j]) break;
            ledConfig.groups[i][j] = &ledConfig.lines[config->groups[i][j] - 1][0];
        }
    }
    Serial.printf("LED_NUM: %d, LINE_NUM: %d, GROUP_NUM: %d\n", ledConfig.LED_NUM, ledConfig.LINE_NUM,
                  ledConfig.GROUP_NUM);

    //FastLED.setMaxPowerInMilliWatts( 250*1000);
    FastLED.addLeds<LED_CHIP, LED_DATA_PIN, LED_COLOR_ORDER>(ledConfig.LEDs, ledConfig.LED_NUM);
    // GRB ordering is typical
    reset();

    effects.push_back(new EffectGradientWalk(GRADIENT_WALK));
    effects.push_back(new EffectStrobe(STROBE));
    effects.push_back(new EffectBreath(BREATH));
    effects.push_back(new EffectNoise(NOISE));
    effects.push_back(new EffectGradientFade(GRADIENT_FADE));
}

void LEDC_updateStripe(const byte *note, const byte *controller) {
    // fixed time reference for all calculations
    ledConfig.timestamp = millis();
    ledConfig.note = note;
    ledConfig.controller = controller;
    //memcpy8(lastControllerValues, controller, 128);

    if (note[TOTAL_RESET]) reset();

    FastLED.clear();

    // meta values
    maybeSetGroupColor(note, controller);
    maybeSetGlobalBrightness(&note[GLOBAL_BRIGHTNESS_TRIM]);
    maybeSetTempo(note[TEMPO] / 2);
    maybeSetTempoTrim(controller);
    maybeSetGlobalColor(note, controller);

    for (const auto& effect : effects) {
        //Serial.printf("handling effect with note %d\n", effect->getTriggerNote());
        effect->handle(ledConfig);
    }

    // set colors etc.
    maybeSetFX(note, controller);

    maybeSetAllOn(note, controller);

    maybeSetGroupOn(note, controller);

    maybeSetLevelOn(note, controller);

    // push to stripe
    FastLED.show();
}

void reset() {
    FastLED.clear(true);
    ledConfig.tempo = DEFAULT_TEMPO;
    ledConfig.tempoTrim = 1;
    ledConfig.globBrightness = LED_BRIGHTNESS_MAX;
    ledConfig.timestamp = 0;
    rainbowStartHue = 0;
    rainbowStartMillis = 0;
    pumpStartMillis = 0;
    rotateStartMillis = 0;
    paletteWalkStartMillis = 0;
    ledConfig.globalColor = &COLOR_1;
    for (auto &i: ledConfig.groupColor) {
        i = *ledConfig.globalColor;
    }
    for (const auto& effect : effects) {
        effect->reset();
    }
}

static void maybeSetGlobalColor(const byte *note, const byte *controller) {
    if (note[GLOBAL_COLOR_1]) {
        ledConfig.globalColor = &COLOR_1;
    }
    if (note[GLOBAL_COLOR_2]) {
        ledConfig.globalColor = &COLOR_2;
    }
    if (note[GLOBAL_COLOR_3]) {
        ledConfig.globalColor = &COLOR_3;
    }
    if (note[GLOBAL_COLOR_4]) {
        ledConfig.globalColor = &COLOR_4;
    }
    if (note[GLOBAL_COLOR_5]) {
        ledConfig.globalColor = &COLOR_5;
    }
    if (note[GLOBAL_COLOR_6]) {
        ledConfig.globalColor = &COLOR_6;
    }
    if (note[GLOBAL_COLOR_7]) {
        ledConfig.globalColor = &COLOR_7;
    }
    if (note[GLOBAL_COLOR_8]) {
        ledConfig.globalColor = &COLOR_8;
    }
    if (note[GLOBAL_COLOR_9]) {
        ledConfig.globalColor = &COLOR_9;
    }
    if (note[GLOBAL_COLOR_10]) {
        ledConfig.globalColor = &COLOR_10;
    }
    if (note[GLOBAL_COLOR_11]) {
        ledConfig.globalColor = &COLOR_11;
    }
    if (note[GLOBAL_COLOR_12]) {
        ledConfig.globalColor = &COLOR_12;
    }
}

static void maybeSetFX(const byte *note, const byte *controller) {
    maybeSetEffectStartTime(note[RAINBOW], &rainbowStartMillis, &ledConfig.timestamp);
    maybeSetEffectStartTime(note[PUMP], &pumpStartMillis, &ledConfig.timestamp);
    maybeSetEffectStartTime(note[ROTATE], &rotateStartMillis, &ledConfig.timestamp);
    maybeSetEffectStartTime(note[PALETTE], &paletteWalkStartMillis, &ledConfig.timestamp, &currentPalette);

    if (note[SPARKLE]) {
        fxSparkle(note[SPARKLE]);
    } else if (note[RAINBOW]) {
        fxRainbow(note[RAINBOW]);
    } else if (note[PUMP]) {
        fxLevelPump(note[PUMP]);
    } else if (note[ROTATE]) {
        fxRotate(note[ROTATE]);
    } else if (note[PALETTE]) {
        fxPalette(note[PALETTE], &palettes[currentPalette % 6]);
    }
}

static void maybeSetAllOn(const byte *note, const byte *controller) {
    if (note[ALL_ON_COLOR_1]) {
        LED_all_on(&COLOR_1, note[ALL_ON_COLOR_1]);
    }
    if (note[ALL_ON_COLOR_2]) {
        LED_all_on(&COLOR_2, note[ALL_ON_COLOR_2]);
    }
    if (note[ALL_ON_COLOR_3]) {
        LED_all_on(&COLOR_3, note[ALL_ON_COLOR_3]);
    }
    if (note[ALL_ON_COLOR_4]) {
        LED_all_on(&COLOR_4, note[ALL_ON_COLOR_4]);
    }
    if (note[ALL_ON_COLOR_5]) {
        LED_all_on(&COLOR_5, note[ALL_ON_COLOR_5]);
    }
    if (note[ALL_ON_COLOR_6]) {
        LED_all_on(&COLOR_6, note[ALL_ON_COLOR_6]);
    }
    if (note[ALL_ON_COLOR_7]) {
        LED_all_on(&COLOR_7, note[ALL_ON_COLOR_7]);
    }
    if (note[ALL_ON_COLOR_8]) {
        LED_all_on(&COLOR_8, note[ALL_ON_COLOR_8]);
    }
    if (note[ALL_ON_COLOR_9]) {
        LED_all_on(&COLOR_9, note[ALL_ON_COLOR_9]);
    }
    if (note[ALL_ON_COLOR_10]) {
        LED_all_on(&COLOR_10, note[ALL_ON_COLOR_10]);
    }
    if (note[ALL_ON_COLOR_11]) {
        LED_all_on(&COLOR_11, note[ALL_ON_COLOR_11]);
    }
    if (note[ALL_ON_COLOR_12]) {
        LED_all_on(&COLOR_12, note[ALL_ON_COLOR_12]);
    }
}

static void maybeSetGroupOn(const byte *note, const byte *controller) {
    // Groups/Segments On
    if (note[GROUP_ON_1]) {
        LED_group_on(ledConfig.groups[1], &ledConfig.groupColor[1], note[GROUP_ON_1]);
    }
    if (note[GROUP_ON_2]) {
        LED_group_on(ledConfig.groups[2], &ledConfig.groupColor[2], note[GROUP_ON_2]);
    }
    if (note[GROUP_ON_3]) {
        LED_group_on(ledConfig.groups[3], &ledConfig.groupColor[3], note[GROUP_ON_3]);
    }
    if (note[GROUP_ON_4]) {
        LED_group_on(ledConfig.groups[4], &ledConfig.groupColor[4], note[GROUP_ON_4]);
    }
    if (note[GROUP_ON_5]) {
        LED_group_on(ledConfig.groups[5], &ledConfig.groupColor[5], note[GROUP_ON_5]);
    }
    if (note[GROUP_ON_6]) {
        LED_group_on(ledConfig.groups[6], &ledConfig.groupColor[6], note[GROUP_ON_6]);
    }
    if (note[GROUP_ON_7]) {
        LED_group_on(ledConfig.groups[7], &ledConfig.groupColor[7], note[GROUP_ON_7]);
    }
    if (note[GROUP_ON_8]) {
        LED_group_on(ledConfig.groups[8], &ledConfig.groupColor[8], note[GROUP_ON_8]);
    }
    if (note[GROUP_ON_9]) {
        LED_group_on(ledConfig.groups[9], &ledConfig.groupColor[9], note[GROUP_ON_9]);
    }
    if (note[GROUP_ON_10]) {
        LED_group_on(ledConfig.groups[10], &ledConfig.groupColor[10], note[GROUP_ON_10]);
    }
    if (note[GROUP_ON_ALL]) {
        LED_group_on(ledConfig.groups[1], &ledConfig.groupColor[1], note[GROUP_ON_ALL]);
        LED_group_on(ledConfig.groups[2], &ledConfig.groupColor[2], note[GROUP_ON_ALL]);
        LED_group_on(ledConfig.groups[3], &ledConfig.groupColor[3], note[GROUP_ON_ALL]);
        LED_group_on(ledConfig.groups[4], &ledConfig.groupColor[4], note[GROUP_ON_ALL]);
        LED_group_on(ledConfig.groups[5], &ledConfig.groupColor[5], note[GROUP_ON_ALL]);
        LED_group_on(ledConfig.groups[6], &ledConfig.groupColor[6], note[GROUP_ON_ALL]);
        LED_group_on(ledConfig.groups[7], &ledConfig.groupColor[7], note[GROUP_ON_ALL]);
        LED_group_on(ledConfig.groups[8], &ledConfig.groupColor[8], note[GROUP_ON_ALL]);
        LED_group_on(ledConfig.groups[9], &ledConfig.groupColor[9], note[GROUP_ON_ALL]);
        LED_group_on(ledConfig.groups[10], &ledConfig.groupColor[10], note[GROUP_ON_ALL]);
    }
}

static void level1_On(const CRGB *color, const byte velocity) {
    LED_group_on(ledConfig.groups[1], color, velocity);
    LED_group_on(ledConfig.groups[10], color, velocity);
}

static void level2_On(const CRGB *color, const byte velocity) {
    level1_On(color, velocity);
    LED_group_on(ledConfig.groups[2], color, velocity);
    LED_group_on(ledConfig.groups[9], color, velocity);
}

static void level3_On(const CRGB *color, const byte velocity) {
    level2_On(color, velocity);
    LED_group_on(ledConfig.groups[3], color, velocity);
    LED_group_on(ledConfig.groups[8], color, velocity);
}

static void level4_On(const CRGB *color, const byte velocity) {
    level3_On(color, velocity);
    LED_group_on(ledConfig.groups[4], color, velocity);
    LED_group_on(ledConfig.groups[7], color, velocity);
}

static void level5_On(const CRGB *color, const byte velocity) {
    level4_On(color, velocity);
    LED_group_on(ledConfig.groups[5], color, velocity);
    LED_group_on(ledConfig.groups[6], color, velocity);
}

static void maybeSetLevelOn(const byte *note, const byte *controller) {
    // Horizontal Segment Blocks (for Level Meter etc.)
    if (note[LEVEL_ON_1]) {
        level1_On(ledConfig.globalColor, note[LEVEL_ON_1]);
    }
    if (note[LEVEL_ON_2]) {
        level2_On(ledConfig.globalColor, note[LEVEL_ON_2]);
    }
    if (note[LEVEL_ON_3]) {
        level3_On(ledConfig.globalColor, note[LEVEL_ON_3]);
    }
    if (note[LEVEL_ON_4]) {
        level4_On(ledConfig.globalColor, note[LEVEL_ON_4]);
    }
    if (note[LEVEL_ON_5]) {
        level5_On(ledConfig.globalColor, note[LEVEL_ON_5]);
    }
}

void maybeSetGroupColor(const byte *note, const byte *controller) {
    if (note[GROUP_HUE_1]) {
        ledConfig.groupColor[1] = CHSV(note[GROUP_HUE_1], 255 - controller[CONTROLLER_GROUP_COLOR_SATURATION_TRIM],
                                       255 - controller[CONTROLLER_GROUP_COLOR_BRIGHTNESS_TRIM]);
    }
    if (note[GROUP_HUE_2]) {
        ledConfig.groupColor[2] = CHSV(note[GROUP_HUE_2], 255 - controller[CONTROLLER_GROUP_COLOR_SATURATION_TRIM],
                                       255 - controller[CONTROLLER_GROUP_COLOR_BRIGHTNESS_TRIM]);
    }
    if (note[GROUP_HUE_3]) {
        ledConfig.groupColor[3] = CHSV(note[GROUP_HUE_3], 255 - controller[CONTROLLER_GROUP_COLOR_SATURATION_TRIM],
                                       255 - controller[CONTROLLER_GROUP_COLOR_BRIGHTNESS_TRIM]);
    }
    if (note[GROUP_HUE_4]) {
        ledConfig.groupColor[4] = CHSV(note[GROUP_HUE_4], 255 - controller[CONTROLLER_GROUP_COLOR_SATURATION_TRIM],
                                       255 - controller[CONTROLLER_GROUP_COLOR_BRIGHTNESS_TRIM]);
    }
    if (note[GROUP_HUE_5]) {
        ledConfig.groupColor[5] = CHSV(note[GROUP_HUE_5], 255 - controller[CONTROLLER_GROUP_COLOR_SATURATION_TRIM],
                                       255 - controller[CONTROLLER_GROUP_COLOR_BRIGHTNESS_TRIM]);
    }
    if (note[GROUP_HUE_6]) {
        ledConfig.groupColor[6] = CHSV(note[GROUP_HUE_6], 255 - controller[CONTROLLER_GROUP_COLOR_SATURATION_TRIM],
                                       255 - controller[CONTROLLER_GROUP_COLOR_BRIGHTNESS_TRIM]);
    }
    if (note[GROUP_HUE_7]) {
        ledConfig.groupColor[7] = CHSV(note[GROUP_HUE_7], 255 - controller[CONTROLLER_GROUP_COLOR_SATURATION_TRIM],
                                       255 - controller[CONTROLLER_GROUP_COLOR_BRIGHTNESS_TRIM]);
    }
    if (note[GROUP_HUE_8]) {
        ledConfig.groupColor[8] = CHSV(note[GROUP_HUE_8], 255 - controller[CONTROLLER_GROUP_COLOR_SATURATION_TRIM],
                                       255 - controller[CONTROLLER_GROUP_COLOR_BRIGHTNESS_TRIM]);
    }
    if (note[GROUP_HUE_9]) {
        ledConfig.groupColor[9] = CHSV(note[GROUP_HUE_9], 255 - controller[CONTROLLER_GROUP_COLOR_SATURATION_TRIM],
                                       255 - controller[CONTROLLER_GROUP_COLOR_BRIGHTNESS_TRIM]);
    }
    if (note[GROUP_HUE_10]) {
        ledConfig.groupColor[10] = CHSV(note[GROUP_HUE_10], 255 - controller[CONTROLLER_GROUP_COLOR_SATURATION_TRIM],
                                        255 - controller[CONTROLLER_GROUP_COLOR_BRIGHTNESS_TRIM]);
    }
}

// PRIVATE

// LED control helpers

static void LED_line_on(CRGB *line[], const CRGB *color, byte brightness) {
    for (int i = 0; i < MAX_PIXEL_PER_LINE; ++i) {
        if (!line[i]) break;
        *(line[i]) = *color;
        line[i]->nscale8_video(brightness);
    }
}

static void LED_group_on(CRGB **group[], const CRGB *color, byte brightness) {
    for (int line = 0; line < ledConfig.LINE_NUM; ++line) {
        if (!group[line]) break;
        LED_line_on(group[line], color, brightness); // LED_line_on
    }
}

static void LED_all_on(const CRGB *color, byte brightness) {
    LED_group_on(ledConfig.groups[0], color, brightness);
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
    if (*brightnessTrimValue == 0 && ledConfig.globBrightness == LED_BRIGHTNESS_MAX) {
        return;
    }
    ledConfig.globBrightness = LED_BRIGHTNESS_MAX - *brightnessTrimValue * 2;
    FastLED.setBrightness(ledConfig.globBrightness);
    Serial.printf("set globBrightness to %d\n", ledConfig.globBrightness);
}

void maybeSetTempoTrim(const byte *controller) {
    const byte trimValue = controller[CONTROLLER_TEMPO_TRIM];
    //Serial.printf("hello from maybeSetTempoTrim with %d\n", trimValue);
    if (trimValue == ledConfig.lastControllerValues[CONTROLLER_TEMPO_TRIM]) return;

    if (trimValue == 0 || trimValue == 128) {
        ledConfig.tempoTrim = 1;
    } else if (trimValue < 128) {
        long mappedValue = map(trimValue, 1, 127, 125, 1000);
        ledConfig.tempoTrim = static_cast<double>(mappedValue) / 1000.0;
    } else if (trimValue > 128) {
        long mappedValue = map(trimValue, 129, 254, 1000, 8000);
        ledConfig.tempoTrim = static_cast<double>(mappedValue) / 1000.0;
    }
    Serial.printf("set tempo trim to %f \n", ledConfig.tempoTrim);
}

void maybeSetTempo(const byte tempoValue) {
    // ignore null value or equal tempo
    if (tempoValue == 0 || tempoValue + TEMPO_OFFSET == ledConfig.tempo) {
        return;
    }
    ledConfig.tempo = tempoValue + TEMPO_OFFSET;
    Serial.printf("set tempo to %d bpm (value was %d)\n", ledConfig.tempo, tempoValue);
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

static void fxRainbow(byte velo) {
    fill_rainbow_circular(ledConfig.LEDs, ledConfig.LED_NUM,
                          ((ledConfig.timestamp - rainbowStartMillis) / RAINBOW_PERIOD_IN_MILLIS) + rainbowStartHue,
                          (uint8_t) (10 * (velo / 127.0)));
}

static void fxLevelPump(byte velo) {
    const unsigned int currentStep =
            getSteppedSawValue(ledConfig.timestamp - pumpStartMillis, PUMP_PERIOD_IN_MILLIS, 6);
    switch (currentStep) {
        case 0:
            break; // all off
        case 1:
            level1_On(ledConfig.globalColor, velo);
            break;
        case 2:
            level2_On(ledConfig.globalColor, velo);
            break;
        case 3:
            level3_On(ledConfig.globalColor, velo);
            break;
        case 4:
            level4_On(ledConfig.globalColor, velo);
            break;
        case 5:
            level5_On(ledConfig.globalColor, velo);
            break;
        default:
            break;
    }
}

static void fxRotate(byte velo) {
    const unsigned int currentStep = getSteppedSawValue(ledConfig.timestamp - pumpStartMillis,
                                                        getBeatLenInMillis(ledConfig.tempo, 16),
                                                        10);
    LED_group_on(ledConfig.groups[currentStep], ledConfig.globalColor, velo);
}

static void fxPalette(byte velocity, const CRGBPalette16 *pal) {
    const unsigned int step = getSteppedSawValue(ledConfig.timestamp - paletteWalkStartMillis,
                                                 getBeatLenInMillis(ledConfig.tempo, 64),
                                                 ledConfig.LINE_NUM);
    // circling offset
    for (int i = 0; i < ledConfig.LINE_NUM; i++) {
        byte paletteIndex = map(i, 0, ledConfig.LINE_NUM, 0, 255);
        CRGB colorFromPalette = ColorFromPalette(*pal, paletteIndex);
        LED_line_on(ledConfig.lines[(i + step) % ledConfig.LINE_NUM], &colorFromPalette, velocity);
    }
}

static void fxSparkle(byte velocity) {
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