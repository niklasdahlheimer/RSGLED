#ifndef LED_H
#define LED_H

#include <ezLED.h>

#if defined(LEDCONTROLLER)
#define CONNECTION_LED_PIN GPIO_NUM_21
#define MIDI_INPUT_LED_PIN GPIO_NUM_19
#elif defined(RELAYCONTROLLER)
#define CONNECTION_LED_PIN GPIO_NUM_23
#define MIDI_INPUT_LED_PIN GPIO_NUM_22
#endif

static ezLED connectionLED(CONNECTION_LED_PIN);
static ezLED midiLED(MIDI_INPUT_LED_PIN);

inline void LED_dataInBlink() {
    midiLED.blinkNumberOfTimes(40, 0, 1, 0);
}

#endif //LED_H
