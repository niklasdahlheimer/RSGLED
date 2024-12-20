#include "midiController.h"
#include "ledController.h"
#include <Arduino.h>
#include <MIDI.h>
#include "led.h"

MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, MIDI);

static MidiData* midiData;
static byte midiChannel;

void handleControlChange(byte channel, byte number, byte value);

void handlePitchBend(byte channel, int bend);

void handleNoteOn(byte channel, byte note, byte velocity);

void handleAfterTouchPoly(byte channel, byte note, byte pressure);

void handleNoteOff(byte channel, byte note, byte velocity);

void handleError(int8_t error);

void MIDIC_init(const byte _midiChannel, MidiData* _midiData) {
    midiChannel = _midiChannel;
    midiData = _midiData;

    MIDI.begin(midiChannel);
    MIDI.turnThruOff();
    MIDI.setHandleControlChange(handleNoteOn);
    MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleNoteOff(handleNoteOff);
    MIDI.setHandleError(handleError);
    MIDI.setHandleAfterTouchPoly(handleAfterTouchPoly);
}

MidiData *MIDIC_read() {
    MIDI.read(midiChannel);
    return midiData;
}

// private

void handleControlChange(byte channel, byte number, byte value) {
    midiData->controls[number] = value;
    Serial.printf("control change on %03d, value %03d", number, value);
}

void handleNoteOn(byte channel, byte note, byte velocity) {
    midiData->noteOn[note] = 2 * velocity;
    LED_dataInBlink();
    Serial.printf("note on %03d, velocity %03d", note, velocity);
}

void handleAfterTouchPoly(byte channel, byte note, byte pressure) {
    //data.noteOn[note] = 2 * pressure;
    Serial.printf("after touch on %03d, pressure %03d", note, pressure);
}

void handleNoteOff(byte channel, byte note, byte velocity) {
    //serialPrintf(midiLogSerial, "NoteOff: %d %d %d", channel, note, velocity);
    midiData->noteOn[note] = 0;
    Serial.printf("note off %03d", note);
}

void handleError(int8_t error) {
    Serial.printf("MIDI error %03d", error);
};
