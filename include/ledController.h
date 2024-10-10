#ifndef RSGLED_LEDCONTROLLER_H
#define RSGLED_LEDCONTROLLER_H

#include <Arduino.h>
#include <FastLED.h>
#include "midiConsts.h"
#include "defines.h"

#define LED_DATA_PIN                7
#define LED_CHIP                    WS2811
#define LED_COLOR_ORDER             BRG
#define LED_BRIGHTNESS_MAX          255
#define MAX_POWER_MILLIAMPS         500

#define STROBE_ON_FACTOR            0.5 // Factor between 0 and 1 how long the LEDs stay ON

#define BREATH_PERIOD_IN_MILLIS     5000
#define NOISE_PERIOD_IN_MILLIS      10
#define RAINBOW_PERIOD_IN_MILLIS    10
#define PUMP_PERIOD_IN_MILLIS       100

void LEDC_init();
void LEDC_updateStripe(const byte *noteData, const byte *controllerData);

static CRGB COLOR_1 = CRGB::LightSkyBlue;
static CRGB COLOR_2 = CRGB::Fuchsia;
static CRGB COLOR_3 = CRGB::Lime;
static CRGB COLOR_4 = CRGB::Pink;
static CRGB COLOR_5 = CRGB::Gold;
static CRGB COLOR_6 = CRGB::Purple;
static CRGB COLOR_7 = CRGB::Red;
static CRGB COLOR_8 = CRGB::HotPink;
static CRGB COLOR_9 = CRGB::Blue;
static CRGB COLOR_10 = CRGB::OrangeRed;
static CRGB COLOR_11 = CRGB::Teal;
static CRGB COLOR_12 = CRGB::Aqua;

#define TOTAL_RESET                 Note_C_1

#define TEMPO                       Note_D_1
#define TEMPO_OFFSET                60

#define GLOBAL_BRIGHTNESS_TRIM      Note_E_1

#define GROUP_COLOR_TRIM_SATURATION Note_F_1
#define GROUP_COLOR_TRIM_BRIGHTNESS Note_G_1

#define GLOBAL_COLOR_1 Note_C0
#define GLOBAL_COLOR_2 Note_Cis0
#define GLOBAL_COLOR_3 Note_D0
#define GLOBAL_COLOR_4 Note_Dis0
#define GLOBAL_COLOR_5 Note_E0
#define GLOBAL_COLOR_6 Note_F0
#define GLOBAL_COLOR_7 Note_Fis0
#define GLOBAL_COLOR_8 Note_G0
#define GLOBAL_COLOR_9 Note_Gis0
#define GLOBAL_COLOR_10 Note_A0
#define GLOBAL_COLOR_11 Note_Ais0
#define GLOBAL_COLOR_12 Note_B0

#define ALL_ON_COLOR_1 Note_C1
#define ALL_ON_COLOR_2 Note_Cis1
#define ALL_ON_COLOR_3 Note_D1
#define ALL_ON_COLOR_4 Note_Dis1
#define ALL_ON_COLOR_5 Note_E1
#define ALL_ON_COLOR_6 Note_F1
#define ALL_ON_COLOR_7 Note_Fis1
#define ALL_ON_COLOR_8 Note_G1
#define ALL_ON_COLOR_9 Note_Gis1
#define ALL_ON_COLOR_10 Note_A1
#define ALL_ON_COLOR_11 Note_Ais1
#define ALL_ON_COLOR_12 Note_B1

#define GROUP_HUE_1 Note_C2
#define GROUP_HUE_2 Note_Cis2
#define GROUP_HUE_3 Note_D2
#define GROUP_HUE_4 Note_Dis2
#define GROUP_HUE_5 Note_E2
#define GROUP_HUE_6 Note_F2
#define GROUP_HUE_7 Note_Fis2
#define GROUP_HUE_8 Note_G2
#define GROUP_HUE_9 Note_Gis2
#define GROUP_HUE_10 Note_A2
#define GROUP_HUE_ALL Note_Ais2
#define GROUP_SAT_TRIM_ALL Note_B2

#define GROUP_ON_1 Note_C3
#define GROUP_ON_2 Note_Cis3
#define GROUP_ON_3 Note_D3
#define GROUP_ON_4 Note_Dis3
#define GROUP_ON_5 Note_E3
#define GROUP_ON_6 Note_F3
#define GROUP_ON_7 Note_Fis3
#define GROUP_ON_8 Note_G3
#define GROUP_ON_9 Note_Gis3
#define GROUP_ON_10 Note_A3

#define LEVEL_ON_1 Note_C4
#define LEVEL_ON_2 Note_Cis4
#define LEVEL_ON_3 Note_D4
#define LEVEL_ON_4 Note_Dis4
#define LEVEL_ON_5 Note_E4

#define STROBE Note_C5
#define BREATH Note_D5
#define NOISE Note_E5
#define RAINBOW Note_F5
#define PUMP Note_G5
#define ROTATE Note_A5
#define GRADIENT Note_B5

#endif //RSGLED_LEDCONTROLLER_H
