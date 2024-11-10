#ifndef LED_H
#define LED_H

#include <ezLED.h>

#define CONNECTION_LED 21
#define MIDI_INPUT_LED 19
#define MIDI_CHANNEL_ALL 11
#define CONNECTION_BLINK_TIME_IN_MS 500

static ezLED connectionLED(CONNECTION_LED);
static ezLED midiLED(MIDI_INPUT_LED);


inline void LED_dataInBlink() {
    midiLED.blinkNumberOfTimes(40, 0, 1, 0);
}


#endif //LED_H
