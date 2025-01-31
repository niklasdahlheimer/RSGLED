#ifndef RSGLED_MIDICONTROLLERBLE_H
#define RSGLED_MIDICONTROLLERBLE_H

void MIDICBLE_init(byte _midiChannel, char letter, MidiData* _bleMidiData);

MidiData *MIDICBLE_getMidiDataArray();

void MIDICBLE_loop();

unsigned long MIDICBLE_lastNoteOn();

#endif //RSGLED_MIDICONTROLLERBLE_H
