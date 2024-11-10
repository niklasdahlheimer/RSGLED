#include "midiController.h"
#include <Arduino.h>
#include <BLEMidi.h>
#include "led.h"

static MidiData* bleMidiData;
static byte midiChannel;


// private

static void onAdvertisingStart() {
    connectionLED.blink(CONNECTION_BLINK_TIME_IN_MS, CONNECTION_BLINK_TIME_IN_MS);
}

static void handleConnect() {
    Serial.println("MIDI BLE Controller connected!");
    Serial.printf("listening for midi on channel %d and %d\n", midiChannel + 1, MIDI_CHANNEL_ALL + 1);
    connectionLED.turnON();
}

static void handleDisconnect() {
    Serial.println("MIDI BLE Controller disconnected!");
    onAdvertisingStart();
}

static void handleControlChange(byte channel, byte number, byte value, uint16_t timestamp) {
    if (channel == midiChannel || channel == MIDI_CHANNEL_ALL) {
        bleMidiData->controls[number] = value * 2;
        LED_dataInBlink();
        Serial.printf("control change %d %d\n", number, value);
    }
}

static void handleNoteOff(byte channel, byte note, byte velocity, uint16_t timestamp) {
    if (channel == midiChannel || channel == MIDI_CHANNEL_ALL) {
        bleMidiData->noteOn[note] = 0;
        Serial.printf("note off %d\n", note);
    }
}

static void handleNoteOn(byte channel, byte note, byte velocity, uint16_t timestamp) {
    if (channel == midiChannel || channel == MIDI_CHANNEL_ALL) {
        if (velocity == 0) {
            handleNoteOff(channel, note, velocity, timestamp);
            return;
        }

        bleMidiData->noteOn[note] = 2 * velocity;

        LED_dataInBlink();
        Serial.printf("note on %d, velocity %d\n", note, velocity);
    }
}


// public

void MIDICBLE_init(const byte _midiChannel, MidiData* _bleMidiData) {
    midiChannel = _midiChannel;
    bleMidiData = _bleMidiData;
    char name[16];
    sprintf(name, "RSGLED%02d", midiChannel);

    Serial.println("init BleController");
    BLEMidiServer.begin(name);

    BLEMidiServer.setOnConnectCallback(handleConnect);
    BLEMidiServer.setOnDisconnectCallback(handleDisconnect);
    BLEMidiServer.setControlChangeCallback(handleControlChange);
    BLEMidiServer.setNoteOnCallback(handleNoteOn);
    BLEMidiServer.setNoteOffCallback(handleNoteOff);

    // waiting for connection...
    onAdvertisingStart();
}

MidiData *MIDICBLE_read() {
    return bleMidiData;
}

void MIDICBLE_loop() {
    connectionLED.loop();
    midiLED.loop();
}
