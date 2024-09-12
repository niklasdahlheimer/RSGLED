#include "ledController.h"
#include "helper.h"

SoftwareSerial* ledLogSerial;

CRGB leds[LED_NUM];

CRGBSet group1 = CRGBSet(leds, 8, 9);
CRGBSet group2 = CRGBSet(leds, 6, 7);
CRGBSet group3 = CRGBSet(leds, 4, 5);
CRGBSet group4 = CRGBSet(leds, 2, 3);
CRGBSet group5 = CRGBSet(leds, 0, 1);
CRGBSet groupAll = CRGBSet(leds, 0, 9);

unsigned long lastTriggerMillis = 0;

unsigned long strobeStartMillis = 0;

unsigned long breathStartMillis = 0;
double breathBrightnessFactor = 0.5;

double noiseCurrentVal = 0.5;
unsigned long noiseLastUpdateMillis = 0;

unsigned int rainbowStartHue = 0;
unsigned long rainbowStartMillis = 0;

void maybeSetEffectStartTime(boolean isNoteOn, unsigned long *startTimeRef, const unsigned long *curr);
void LED_on(CRGBSet *group, const CRGB *color = &COLOR_RSG_BLUE);
void LED_off(CRGBSet *group);
void LED_strobe();
void LED_breath();
void LED_noise();
void LED_rainbow();

void LEDC_init(SoftwareSerial* serial)         {
        ledLogSerial = serial;

    // sanity check delay - allows reprogramming if accidentally blowing power w/leds
    //delay(1000);
    FastLED.addLeds<LED_CHIP, LED_DATA_PIN, LED_COLOR_ORDER>(leds, LED_NUM); // GRB ordering is typical
    FastLED.setBrightness(LED_BRIGHTNESS_MAX);
    FastLED.clear(true);
}

void LEDC_updateStripe(const bool *noteOn, const unsigned long triggerMillis) {
    // prime values
    lastTriggerMillis = triggerMillis;

    FastLED.clear();
    FastLED.setBrightness(LED_BRIGHTNESS_MAX);

    maybeSetEffectStartTime(noteOn[BREATH_NOTE], &breathStartMillis, &triggerMillis);
    maybeSetEffectStartTime(noteOn[STROBE_NOTE], &strobeStartMillis, &triggerMillis);
    maybeSetEffectStartTime(noteOn[RAINBOW_NOTE], &rainbowStartMillis, &triggerMillis);

    // Effects
    if (noteOn[STROBE_NOTE]) {
        LED_strobe();
    } else if (noteOn[BREATH_NOTE]) {
        LED_breath();
    } else if (noteOn[NOISE_NOTE]) {
        LED_noise();
    } else if (noteOn[RAINBOW_NOTE]) {
        LED_rainbow();
    }

    // Color for All On
    if (noteOn[Note_C1]) {
        LED_on(&groupAll, &COLOR_RSG_PINK);
    }
    if (noteOn[Note_D1]) {
        LED_on(&groupAll, &COLOR_RSG_Purple);
    }
    if (noteOn[Note_E1]) {
        LED_on(&groupAll, &COLOR_RSG_GREEN);
    }
    if (noteOn[Note_F1]) {
        LED_on(&groupAll, &COLOR_RSG_BLUE);
    }
    if (noteOn[Note_G1]) {
        LED_on(&groupAll, &COLOR_RSG_Yellow);
    }
    if (noteOn[Note_A1]) {
        LED_on(&groupAll, &COLOR_RSG_RED);
    }
    if (noteOn[Note_B1]) {
        LED_on(&groupAll, &COLOR_RSG_WHITE);
    }

    // Segments On
    if (noteOn[Note_C2]) {
        LED_on(&group1, &COLOR_RSG_PINK);
    }
    if (noteOn[Note_Cis2]) {
        LED_on(&group2, &COLOR_RSG_WHITE);
    }
    if (noteOn[Note_D2]) {
        LED_on(&group3, &COLOR_RSG_GREEN);
    }
    if (noteOn[Note_Dis2]) {
        LED_on(&group4, &COLOR_RSG_PINK);
    }
    if (noteOn[Note_E2]) {
        LED_on(&group5);
    }

    FastLED.show();
}

 // private

void LED_on(CRGBSet *group, const CRGB *color) {
    *group = *color;
}

void LED_off(CRGBSet *group) {
    *group = CRGB::Black;
}

int getRectValue(unsigned long currentTime, unsigned long period, float onFactor) {
    unsigned long phase = currentTime % period;
    unsigned long onPeriod = period * onFactor;
    return phase < onPeriod ? 1 : 0;
}

void LED_strobe() {
    int state = getRectValue(strobeStartMillis - lastTriggerMillis, STROBE_PERIOD, STROBE_ON_FACTOR);
    if (state == 1) {
        LED_on(&groupAll, &COLOR_RSG_BLUE); // Turn all LEDs on to the strobe color
    } else {
        FastLED.clear(); // Turn all LEDs off
    }
}

void LED_breath() {
    const unsigned int millisOfCurrentSecond = (lastTriggerMillis - breathStartMillis);

    breathBrightnessFactor = 0.5 * (1 + sin(2 * M_PI * millisOfCurrentSecond / BREATH_PERIOD_IN_MILLIS - M_PI / 2));
    FastLED.setBrightness(static_cast<uint8_t>(breathBrightnessFactor * LED_BRIGHTNESS_MAX));
    LED_on(&groupAll, &COLOR_RSG_BLUE); // Turn all LEDs on to the strobe color
}

void LED_noise() {
    if (lastTriggerMillis - noiseLastUpdateMillis > NOISE_PERIOD_IN_MILLIS) {
        noiseLastUpdateMillis = lastTriggerMillis;
        noiseCurrentVal += (random(0, 10) - 5) * 0.01;
        noiseCurrentVal = noiseCurrentVal > 0.9 ? 0.9 : (noiseCurrentVal < 0.1 ? 0.1 : noiseCurrentVal);
    }
    FastLED.setBrightness(LED_BRIGHTNESS_MAX * noiseCurrentVal);
    LED_on(&groupAll, &COLOR_RSG_BLUE); // Turn all LEDs on to the strobe color
}

void LED_rainbow() {
    fill_rainbow(leds, LED_NUM,
                 ((rainbowStartMillis - lastTriggerMillis) / RAINBOW_PERIOD_IN_MILLIS) + rainbowStartHue,
                 10);
}

void maybeSetEffectStartTime(const boolean isNoteOn, unsigned long *startTimeRef, const unsigned long *curr) {
    if(isNoteOn && *startTimeRef == 0) {
        *startTimeRef = *curr;
    }else if (!isNoteOn && *startTimeRef != 0){
        *startTimeRef = 0;
    }
}