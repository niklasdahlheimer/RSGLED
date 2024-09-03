#include "midiController.h"
#include "ledController.h"

MIDI_CREATE_INSTANCE(HardwareSerial, Serial, MIDI);

SoftwareSerial *logSerial;
bool noteOnArray[255];

void MIDIC_handleNoteOn(byte channel, byte note, byte velocity);
void MIDIC_handleNoteOff(byte channel, byte note, byte velocity);
void MIDIC_handleError(int8_t error);
void MIDIC_handleSystemExclusive(byte *array, unsigned size);
void MIDIC_handleTimeCodeQuarterFrame(byte data);
void MIDIC_handleSongPosition(unsigned int beats);
void MIDIC_handleSongSelect(byte songNumber);
void MIDIC_handleTuneRequest();

void MIDIC_init(SoftwareSerial *serial) {
    logSerial = serial;

    //Serial.begin(115200);

    // Create and bind the MIDI interface to the default hardware Serial port
    //midiSerial.begin(31250); // MIDI Baudrate für den SoftwareSerial Port
    MIDI.begin(MIDI_CHANNEL);
    MIDI.turnThruOff();

    MIDI.setHandleNoteOn(MIDIC_handleNoteOn);
    MIDI.setHandleNoteOff(MIDIC_handleNoteOff);
    MIDI.setHandleError(MIDIC_handleError);
}

void MIDIC_read() {
    MIDI.read();
}

bool* MIDIC_getNoteOnArray(){
    return noteOnArray;
}

void MIDIC_handleNoteOn(byte channel, byte note, byte velocity) {
    serialPrintf(logSerial, "NoteOn: %d %d %d", channel, note, velocity);
    noteOnArray[note] = true;
}

void MIDIC_handleNoteOff(byte channel, byte note, byte velocity) {
    serialPrintf(logSerial, "NoteOff: %d %d %d", channel, note, velocity);
    noteOnArray[note] = false;
}

void MIDIC_handleError(int8_t error) {
    serialPrintf(logSerial, "MIDI error: %d", error);
};
