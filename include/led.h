#ifndef LED_H
#define LED_H

#define BLINK_TIME_IN_MS_SLOW 500
#define BLINK_TIME_IN_MS_FAST 250

#include <ezLED.h>

#if defined(LEDCONTROLLER)
#define CONNECTION_LED_PIN GPIO_NUM_21
#define MIDI_INPUT_LED_PIN GPIO_NUM_19
#elif defined(RELAYCONTROLLER)
#define CONNECTION_LED_PIN GPIO_NUM_18
#define MIDI_INPUT_LED_PIN GPIO_NUM_19
#endif

static ezLED connectionLED(CONNECTION_LED_PIN);
static ezLED midiLED(MIDI_INPUT_LED_PIN);

inline void LED_blinkOnce(ezLED* led) {
    led->blinkNumberOfTimes(40, 40, 1, 0);
}

inline void LED_blinkSlow(ezLED* led) {
    led->blink(BLINK_TIME_IN_MS_SLOW,BLINK_TIME_IN_MS_SLOW);
}

inline void LED_blinkFast(ezLED* led) {
    led->blink(BLINK_TIME_IN_MS_FAST,BLINK_TIME_IN_MS_FAST);
}

inline void LED_Off(ezLED* led) {
    led->turnOFF();
}

inline void LED_On(ezLED* led) {
    led->turnON();
}


#endif //LED_H
