#ifndef RSGLED_MIDICONTROLLERBLE_H
#define RSGLED_MIDICONTROLLERBLE_H

#include <Arduino.h>

typedef struct {
    byte noteOn[255];               // Boolean array for note on/off states
    byte controls[128];              // Array of 128 control values (0-127)
} MidiData;

void MIDICBLE_init();

MidiData * MIDICBLE_read();

#endif //RSGLED_MIDICONTROLLERBLE_H
