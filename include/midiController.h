#ifndef RSGLED_MIDICONTROLLER_H
#define RSGLED_MIDICONTROLLER_H

#include <Arduino.h>
#include <midiData.h>

void MIDIC_init(byte _midiChannel, MidiData *_midiData);

MidiData *MIDIC_read();

unsigned long MIDIC_lastNoteOn();

#endif //RSGLED_MIDICONTROLLER_H
