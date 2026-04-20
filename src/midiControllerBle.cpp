#include "midiController.h"
#include "logging.h"
#include <Arduino.h>
#include <BLEMidi.h>
#include <midiConsts.h>

#include "led.h"
#include "esp_bt.h"


static MidiData *bleMidiData;
static byte midiChannel;
static unsigned long lastNoteOn = 0;

// private

static void onAdvertisingStart() {
    LED_blinkSlow(&connectionLED);
}

static void handleConnect() {
    LOGN("BLE MIDI: Controller connected!");
    LOGD("BLE MIDI: listening for midi on channel %d and %d\n", midiChannel, MIDI_CHANNEL_ALL);
    connectionLED.turnON();
}

static void handleDisconnect() {
    LOGN("BLE MIDI: Controller disconnected!");
    onAdvertisingStart();
}

static void handleAllNoteOff() {
    LOGD("BLE MIDI: All Note Off command received!\n");
    for (unsigned char &i: bleMidiData->noteOn) {
        i = 0;
    }
}

static void handleControlChange(byte channel, byte number, byte value, uint16_t timestamp) {
    LOGD("BLE MIDI: control change %d, value %d, channel %d\n", number, value, channel + 1);
    if (channel + 1 != midiChannel && channel + 1 != MIDI_CHANNEL_ALL) {
        return;
    }

    bleMidiData->controls[number] = value * 2;
    LED_blinkOnce(&midiLED);


    if (number == ALL_NOTE_OFF_CC && value == ALL_NOTE_OFF_VAL) {
        handleAllNoteOff();
    }
}

static void handleNoteOff(byte channel, byte note, byte velocity, uint16_t timestamp) {
    LOGD("BLE MIDI: note off %d, channel %d\n", note, channel + 1);
    if (channel + 1 != midiChannel && channel + 1 != MIDI_CHANNEL_ALL) {
        return;
    }

    bleMidiData->noteOn[note] = 0;
}

static void handleNoteOn(byte channel, byte note, byte velocity, uint16_t timestamp) {
    LOGD("BLE MIDI: note on %d, velocity %d, channel %d\n", note, velocity, channel + 1);
    if (channel + 1 != midiChannel && channel + 1 != MIDI_CHANNEL_ALL) {
        return;
    }

    if (velocity == 0) {
        handleNoteOff(channel, note, velocity, timestamp);
        return;
    }

    lastNoteOn = millis();
    bleMidiData->noteOn[note] = 2 * velocity;

    LED_blinkOnce(&midiLED);
}


// public

void MIDICBLE_init(const byte _midiChannel, char letter, MidiData *_bleMidiData) {
    midiChannel = _midiChannel;
    bleMidiData = _bleMidiData;
    char name[16];
    sprintf(name, "RSG_LED_%c", letter);

    LOGN("BLE MIDI: init BleController");
    BLEMidiServer.begin(name);
    BLEMidiServer.setOnConnectCallback(handleConnect);
    BLEMidiServer.setOnDisconnectCallback(handleDisconnect);
    BLEMidiServer.setControlChangeCallback(handleControlChange);
    BLEMidiServer.setNoteOnCallback(handleNoteOn);
    BLEMidiServer.setNoteOffCallback(handleNoteOff);

    // set BLE power level
    esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_DEFAULT, ESP_PWR_LVL_P9);
    // waiting for connection...
    onAdvertisingStart();
}

unsigned long MIDICBLE_lastNoteOn() {
    return lastNoteOn;
}

MidiData *MIDICBLE_getMidiDataArray() {
    return bleMidiData;
}

void MIDICBLE_loop() {
    /*if (BLEMidiServer.isConnected() && connectionLED.getState() != LED_BLINKING) {
        connectionLED.turnON();
    }*/
    connectionLED.loop();
    midiLED.loop();
}
