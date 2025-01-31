#include "midiController.h"
#include "ledController.h"
#include <Arduino.h>
#include <MIDI.h>
#include "led.h"

MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, MIDI);

static MidiData *midiData;
static byte midiChannel;
static unsigned long lastNoteOn = 0;

void handleControlChange(byte channel, byte number, byte value);

void handleNoteOn(byte channel, byte note, byte velocity);

void handleNoteOff(byte channel, byte note, byte velocity);

void handleError(int8_t error);

void MIDIC_init(const byte _midiChannel, MidiData *_midiData) {
    midiChannel = _midiChannel;
    midiData = _midiData;

    MIDI.begin(midiChannel);
    MIDI.setHandleControlChange(handleNoteOn);
    MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleNoteOff(handleNoteOff);
    MIDI.setHandleError(handleError);
}

MidiData *MIDIC_read() {
    MIDI.read(midiChannel);
    return midiData;
}

// private

void handleAllNoteOff() {
    Serial.printf("All Note Off command received!\n");
    for (unsigned char & i : midiData->noteOn) {
        i = 0;
    }
}

void handleControlChange(const byte channel, const byte number, const byte value) {
    midiData->controls[number] = value * 2;
    Serial.printf("control change on %03d, value %03d\n", number, value);

    if(number == ALL_NOTE_OFF_CC && value == ALL_NOTE_OFF_VAL){
        handleAllNoteOff();
    }
}

void handleNoteOn(const byte channel, const byte note, const byte velocity) {
    midiData->noteOn[note] = 2 * velocity;
    LED_dataInBlink();
    lastNoteOn = millis();
    Serial.printf("note on %03d, velocity %03d\n", note, velocity);
}

void handleNoteOff(const byte channel, const byte note, byte velocity) {
    midiData->noteOn[note] = 0;
    Serial.printf("note off %03d\n", note);
}

void handleError(const int8_t error) {
    Serial.printf("MIDI error %03d\n", error);
};

unsigned long MIDIC_lastNoteOn() {
    return lastNoteOn;
}
