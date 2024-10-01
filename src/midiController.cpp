#include "midiController.h"
#include "ledController.h"
#include <Arduino.h>
#include "midiConsts.h"
#include "helper.h"
#include <MIDI.h>

MIDI_CREATE_INSTANCE(HardwareSerial, Serial, MIDI);

SoftwareSerial *midiLogSerial;
bool noteOnArray[255];
unsigned int midiTempo = 120;

void handleSystemExclusive(byte *array, unsigned size);
void handleTimeCodeQuarterFrame(byte data);
void handleSongPosition(unsigned int beats);
void handleSongSelect(byte songNumber);
void handleTuneRequest();
void handleNoteOn(byte channel, byte note, byte velocity);
void handleNoteOff(byte channel, byte note, byte velocity);
void handleError(int8_t error);

void MIDIC_init(SoftwareSerial *serial) {
    midiLogSerial = serial;

    //Serial.begin(115200);

    // Create and bind the MIDI interface to the default hardware Serial port
    //midiSerial.begin(31250); // MIDI Baudrate für den SoftwareSerial Port
    MIDI.begin(MIDI_CHANNEL);
    MIDI.turnThruOff();

    MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleNoteOff(handleNoteOff);
    MIDI.setHandleError(handleError);
    MIDI.setHandleSystemExclusive(handleSystemExclusive);
}

void MIDIC_read() {
    MIDI.read();
}

bool* MIDIC_getNoteOnArray(){
    return noteOnArray;
}

unsigned int MIDIC_getTempo(){
    return midiTempo;
}

// private

void handleSystemExclusive(byte *array, unsigned size) {
    serialPrintf(midiLogSerial, "SysEx:");
    for(unsigned int i = 0; i < size; i++) {
        serialPrintf(midiLogSerial, " %02X", array[i]);
    }
}

void handleNoteOn(byte channel, byte note, byte velocity) {
    //serialPrintf(midiLogSerial, "NoteOn: %d %d %d", channel, note, velocity);
    noteOnArray[note] = true;
}

void handleNoteOff(byte channel, byte note, byte velocity) {
    //serialPrintf(midiLogSerial, "NoteOff: %d %d %d", channel, note, velocity);
    noteOnArray[note] = false;
}

void handleError(int8_t error) {
    serialPrintf(midiLogSerial, "MIDI error: %d", error);
};
