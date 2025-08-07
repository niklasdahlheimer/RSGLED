#include <Arduino.h>

#ifndef MIDIDATA_H
#define MIDIDATA_H

typedef struct {
    /// Boolean array for 255 note on/off velocity values (0-255)
    byte noteOn[255];
    /// Array of 128 control values (0-255)
    byte controls[128];
} MidiData;

#endif //MIDIDATA_H
