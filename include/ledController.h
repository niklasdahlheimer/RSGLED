#ifndef RSGLED_LEDCONTROLLER_H
#define RSGLED_LEDCONTROLLER_H

#include <Arduino.h>
#include <FastLED.h>
#include "midiConsts.h"
#include "config.h"

#define LED_DATA_PIN                5
#define LED_CHIP                    WS2811
#define LED_COLOR_ORDER             BRG
#define LED_BRIGHTNESS_MAX          255

#define LED_FADE_IN_TIME_MILLIS_MAX 10000

#define DEFAULT_TEMPO               120

#define FULL_GRADIENT_STEPS         200
#define DEFAULT_COLOR               &COLORS[0]

#define TOTAL_RESET                 Note_C_2

#define TEMPO                       Note_D_2
#define TEMPO_OFFSET                60

#define GLOBAL_COLOR_1              Note_C_1
#define GLOBAL_COLOR_2              Note_Cis_1
#define GLOBAL_COLOR_3              Note_D_1
#define GLOBAL_COLOR_4              Note_Dis_1
#define GLOBAL_COLOR_5              Note_E_1
#define GLOBAL_COLOR_6              Note_F_1
#define GLOBAL_COLOR_7              Note_Fis_1
#define GLOBAL_COLOR_8              Note_G_1
#define GLOBAL_COLOR_9              Note_Gis_1
#define GLOBAL_COLOR_10             Note_A_1
#define GLOBAL_COLOR_11             Note_Ais_1
#define GLOBAL_COLOR_12             Note_B_1

#define GROUP_HUE_1                 Note_C0
#define GROUP_HUE_2                 Note_Cis0
#define GROUP_HUE_3                 Note_D0
#define GROUP_HUE_4                 Note_Dis0
#define GROUP_HUE_5                 Note_E0
#define GROUP_HUE_6                 Note_F0
#define GROUP_HUE_7                 Note_Fis0
#define GROUP_HUE_8                 Note_G0
#define GROUP_HUE_9                 Note_Gis0
#define GROUP_HUE_10                Note_A0
#define GROUP_SATURATION_TRIM_ALL   Note_Ais0
#define GROUP_BRIGHTNESS_TRIM_ALL   Note_B0

#define ALL_ON_COLOR_1              Note_C1
#define ALL_ON_COLOR_2              Note_Cis1
#define ALL_ON_COLOR_3              Note_D1
#define ALL_ON_COLOR_4              Note_Dis1
#define ALL_ON_COLOR_5              Note_E1
#define ALL_ON_COLOR_6              Note_F1
#define ALL_ON_COLOR_7              Note_Fis1
#define ALL_ON_COLOR_8              Note_G1
#define ALL_ON_COLOR_9              Note_Gis1
#define ALL_ON_COLOR_10             Note_A1
#define ALL_ON_COLOR_11             Note_Ais1
#define ALL_ON_COLOR_12             Note_B1

#define GROUP_ON_1                  Note_C2
#define GROUP_ON_2                  Note_Cis2
#define GROUP_ON_3                  Note_D2
#define GROUP_ON_4                  Note_Dis2
#define GROUP_ON_5                  Note_E2
#define GROUP_ON_6                  Note_F2
#define GROUP_ON_7                  Note_Fis2
#define GROUP_ON_8                  Note_G2
#define GROUP_ON_9                  Note_Gis2
#define GROUP_ON_10                 Note_A2

#define LEVEL_ON_1                  Note_C3
#define LEVEL_ON_2                  Note_Cis3
#define LEVEL_ON_3                  Note_D3
#define LEVEL_ON_4                  Note_Dis3
#define LEVEL_ON_5                  Note_E3

#define RAINBOW                     Note_C4
#define PALETTE                     Note_Cis4
#define GRADIENT_WALK               Note_D4
#define GRADIENT_FADE               Note_Dis4
#define WHITE_SEGMENTS              Note_E4

#define STROBE                      Note_C5
#define ROTATE                      Note_Cis5
#define BREATH                      Note_D5
#define SPARKLE                     Note_Dis5
#define NOISE                       Note_E5
#define PUMP                        Note_F5
#define FLASH_LINE                  Note_Fis4

#define CONTROLLER_GLOBAL_BRIGHTNESS_TRIM           9
#define CONTROLLER_GROUP_COLOR_SATURATION_TRIM      10
#define CONTROLLER_GROUP_COLOR_BRIGHTNESS_TRIM      70
#define CONTROLLER_TEMPO_TRIM                       76
#define CONTROLLER_FADE_IN                          77

static CRGB COLORS[12]{
    CRGB::LightSkyBlue,
    CRGB::Fuchsia,
    CRGB::Lime,
    CRGB::Pink,
    CRGB::Gold,
    CRGB::Purple,
    CRGB::Red,
    CRGB::HotPink,
    CRGB::Blue,
    CRGB::OrangeRed,
    CRGB::Teal,
    CRGB::Aqua
};

typedef struct {
    int LED_NUM = 0;
    int LINE_NUM = 0;
    int GROUP_NUM = 0;
    CRGB LEDs[MAX_LED_NUM]{};
    CRGB *lines[MAX_LINE_NUM][MAX_PIXEL_PER_LINE_NUM]{};
    CRGB **groups[MAX_GROUP_NUM][MAX_LINE_NUM]{};
    CRGB lineGradientLEDs[MAX_LINE_NUM]{};
    CRGB fullGradientLEDs[FULL_GRADIENT_STEPS]{};
    CRGB groupColor[MAX_GROUP_NUM]{};
    CRGB *globalColor = &COLORS[0];
    byte globBrightness = LED_BRIGHTNESS_MAX;
    byte tempo = DEFAULT_TEMPO;
    double tempoTrim = 1.0;
    unsigned int fadeInTime = 0;
    byte lastControllerValues[128]{};
    const byte *note;
    const byte *controller;
    unsigned long timestamp = 0;

    void lineOn(CRGB *line[], const CRGB *color = DEFAULT_COLOR, const byte brightness = 255) {
        for (int i = 0; i < MAX_PIXEL_PER_LINE_NUM; ++i) {
            if (!line[i]) break;
            *(line[i]) = *color;
            line[i]->nscale8(brightness);
        }
    }

    void lineOff(CRGB *line[]) {
        for (int i = 0; i < MAX_PIXEL_PER_LINE_NUM; ++i) {
            if (!line[i]) break;
            *(line[i]) = CRGB::Black;
        }
    }

    void lineBrighten(CRGB *line[], byte scale) {
        for (int i = 0; i < MAX_PIXEL_PER_LINE_NUM; ++i) {
            if (!line[i]) break;
            (*(line[i])).nscale8(scale);
        }
    }

    void groupOn(CRGB **group[], const CRGB *color = DEFAULT_COLOR, const byte brightness = 255) {
        for (int line = 0; line < LINE_NUM; ++line) {
            if (!group[line]) break;
            lineOn(group[line], color, brightness);
        }
    }

    void groupOff(CRGB **group[]) {
        for (int line = 0; line < LINE_NUM; ++line) {
            if (!group[line]) break;
            lineOff(group[line]);
        }
    }

    void groupBrighten(CRGB **group[], byte scale) {
        for (int line = 0; line < LINE_NUM; ++line) {
            if (!group[line]) break;
            lineBrighten(group[line], scale);
        }
    }

    void groupSolo(CRGB **group[]) {
        for (int i = 1; i < GROUP_NUM + 1; ++i) {
            if (!groups[i]) break;
            if (groups[i] != group) {
                groupOff(groups[i]);
            }
        }
    }

    void levelOn(byte levelNumber, const CRGB *color = DEFAULT_COLOR, const byte brightness = 255) {
        if (levelNumber >= 5) {
            groupOn(groups[5], color, brightness);
            groupOn(groups[6], color, brightness);
        }
        if (levelNumber >= 4) {
            groupOn(groups[4], color, brightness);
            groupOn(groups[7], color, brightness);
        }
        if (levelNumber >= 3) {
            groupOn(groups[3], color, brightness);
            groupOn(groups[8], color, brightness);
        }
        if (levelNumber >= 2) {
            groupOn(groups[2], color, brightness);
            groupOn(groups[9], color, brightness);
        }
        if (levelNumber >= 1) {
            groupOn(groups[1], color, brightness);
            groupOn(groups[10], color, brightness);
        }
    }

    void levelSolo(byte levelNumber) {
        if (levelNumber <= 4) {
            groupOff(groups[5]);
            groupOff(groups[6]);
        }
        if (levelNumber <= 3) {
            groupOff(groups[4]);
            groupOff(groups[7]);
        }
        if (levelNumber <= 2) {
            groupOff(groups[3]);
            groupOff(groups[8]);
        }
        if (levelNumber <= 1) {
            groupOff(groups[2]);
            groupOff(groups[9]);
        }
        if (levelNumber == 0) {
            allOff();
        }
    }

    void allOn(const CRGB *color = DEFAULT_COLOR, const byte brightness = 255) {
        groupOn(groups[0], color, brightness);
    }

    void allOff() {
        groupOff(groups[0]);
    }

    void allBrighten(const byte scale) {
        groupBrighten(groups[0], scale);
    }
} LEDConfig;

void LEDC_init(const Config *config);

void LEDC_updateStripe(const byte *noteData, const byte *controllerData);

#endif //RSGLED_LEDCONTROLLER_H
