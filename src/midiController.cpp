#include "midiController.h"
#include "logging.h"
#include <Arduino.h>
#include <MIDI.h>
#include "led.h"
#include "midiConsts.h"

MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, MIDI);

static MidiData *midiData;
static byte midiChannel;
static unsigned long lastNoteOn = 0;

void handleControlChange(byte channel, byte number, byte value);

void handleNoteOn(byte channel, byte note, byte velocity);

void handleNoteOff(byte channel, byte note, byte velocity);

void handleError(int8_t error);

void MIDIC_init(const byte _midiChannel, MidiData *_midiData) {
    LOGD("Cable MIDI: Init Cable MIDI Controller\n");
    midiChannel = _midiChannel;
    midiData = _midiData;

    MIDI.begin(MIDI_CHANNEL_OMNI);
    MIDI.setHandleControlChange(handleControlChange);
    MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleNoteOff(handleNoteOff);
    MIDI.setHandleError(handleError);
}

MidiData *MIDIC_read() {
    MIDI.read();

    connectionLED.loop();
    midiLED.loop();

    return midiData;
}

// private

void handleAllNoteOff() {
    LOGD("Cable MIDI: All Note Off command received!\n");
    for (unsigned char &i: midiData->noteOn) {
        i = 0;
    }
}

void handleControlChange(const byte channel, const byte number, const byte value) {
    LOGD("Cable MIDI: Control change on %03d, value %03d, channel %d\n", number, value, channel);
    if (channel != midiChannel && channel != MIDI_CHANNEL_ALL) {
        return;
    }

    midiData->controls[number] = value * 2;
    LED_blinkOnce(&midiLED);

    if (number == ALL_NOTE_OFF_CC && value == ALL_NOTE_OFF_VAL) {
        handleAllNoteOff();
    }
}

void handleNoteOn(const byte channel, const byte note, const byte velocity) {
    LOGD("Cable MIDI: Note on %03d, velocity %03d, channel %d\n", note, velocity, channel);
    if (channel != midiChannel && channel != MIDI_CHANNEL_ALL) {
        return;
    }

    midiData->noteOn[note] = 2 * velocity;
    lastNoteOn = millis();
    LED_blinkOnce(&midiLED);
}

void handleNoteOff(const byte channel, const byte note, byte velocity) {
    LOGD("Cable MIDI: Note off %03d, channel %d\n", note, channel);
    if (channel != midiChannel && channel != MIDI_CHANNEL_ALL) {
        return;
    }

    midiData->noteOn[note] = 0;
}

void handleError(const int8_t error) {
    LOGD("Cable MIDI: MIDI error %03d\n", error);
};

unsigned long MIDIC_lastNoteOn() {
    return lastNoteOn;
}
