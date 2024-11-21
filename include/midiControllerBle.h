#ifndef RSGLED_MIDICONTROLLERBLE_H
#define RSGLED_MIDICONTROLLERBLE_H


void MIDICBLE_init(byte _midiChannel, char letter, MidiData* _bleMidiData);

MidiData *MIDICBLE_read();

void MIDICBLE_loop();

unsigned long MIDICBLE_lastReceived();

#endif //RSGLED_MIDICONTROLLERBLE_H
