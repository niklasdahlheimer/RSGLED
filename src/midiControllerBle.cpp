#include "midiController.h"
#include <Arduino.h>
#include "defines.h"
#include <BLEMidi.h>

static MidiData bleMidiData;
static byte midiChannel;

// forward declaration

void handleConnect();

void handleDisconnect();

void handleControlChange(byte channel, byte number, byte value, uint16_t timestamp);

void handleNoteOn(byte channel, byte note, byte velocity, uint16_t timestamp);

void handleNoteOff(byte channel, byte note, byte velocity, uint16_t timestamp);

// public

void MIDICBLE_init(const byte _midiChannel) {
    midiChannel = _midiChannel;
    char name[16];
    sprintf(name, "RSGLED%02d", midiChannel);
    pinMode(MIDI_INPUT_LED, OUTPUT);
    Serial.println("init BleController");
    BLEMidiServer.begin(name);
    //BLEMidiServer.enableDebugging(Serial);

    BLEMidiServer.setOnConnectCallback(handleConnect);
    BLEMidiServer.setOnDisconnectCallback(handleDisconnect);
    BLEMidiServer.setControlChangeCallback(handleControlChange);
    BLEMidiServer.setNoteOnCallback(handleNoteOn);
    BLEMidiServer.setNoteOffCallback(handleNoteOff);
}

MidiData *MIDICBLE_read() {
    return &bleMidiData;
}

// private

void handleConnect() {
    Serial.println("MIDI BLE Controller connected!");
    Serial.printf("listening for midi on channel %d\n",midiChannel+1);
}

void handleDisconnect() {
    Serial.println("MIDI BLE Controller disconnected!");
}

void handleControlChange(byte channel, byte number, byte value, uint16_t timestamp) {
    if(channel == midiChannel) {
        bleMidiData.controls[number] = value;
        Serial.printf("control change %d %d\n", number, value);
    }
}

void handleNoteOn(byte channel, byte note, byte velocity, uint16_t timestamp) {
    if(channel == midiChannel) {
        bleMidiData.noteOn[note] = 2 * velocity;
        digitalWrite(MIDI_INPUT_LED, velocity > 0 ? HIGH : LOW);
        Serial.printf("note on %d, velocity %d\n", note, velocity);
    }
}

void handleNoteOff(byte channel, byte note, byte velocity, uint16_t timestamp) {
    if(channel == midiChannel) {
        bleMidiData.noteOn[note] = 0;
        digitalWrite(MIDI_INPUT_LED, LOW);
        Serial.printf("note off %d\n", note);
    }
}
