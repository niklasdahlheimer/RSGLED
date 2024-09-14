#ifndef RSGLED_MIDICONTROLLER_H
#define RSGLED_MIDICONTROLLER_H

#include <SoftwareSerial.h>

#define MIDI_CHANNEL 1

void MIDIC_init(SoftwareSerial *logSerial);

void MIDIC_read();

bool *MIDIC_getNoteOnArray();

unsigned int MIDIC_getTempo();


#endif //RSGLED_MIDICONTROLLER_H
