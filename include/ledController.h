#ifndef RSGLED_LEDCONTROLLER_H
#define RSGLED_LEDCONTROLLER_H

#include <Arduino.h>
#include <FastLED.h>
#include <SoftwareSerial.h>
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

void LEDC_init(SoftwareSerial* logSerial);
void LEDC_updateStripe(const byte *noteData, const byte *controllerData);


static CRGB COLOR_1 = CRGB::SkyBlue;
static CRGB COLOR_2 = CRGB::White;
static CRGB COLOR_3 = CRGB::Green;
static CRGB COLOR_4 = CRGB::Pink;
static CRGB COLOR_5 = CRGB::Yellow;
static CRGB COLOR_6 = CRGB::Purple;
static CRGB COLOR_7 = CRGB::Red;
static CRGB COLOR_8 = CRGB::HotPink;
static CRGB COLOR_9 = CRGB::Blue;
static CRGB COLOR_10 = CRGB::Orange;
static CRGB COLOR_11 = CRGB::Teal;
static CRGB COLOR_12 = CRGB::MidnightBlue;

#define NOTE_ALL_COLOR_1 Note_C1
#define NOTE_ALL_COLOR_2 Note_Cis1
#define NOTE_ALL_COLOR_3 Note_D1
#define NOTE_ALL_COLOR_4 Note_Dis1
#define NOTE_ALL_COLOR_5 Note_E1
#define NOTE_ALL_COLOR_6 Note_F1
#define NOTE_ALL_COLOR_7 Note_Fis1
#define NOTE_ALL_COLOR_8 Note_G1
#define NOTE_ALL_COLOR_9 Note_Gis1
#define NOTE_ALL_COLOR_10 Note_A1
#define NOTE_ALL_COLOR_11 Note_Ais1
#define NOTE_ALL_COLOR_12 Note_B1

#define NOTE_COLOR_SWITCH_1 Note_C2
#define NOTE_COLOR_SWITCH_2 Note_Cis2
#define NOTE_COLOR_SWITCH_3 Note_D2
#define NOTE_COLOR_SWITCH_4 Note_Dis2
#define NOTE_COLOR_SWITCH_5 Note_E2
#define NOTE_COLOR_SWITCH_6 Note_F2
#define NOTE_COLOR_SWITCH_7 Note_Fis2
#define NOTE_COLOR_SWITCH_8 Note_G2
#define NOTE_COLOR_SWITCH_9 Note_Gis2
#define NOTE_COLOR_SWITCH_10 Note_A2
#define NOTE_COLOR_SWITCH_11 Note_Ais2
#define NOTE_COLOR_SWITCH_12 Note_B2

#define NOTE_GROUP_1 Note_C3
#define NOTE_GROUP_2 Note_Cis3
#define NOTE_GROUP_3 Note_D3
#define NOTE_GROUP_4 Note_Dis3
#define NOTE_GROUP_5 Note_E3
#define NOTE_GROUP_6 Note_F3
#define NOTE_GROUP_7 Note_Fis3
#define NOTE_GROUP_8 Note_G3
#define NOTE_GROUP_9 Note_Gis3
#define NOTE_GROUP_10 Note_A3

#define NOTE_STROBE Note_C5
#define NOTE_BREATH Note_D5
#define NOTE_NOISE Note_E5
#define NOTE_RAINBOW Note_F5
#define NOTE_PUMP Note_G5

#define NOTE_LEVEL_1 Note_C6
#define NOTE_LEVEL_2 Note_Cis6
#define NOTE_LEVEL_3 Note_D6
#define NOTE_LEVEL_4 Note_Dis6
#define NOTE_LEVEL_5 Note_E6

#define CONTROLLER_INDEX_BRIGHTNESS_TRIM     0
#define CONTROLLER_INDEX_TEMPO               1
#define TEMPO_OFFSET                         60

#endif //RSGLED_LEDCONTROLLER_H
