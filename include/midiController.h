#ifndef RSGLED_MIDICONTROLLER_H
#define RSGLED_MIDICONTROLLER_H

#include <Arduino.h>

typedef struct {
    /// Boolean array for 255 note on/off velocity values (0-255)
    byte noteOn[255];
    /// Array of 128 control values (0-255)
    byte controls[128];
} MidiData;

void MIDIC_init(byte _midiChannel);

MidiData *MIDIC_read();

#endif //RSGLED_MIDICONTROLLER_H
