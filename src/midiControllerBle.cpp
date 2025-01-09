#include "midiController.h"
#include <Arduino.h>
#include <BLEMidi.h>
#include "led.h"

static MidiData* bleMidiData;
static byte midiChannel;
static unsigned long lastNoteOn = 0;


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
        //lastReceived = millis();
        bleMidiData->controls[number] = value * 2;
        LED_dataInBlink();
        Serial.printf("control change %d %d\n", number, value);
    }
    if(number == 123 && value == 0){
        Serial.printf("All Note Off command received!\n");
        for (int i = 0; i < 255; ++i) {
            bleMidiData->noteOn[i] = 0;
        }
    }
}

static void handleNoteOff(byte channel, byte note, byte velocity, uint16_t timestamp) {
    if (channel == midiChannel || channel == MIDI_CHANNEL_ALL) {
        //lastReceived = millis();
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

        lastNoteOn = millis();
        bleMidiData->noteOn[note] = 2 * velocity;

        LED_dataInBlink();
        Serial.printf("note on %d, velocity %d\n", note, velocity);
    }
}


// public

void MIDICBLE_init(const byte _midiChannel, char letter, MidiData* _bleMidiData) {
    midiChannel = _midiChannel;
    bleMidiData = _bleMidiData;
    char name[16];
    sprintf(name, "RSG_LED_%c", letter);

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

unsigned long MIDICBLE_lastNoteOn() {
    return lastNoteOn;
}

MidiData *MIDICBLE_read() {
    return bleMidiData;
}

void MIDICBLE_loop() {
    connectionLED.loop();
    midiLED.loop();
}
