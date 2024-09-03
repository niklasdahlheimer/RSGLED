#ifndef RSGLED_LEDCONTROLLER_H
#define RSGLED_LEDCONTROLLER_H
#include <Arduino.h>
#include <FastLED.h>
#include <SoftwareSerial.h>
#include "midiConsts.h"

#define LED_CHIP WS2811
#define LED_COLOR_ORDER BRG
#define LED_NUM 10
#define LED_DATA_PIN 3
#define LED_BRIGHTNESS_MAX 100

#define STROBE_NOTE Note_C3
#define STROBE_PERIOD  100   // Duration in milliseconds the LEDs stay ON
#define STROBE_ON_FACTOR 0.5 // Factor between 0 and 1 how long the LEDs stay ON

#define BREATH_NOTE Note_D3
#define BREATH_PERIOD_IN_MILLIS 5000

#define NOISE_NOTE Note_E3
#define NOISE_PERIOD_IN_MILLIS 10

#define RAINBOW_NOTE Note_F3
#define RAINBOW_PERIOD_IN_MILLIS 10

static CRGB COLOR_RSG_BLUE = CRGB::SkyBlue;
static CRGB COLOR_RSG_WHITE = CRGB::White;
static CRGB COLOR_RSG_GREEN = CRGB::Green;
static CRGB COLOR_RSG_PINK = CRGB::Pink;
static CRGB COLOR_RSG_RED = CRGB::Pink;
static CRGB COLOR_RSG_Yellow = CRGB::Yellow;
static CRGB COLOR_RSG_Purple = CRGB::Purple;

void LEDC_init(SoftwareSerial* logSerial);
void LEDC_updateStripe(const bool* noteOn, unsigned long triggerMillis);

CRGB leds[LED_NUM];

CRGBSet group1 = CRGBSet(leds, 8, 9);
CRGBSet group2 = CRGBSet(leds, 6, 7);
CRGBSet group3 = CRGBSet(leds, 4, 5);
CRGBSet group4 = CRGBSet(leds, 2, 3);
CRGBSet group5 = CRGBSet(leds, 0, 1);
CRGBSet groupAll = CRGBSet(leds, 0, 9);


#endif //RSGLED_LEDCONTROLLER_H
