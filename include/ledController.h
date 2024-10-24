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
//#define MAX_POWER_MILLIAMPS       500

#define DEFAULT_TEMPO               120

#define MAX_LED_NUM                 100

#define STROBE_ON_FACTOR            0.2 // Factor between 0 and 1 how long the LEDs stay ON

#define BREATH_PERIOD_IN_MILLIS     5000
#define NOISE_PERIOD_IN_MILLIS      10
#define RAINBOW_PERIOD_IN_MILLIS    10
#define PUMP_PERIOD_IN_MILLIS       100

void LEDC_init(const Config* config);
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

#define TOTAL_RESET                 Note_C_2

#define TEMPO                       Note_D_2
#define TEMPO_OFFSET                60

#define GLOBAL_BRIGHTNESS_TRIM      Note_E_2

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

#define ALL_ON_COLOR_1              Note_C0
#define ALL_ON_COLOR_2              Note_Cis0
#define ALL_ON_COLOR_3              Note_D0
#define ALL_ON_COLOR_4              Note_Dis0
#define ALL_ON_COLOR_5              Note_E0
#define ALL_ON_COLOR_6              Note_F0
#define ALL_ON_COLOR_7              Note_Fis0
#define ALL_ON_COLOR_8              Note_G0
#define ALL_ON_COLOR_9              Note_Gis0
#define ALL_ON_COLOR_10             Note_A0
#define ALL_ON_COLOR_11             Note_Ais0
#define ALL_ON_COLOR_12             Note_B0

#define GROUP_HUE_1                 Note_C1
#define GROUP_HUE_2                 Note_Cis1
#define GROUP_HUE_3                 Note_D1
#define GROUP_HUE_4                 Note_Dis1
#define GROUP_HUE_5                 Note_E1
#define GROUP_HUE_6                 Note_F1
#define GROUP_HUE_7                 Note_Fis1
#define GROUP_HUE_8                 Note_G1
#define GROUP_HUE_9                 Note_Gis1
#define GROUP_HUE_10                Note_A1
#define GROUP_SATURATION_TRIM_ALL   Note_Ais1
#define GROUP_BRIGHTNESS_TRIM_ALL   Note_B1

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
#define GROUP_ON_ALL                Note_Ais2

#define LEVEL_ON_1                  Note_C3
#define LEVEL_ON_2                  Note_Cis3
#define LEVEL_ON_3                  Note_D3
#define LEVEL_ON_4                  Note_Dis3
#define LEVEL_ON_5                  Note_E3

#define STROBE                      Note_C4
#define BREATH                      Note_D4
#define SPARKLE                     Note_Dis4
#define NOISE                       Note_E4
#define RAINBOW                     Note_F4
#define PUMP                        Note_G4
#define ROTATE                      Note_A4
#define PALETTE                     Note_Ais4
#define GRADIENT                    Note_B4

#define CONTROLLER_HUE_GROUP        10
#define CONTROLLER_BRIGHTNESS_GROUP 70

#endif //RSGLED_LEDCONTROLLER_H
