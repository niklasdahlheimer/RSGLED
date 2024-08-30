#include "midiController.h"
#include "ledController.h"

MIDI_CREATE_INSTANCE(HardwareSerial, Serial, MIDI);

MidiController::MidiController(SoftwareSerial *logSerial) :
        logSerial(logSerial),
        noteOn{} {

    Serial.begin(115200);

    // Create and bind the MIDI interface to the default hardware Serial port
    //midiSerial.begin(31250); // MIDI Baudrate für den SoftwareSerial Port
    MIDI.begin(MIDI_CHANNEL);
    MIDI.turnThruOff();

    MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleNoteOff(handleNoteOff);
    MIDI.setHandleError(handleError);

}

void MidiController::read() {
    MIDI.read();
}

void MidiController::handleNoteOn(byte channel, byte note, byte velocity) {
    serialPrintf(logSerial, "NoteOn: %d %d %d", channel, note, velocity);
    noteOn[note] = true;
}

void MidiController::handleNoteOff(byte channel, byte note, byte velocity) {
    serialPrintf(logSerial, "NoteOff: %d %d %d", channel, note, velocity);
    noteOn[note] = false;
}

void MidiController::handleError(int8_t error) {

};

void MidiController::handleSystemExclusive(byte *array, unsigned size) {

};

void MidiController::handleTimeCodeQuarterFrame(byte data) {

};

void MidiController::handleSongPosition(unsigned int beats) {

};

void MidiController::handleSongSelect(byte songNumber) {

};

void MidiController::handleTuneRequest() {

};
