#ifndef RSGLED_MIDICONTROLLERRTP_H
#define RSGLED_MIDICONTROLLERRTP_H

#include <Arduino.h>
#include <midiData.h>

void MIDICRTP_init(byte _midiChannel, char letter, int ip, MidiData *_midiData);
void MIDICRTP_loop();
void MIDICRTP_disconnect();
unsigned long MIDICRTP_lastNoteOn();

#endif //RSGLED_MIDICONTROLLERRTP_H
