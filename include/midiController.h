#ifndef RSGLED_MIDICONTROLLER_H
#define RSGLED_MIDICONTROLLER_H

#include <Arduino.h>

#define ALL_NOTE_OFF_CC 123
#define ALL_NOTE_OFF_VAL 0

typedef struct {
    /// Boolean array for 255 note on/off velocity values (0-255)
    byte noteOn[255];
    /// Array of 128 control values (0-255)
    byte controls[128];
} MidiData;

void MIDIC_init(byte _midiChannel, MidiData* _midiData);

MidiData *MIDIC_read();

unsigned long MIDIC_lastNoteOn();

#endif //RSGLED_MIDICONTROLLER_H
