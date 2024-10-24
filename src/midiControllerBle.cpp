#include "midiController.h"
#include <Arduino.h>
#include <BLEMidi.h>

#define MIDI_INPUT_LED 2
#define MIDI_CHANNEL_ALL 11

static MidiData bleMidiData;
static byte midiChannel;

// forward declarations

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
    Serial.printf("listening for midi on channel %d and %d\n", midiChannel + 1, MIDI_CHANNEL_ALL + 1);
    digitalWrite(MIDI_INPUT_LED, HIGH);
}

void handleDisconnect() {
    Serial.println("MIDI BLE Controller disconnected!");
    digitalWrite(MIDI_INPUT_LED, LOW);
}

void handleControlChange(byte channel, byte number, byte value, uint16_t timestamp) {
    if (channel == midiChannel || channel == MIDI_CHANNEL_ALL) {
        bleMidiData.controls[number] = value * 2;
        Serial.printf("control change %d %d\n", number, value);
    }
}

void handleNoteOn(byte channel, byte note, byte velocity, uint16_t timestamp) {
    if (channel == midiChannel || channel == MIDI_CHANNEL_ALL) {
        bleMidiData.noteOn[note] = 2 * velocity;
        digitalWrite(MIDI_INPUT_LED, velocity > 0 ? LOW : HIGH);
        Serial.printf("note on %d, velocity %d\n", note, velocity);
    }
}

void handleNoteOff(byte channel, byte note, byte velocity, uint16_t timestamp) {
    if (channel == midiChannel || channel == MIDI_CHANNEL_ALL) {
        bleMidiData.noteOn[note] = 0;
        digitalWrite(MIDI_INPUT_LED, HIGH);
        Serial.printf("note off %d\n", note);
    }
}
