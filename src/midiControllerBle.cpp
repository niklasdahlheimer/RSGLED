#include "midiController.h"
#include <Arduino.h>
#include "defines.h"
#include <BLEMidi.h>

MidiData data;

// forward declaration

void handleConnect();

void handleDisconnect();

void handleControlChange(byte channel, byte number, byte value, uint16_t timestamp);

void handleNoteOn(byte channel, byte note, byte velocity, uint16_t timestamp);

void handleNoteOff(byte channel, byte note, byte velocity, uint16_t timestamp);

// public

void MIDICBLE_init() {
    pinMode(MIDI_INPUT_LED, OUTPUT);

    BLEMidiServer.begin("Basic MIDI device");
    BLEMidiServer.enableDebugging(Serial);

    BLEMidiServer.setOnConnectCallback(handleConnect);
    BLEMidiServer.setOnDisconnectCallback(handleDisconnect);
    BLEMidiServer.setControlChangeCallback(handleControlChange);
    BLEMidiServer.setNoteOnCallback(handleNoteOn);
    BLEMidiServer.setNoteOffCallback(handleNoteOff);
}

MidiData *MIDICBLE_read() {
    return &data;
}

// private

void handleControlChange(byte channel, byte number, byte value, uint16_t timestamp) {
    data.controls[number] = value;
}

void handleNoteOn(byte channel, byte note, byte velocity, uint16_t timestamp) {
    data.noteOn[note] = 2 * velocity;
    digitalWrite(MIDI_INPUT_LED, HIGH);
    Serial.printf("note on %d", note);
}

void handleNoteOff(byte channel, byte note, byte velocity, uint16_t timestamp) {
    //serialPrintf(midiLogSerial, "NoteOff: %d %d %d", channel, note, velocity);
    data.noteOn[note] = 0;
    digitalWrite(MIDI_INPUT_LED, LOW);
    Serial.printf("note off %d", note);
}
