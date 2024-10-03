#ifndef RSGLED_MIDICONTROLLER_H
#define RSGLED_MIDICONTROLLER_H

#include <SoftwareSerial.h>

typedef struct {
    bool noteOn[255];               // Boolean array for note on/off states
    unsigned char controls[128];    // Array of 128 control values (0-127)
    unsigned int tempo;             // Tempo, default value will be set in the program
} MidiData;

void MIDIC_init(SoftwareSerial *logSerial);

MidiData * MIDIC_read();

#endif //RSGLED_MIDICONTROLLER_H
