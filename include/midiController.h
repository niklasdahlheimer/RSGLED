#ifndef RSGLED_MIDICONTROLLER_H
#define RSGLED_MIDICONTROLLER_H

#include <Arduino.h>

typedef struct {
    byte noteOn[255];               // Boolean array for note on/off states
    byte controls[128];              // Array of 128 control values (0-127)
} MidiData;

void MIDIC_init();

MidiData * MIDIC_read();

#endif //RSGLED_MIDICONTROLLER_H
