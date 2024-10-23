#include "midiController.h"
#include "ledController.h"
#include <Arduino.h>
#include <MIDI.h>
#include "defines.h"

MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, MIDI);

static MidiData midiData;
static byte midiChannel;

void handleControlChange(byte channel, byte number, byte value);

void handlePitchBend(byte channel, int bend);

void handleNoteOn(byte channel, byte note, byte velocity);

void handleAfterTouchPoly(byte channel, byte note, byte pressure);

void handleNoteOff(byte channel, byte note, byte velocity);

void handleError(int8_t error);

void MIDIC_init(const byte _midiChannel) {
    midiChannel = _midiChannel;
    // Create and bind the MIDI interface to the default hardware Serial port
    //midiSerial.begin(31250); // MIDI Baudrate für den SoftwareSerial Port
    MIDI.begin(midiChannel);
    MIDI.turnThruOff();
    MIDI.setHandleControlChange(handleNoteOn);
    MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleNoteOff(handleNoteOff);
    MIDI.setHandleError(handleError);
    MIDI.setHandleAfterTouchPoly(handleAfterTouchPoly);
    pinMode(MIDI_INPUT_LED, OUTPUT);
}

MidiData *MIDIC_read() {
    MIDI.read(midiChannel);
    return &midiData;
}

// private

void handleControlChange(byte channel, byte number, byte value) {
    midiData.controls[number] = value;
    Serial.printf("control change on %03d, value %03d", number, value);
}

void handleNoteOn(byte channel, byte note, byte velocity) {
    midiData.noteOn[note] = 2 * velocity;
    digitalWrite(MIDI_INPUT_LED, HIGH);
    Serial.printf("note on %03d, velocity %03d", note, velocity);
}

void handleAfterTouchPoly(byte channel, byte note, byte pressure) {
    //data.noteOn[note] = 2 * pressure;
    Serial.printf("after touch on %03d, pressure %03d", note, pressure);
}

void handleNoteOff(byte channel, byte note, byte velocity) {
    //serialPrintf(midiLogSerial, "NoteOff: %d %d %d", channel, note, velocity);
    midiData.noteOn[note] = 0;
    digitalWrite(MIDI_INPUT_LED, LOW);
    Serial.printf("note off %03d", note);
}

void handleError(int8_t error) {
    Serial.printf("MIDI error %03d", error);
};
