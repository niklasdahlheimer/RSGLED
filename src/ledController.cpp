#include "ledController.h"
#include <vector>
#include "fxGradientWalk.h"
#include "fxGradientFade.h"
#include "fxBreath.h"
#include "fxNoise.h"
#include "fxRainbow.h"
#include "fxSparkle.h"
#include "fxPalette.h"
#include "overlayLevelPump.h"
#include "overlayRotate.h"
#include "overlayStrobe.h"

static LEDConfig ledConfig;
std::vector<FXBase *> effects;

// static ons

static void maybeSetAllOn(const byte *note, const byte *controller) {
    if (note[ALL_ON_COLOR_1]) {
        ledConfig.allOn(&COLORS[0], note[ALL_ON_COLOR_1]);
    }
    if (note[ALL_ON_COLOR_2]) {
        ledConfig.allOn(&COLORS[1], note[ALL_ON_COLOR_2]);
    }
    if (note[ALL_ON_COLOR_3]) {
        ledConfig.allOn(&COLORS[2], note[ALL_ON_COLOR_3]);
    }
    if (note[ALL_ON_COLOR_4]) {
        ledConfig.allOn(&COLORS[3], note[ALL_ON_COLOR_4]);
    }
    if (note[ALL_ON_COLOR_5]) {
        ledConfig.allOn(&COLORS[4], note[ALL_ON_COLOR_5]);
    }
    if (note[ALL_ON_COLOR_6]) {
        ledConfig.allOn(&COLORS[5], note[ALL_ON_COLOR_6]);
    }
    if (note[ALL_ON_COLOR_7]) {
        ledConfig.allOn(&COLORS[6], note[ALL_ON_COLOR_7]);
    }
    if (note[ALL_ON_COLOR_8]) {
        ledConfig.allOn(&COLORS[7], note[ALL_ON_COLOR_8]);
    }
    if (note[ALL_ON_COLOR_9]) {
        ledConfig.allOn(&COLORS[8], note[ALL_ON_COLOR_9]);
    }
    if (note[ALL_ON_COLOR_10]) {
        ledConfig.allOn(&COLORS[9], note[ALL_ON_COLOR_10]);
    }
    if (note[ALL_ON_COLOR_11]) {
        ledConfig.allOn(&COLORS[10], note[ALL_ON_COLOR_11]);
    }
    if (note[ALL_ON_COLOR_12]) {
        ledConfig.allOn(&COLORS[11], note[ALL_ON_COLOR_12]);
    }
}

static void maybeSetGroupOn(const byte *note, const byte *controller) {
    // Groups/Segments On
    if (note[GROUP_ON_1]) {
        ledConfig.groupOn(ledConfig.groups[1], &ledConfig.groupColor[1], note[GROUP_ON_1]);
    }
    if (note[GROUP_ON_2]) {
        ledConfig.groupOn(ledConfig.groups[2], &ledConfig.groupColor[2], note[GROUP_ON_2]);
    }
    if (note[GROUP_ON_3]) {
        ledConfig.groupOn(ledConfig.groups[3], &ledConfig.groupColor[3], note[GROUP_ON_3]);
    }
    if (note[GROUP_ON_4]) {
        ledConfig.groupOn(ledConfig.groups[4], &ledConfig.groupColor[4], note[GROUP_ON_4]);
    }
    if (note[GROUP_ON_5]) {
        ledConfig.groupOn(ledConfig.groups[5], &ledConfig.groupColor[5], note[GROUP_ON_5]);
    }
    if (note[GROUP_ON_6]) {
        ledConfig.groupOn(ledConfig.groups[6], &ledConfig.groupColor[6], note[GROUP_ON_6]);
    }
    if (note[GROUP_ON_7]) {
        ledConfig.groupOn(ledConfig.groups[7], &ledConfig.groupColor[7], note[GROUP_ON_7]);
    }
    if (note[GROUP_ON_8]) {
        ledConfig.groupOn(ledConfig.groups[8], &ledConfig.groupColor[8], note[GROUP_ON_8]);
    }
    if (note[GROUP_ON_9]) {
        ledConfig.groupOn(ledConfig.groups[9], &ledConfig.groupColor[9], note[GROUP_ON_9]);
    }
    if (note[GROUP_ON_10]) {
        ledConfig.groupOn(ledConfig.groups[10], &ledConfig.groupColor[10], note[GROUP_ON_10]);
    }
    if (note[GROUP_ON_ALL]) {
        ledConfig.groupOn(ledConfig.groups[1], &ledConfig.groupColor[1], note[GROUP_ON_ALL]);
        ledConfig.groupOn(ledConfig.groups[2], &ledConfig.groupColor[2], note[GROUP_ON_ALL]);
        ledConfig.groupOn(ledConfig.groups[3], &ledConfig.groupColor[3], note[GROUP_ON_ALL]);
        ledConfig.groupOn(ledConfig.groups[4], &ledConfig.groupColor[4], note[GROUP_ON_ALL]);
        ledConfig.groupOn(ledConfig.groups[5], &ledConfig.groupColor[5], note[GROUP_ON_ALL]);
        ledConfig.groupOn(ledConfig.groups[6], &ledConfig.groupColor[6], note[GROUP_ON_ALL]);
        ledConfig.groupOn(ledConfig.groups[7], &ledConfig.groupColor[7], note[GROUP_ON_ALL]);
        ledConfig.groupOn(ledConfig.groups[8], &ledConfig.groupColor[8], note[GROUP_ON_ALL]);
        ledConfig.groupOn(ledConfig.groups[9], &ledConfig.groupColor[9], note[GROUP_ON_ALL]);
        ledConfig.groupOn(ledConfig.groups[10], &ledConfig.groupColor[10], note[GROUP_ON_ALL]);
    }
}

static void maybeSetLevelOn(const byte *note, const byte *controller) {
    // Horizontal Segment Blocks (for Level Meter etc.)
    if (note[LEVEL_ON_1]) {
        ledConfig.levelOn(1, ledConfig.globalColor, note[LEVEL_ON_1]);
    }
    if (note[LEVEL_ON_2]) {
        ledConfig.levelOn(2, ledConfig.globalColor, note[LEVEL_ON_2]);
    }
    if (note[LEVEL_ON_3]) {
        ledConfig.levelOn(3, ledConfig.globalColor, note[LEVEL_ON_3]);
    }
    if (note[LEVEL_ON_4]) {
        ledConfig.levelOn(4, ledConfig.globalColor, note[LEVEL_ON_4]);
    }
    if (note[LEVEL_ON_5]) {
        ledConfig.levelOn(5, ledConfig.globalColor, note[LEVEL_ON_5]);
    }
}

// Global param setters

static void maybeSetGlobalColor(const byte *note, const byte *controller) {
    if (note[GLOBAL_COLOR_1]) {
        ledConfig.globalColor = &COLORS[0];
    }
    if (note[GLOBAL_COLOR_2]) {
        ledConfig.globalColor = &COLORS[0];
    }
    if (note[GLOBAL_COLOR_3]) {
        ledConfig.globalColor = &COLORS[0];
    }
    if (note[GLOBAL_COLOR_4]) {
        ledConfig.globalColor = &COLORS[0];
    }
    if (note[GLOBAL_COLOR_5]) {
        ledConfig.globalColor = &COLORS[0];
    }
    if (note[GLOBAL_COLOR_6]) {
        ledConfig.globalColor = &COLORS[0];
    }
    if (note[GLOBAL_COLOR_7]) {
        ledConfig.globalColor = &COLORS[0];
    }
    if (note[GLOBAL_COLOR_8]) {
        ledConfig.globalColor = &COLORS[0];
    }
    if (note[GLOBAL_COLOR_9]) {
        ledConfig.globalColor = &COLORS[0];
    }
    if (note[GLOBAL_COLOR_10]) {
        ledConfig.globalColor = &COLORS[0];
    }
    if (note[GLOBAL_COLOR_11]) {
        ledConfig.globalColor = &COLORS[0];
    }
    if (note[GLOBAL_COLOR_12]) {
        ledConfig.globalColor = &COLORS[0];
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

int findMaxLedNum(const Config *config) {
    int max = 0;
    for (int i = 0; i < MAX_LINE_NUM; i++) {
        if (!config->lines[i][0]) break;
        for (int j = 0; j < MAX_PIXEL_PER_LINE_NUM; j++) {
            if (config->lines[i][j] > max) {
                max = config->lines[i][j];
            }
        }
    }
    return max;
}

// public

void reset() {
    FastLED.clear(true);
    ledConfig.tempo = DEFAULT_TEMPO;
    ledConfig.tempoTrim = 1;
    ledConfig.globBrightness = LED_BRIGHTNESS_MAX;
    ledConfig.timestamp = 0;
    ledConfig.globalColor = &COLORS[0];
    for (auto &i: ledConfig.groupColor) {
        i = *ledConfig.globalColor;
    }
    for (const auto &effect: effects) {
        effect->reset();
    }
}

void LEDC_init(const Config *config) {
    ledConfig.LED_NUM = findMaxLedNum(config);

    // assign ledConfig.lines
    for (int i = 0; i < MAX_LINE_NUM; i++) {
        if (!config->lines[i][0]) break;
        ledConfig.LINE_NUM++;
        for (int j = 0; j < MAX_PIXEL_PER_LINE_NUM; j++) {
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
    for (int i = 1; i < MAX_GROUP_NUM; i++) {
        if (!config->groups[i][0]) break;
        ledConfig.GROUP_NUM++;
        for (int j = 0; j < MAX_LINE_NUM; j++) {
            if (!config->groups[i][j]) break;
            ledConfig.groups[i][j] = &ledConfig.lines[config->groups[i][j] - 1][0];
        }
    }
    Serial.printf("LED_NUM: %d, LINE_NUM: %d, GROUP_NUM: %d\n",
                  ledConfig.LED_NUM, ledConfig.LINE_NUM, ledConfig.GROUP_NUM);

    //FastLED.setMaxPowerInMilliWatts( 250*1000);
    FastLED.addLeds<LED_CHIP, LED_DATA_PIN, LED_COLOR_ORDER>(ledConfig.LEDs, ledConfig.LED_NUM);
    // GRB ordering is typical
    reset();

    effects.push_back(new FXGradientWalk(GRADIENT_WALK));
    effects.push_back(new FXGradientFade(GRADIENT_FADE));
    effects.push_back(new FXRainbow(RAINBOW));
    effects.push_back(new FXPalette(PALETTE));

    // Overlays
    effects.push_back(new FXBreath(BREATH));
    effects.push_back(new FXSparkle(SPARKLE));
    effects.push_back(new FXNoise(NOISE));

    effects.push_back(new OverlayLevelPump(PUMP));
    effects.push_back(new OverlayRotate(ROTATE));
    effects.push_back(new OverlayStrobe(STROBE));
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

    maybeSetAllOn(note, controller);

    maybeSetGroupOn(note, controller);

    maybeSetLevelOn(note, controller);

    // handle effects
    for (const auto &effect: effects) {
        //Serial.printf("handling effect with note %d\n", effect->getTriggerNote());
        effect->handle(ledConfig);
    }

    // push to stripe
    FastLED.show();
}
