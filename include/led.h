#ifndef LED_H
#define LED_H

#include <ezLED.h>

#if defined(LEDCONTROLLER)
#define CONNECTION_LED GPIO_NUM_21
#define MIDI_INPUT_LED GPIO_NUM_19
#elif defined(RELAYCONTROLLER)
#define CONNECTION_LED GPIO_NUM_23
#define MIDI_INPUT_LED GPIO_NUM_22
#endif

#define CONNECTION_BLINK_TIME_IN_MS 500

static ezLED connectionLED(CONNECTION_LED);
static ezLED midiLED(MIDI_INPUT_LED);


inline void LED_dataInBlink() {
    midiLED.blinkNumberOfTimes(40, 0, 1, 0);
}


#endif //LED_H
