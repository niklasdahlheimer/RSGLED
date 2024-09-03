#ifndef RSGLED_MIDICONTROLLER_H
#define RSGLED_MIDICONTROLLER_H

#include <Arduino.h>
#include "midiConsts.h"
#include "helper.h"
#include <MIDI.h>
#include <SoftwareSerial.h>

#define MIDI_CHANNEL 1

void MIDIC_init(SoftwareSerial *logSerial);

void MIDIC_read();

bool *MIDIC_getNoteOnArray();


#endif //RSGLED_MIDICONTROLLER_H
