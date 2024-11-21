#include "ledController.h"
#include <vector>

#include "fxCOlorAll.h"
#include "FXColorGroup.h"
#include "FXColorLevel.h"
#include "FXColorLine.h"

#include "fxGradientWalk.h"
#include "fxGradientFade.h"
#include "fxRainbow.h"
#include "fxSparkle.h"
#include "fxPalette.h"
#include "fxWhiteSegments.h"
#include "fxTest.h"
#include "fxFreeRun.h"

#include "overlayLevelPump.h"
#include "overlayRotateGroup.h"
#include "OverlayRotateLine.h"
#include "overlayStrobe.h"
#include "overlayNoise.h"
#include "overlayBreath.h"
#include "OverlayDab.h"

static LEDConfig ledConfig;
std::vector<FXBase *> effects;

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
    if (controller[CONTROLLER_TEMPO_TRIM] == ledConfig.lastControllerValues[CONTROLLER_TEMPO_TRIM]) return;
    const byte trimValue = controller[CONTROLLER_TEMPO_TRIM];

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

void maybeSetFadeInTime(const byte *controller) {
    if (controller[CONTROLLER_FADE_IN] == ledConfig.lastControllerValues[CONTROLLER_FADE_IN]) return;
    ledConfig.fadeInTime = map(controller[CONTROLLER_FADE_IN], 0, 254, 0,LED_FADE_IN_TIME_MILLIS_MAX);
    Serial.printf("fade in time to %d ms \n", ledConfig.fadeInTime);
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
            ledConfig.lines[i][j] = &ledConfig.LEDs[config->lines[i][j] - 1]; // indices are 1-based to avoid null-pointers
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

    effects.push_back(new FXColorAll(ALL_ON_COLOR_1, &COLORS[0]));
    effects.push_back(new FXColorAll(ALL_ON_COLOR_2, &COLORS[1]));
    effects.push_back(new FXColorAll(ALL_ON_COLOR_3, &COLORS[2]));
    effects.push_back(new FXColorAll(ALL_ON_COLOR_4, &COLORS[3]));
    effects.push_back(new FXColorAll(ALL_ON_COLOR_5, &COLORS[4]));
    effects.push_back(new FXColorAll(ALL_ON_COLOR_6, &COLORS[5]));
    effects.push_back(new FXColorAll(ALL_ON_COLOR_7, &COLORS[6]));
    effects.push_back(new FXColorAll(ALL_ON_COLOR_8, &COLORS[7]));
    effects.push_back(new FXColorAll(ALL_ON_COLOR_9, &COLORS[8]));
    effects.push_back(new FXColorAll(ALL_ON_COLOR_10, &COLORS[9]));
    effects.push_back(new FXColorAll(ALL_ON_COLOR_11, &COLORS[10]));
    effects.push_back(new FXColorAll(ALL_ON_COLOR_12, &COLORS[11]));

    effects.push_back(new FXColorGroup(GROUP_ON_1, &ledConfig.groupColor[1], 1));
    effects.push_back(new FXColorGroup(GROUP_ON_2, &ledConfig.groupColor[2], 2));
    effects.push_back(new FXColorGroup(GROUP_ON_3, &ledConfig.groupColor[3], 3));
    effects.push_back(new FXColorGroup(GROUP_ON_4, &ledConfig.groupColor[4], 4));
    effects.push_back(new FXColorGroup(GROUP_ON_5, &ledConfig.groupColor[5], 5));
    effects.push_back(new FXColorGroup(GROUP_ON_6, &ledConfig.groupColor[6], 6));
    effects.push_back(new FXColorGroup(GROUP_ON_7, &ledConfig.groupColor[7], 7));
    effects.push_back(new FXColorGroup(GROUP_ON_8, &ledConfig.groupColor[8], 8));
    effects.push_back(new FXColorGroup(GROUP_ON_9, &ledConfig.groupColor[9], 9));
    effects.push_back(new FXColorGroup(GROUP_ON_10, &ledConfig.groupColor[10], 10));

    effects.push_back(new FXColorLevel(LEVEL_ON_1, ledConfig.globalColor, 1));
    effects.push_back(new FXColorLevel(LEVEL_ON_2, ledConfig.globalColor, 2));
    effects.push_back(new FXColorLevel(LEVEL_ON_3, ledConfig.globalColor, 3));
    effects.push_back(new FXColorLevel(LEVEL_ON_4, ledConfig.globalColor, 4));
    effects.push_back(new FXColorLevel(LEVEL_ON_5, ledConfig.globalColor, 5));

    //effects
    effects.push_back(new FXGradientWalk(GRADIENT_WALK));
    effects.push_back(new FXGradientFade(GRADIENT_FADE));
    effects.push_back(new FXRainbow(RAINBOW));
    effects.push_back(new FXPalette(PALETTE));
    effects.push_back(new FXWhiteSegments(WHITE_SEGMENTS));
    effects.push_back(new FXColorLine(LINE_ON, ledConfig.globalColor));

    effects.push_back(new FXTest(TEST_MODE));
    effects.push_back(new FXFreeRun(FREE_RUN));

    // Overlays
    effects.push_back(new OverlayStrobe(STROBE));
    effects.push_back(new OverlayRotateGroup(ROTATE_GROUP));
    effects.push_back(new OverlayRotateLine(ROTATE_LINE));
    effects.push_back(new OverlayBreath(BREATH));
    effects.push_back(new FXSparkle(SPARKLE));
    effects.push_back(new OverlayNoise(NOISE));
    effects.push_back(new OverlayLevelPump(PUMP));
    effects.push_back(new OverlayDab(FLASH_LINE));
}

void LEDC_updateStripe(const byte *note, const byte *controller) {
    // fixed time reference for all calculations
    ledConfig.timestamp = millis();
    ledConfig.note = note;
    ledConfig.controller = controller;

    if (note[TOTAL_RESET]) reset();

    FastLED.clear();

    // meta values
    maybeSetGroupColor(note, controller);
    maybeSetGlobalBrightness(&controller[CONTROLLER_GLOBAL_BRIGHTNESS_TRIM]);
    maybeSetTempo(note[TEMPO] / 2);
    maybeSetTempoTrim(controller);
    maybeSetGlobalColor(note, controller);
    maybeSetFadeInTime(controller);

    // handle effects
    for (const auto &effect: effects) {
        //Serial.printf("handling effect with note %d\n", effect->getTriggerNote());
        effect->handle(ledConfig);
    }

    memcpy8(ledConfig.lastControllerValues, controller, 128);

    // push to stripe
    FastLED.show();
}
