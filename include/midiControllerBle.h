#ifndef RSGLED_MIDICONTROLLERBLE_H
#define RSGLED_MIDICONTROLLERBLE_H


void MIDICBLE_init(byte _midiChannel, MidiData* _bleMidiData);

MidiData *MIDICBLE_read();

void MIDICBLE_loop();

#endif //RSGLED_MIDICONTROLLERBLE_H
