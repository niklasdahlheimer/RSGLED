#ifndef RSGLED_MIDICONTROLLER_H
#define RSGLED_MIDICONTROLLER_H

#include <Arduino.h>
#include <midiData.h>

#define ALL_NOTE_OFF_CC 123
#define ALL_NOTE_OFF_VAL 0

void MIDIC_init(byte _midiChannel, MidiData* _midiData);

MidiData *MIDIC_read();

unsigned long MIDIC_lastNoteOn();

#endif //RSGLED_MIDICONTROLLER_H
