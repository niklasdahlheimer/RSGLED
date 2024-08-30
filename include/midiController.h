#ifndef RSGLED_MIDICONTROLLER_H
#define RSGLED_MIDICONTROLLER_H
#include <Arduino.h>
#include "midiConsts.h"
#include "helper.h"
#include <MIDI.h>
#include <SoftwareSerial.h>

#define MIDI_CHANNEL 1

class MidiController{
public:
    MidiController(SoftwareSerial* logSerial);
    void read();
    bool* getNoteOnArray(){
        return noteOn;
    }

private:
    SoftwareSerial* logSerial;
    bool noteOn[255];
    void handleNoteOn(byte channel, byte note, byte velocity);
    void handleNoteOff(byte channel, byte note, byte velocity);
    void handleError(int8_t error);
    void handleSystemExclusive(byte *array, unsigned size);
    void handleTimeCodeQuarterFrame(byte data);
    void handleSongPosition(unsigned int beats);
    void handleSongSelect(byte songnumber);
    void handleTuneRequest();

};

#endif //RSGLED_MIDICONTROLLER_H
