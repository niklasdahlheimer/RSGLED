#include "midiController.h"
#include "ledController.h"
#include <Arduino.h>
#include <MIDI.h>
#include "defines.h"

MIDI_CREATE_INSTANCE(HardwareSerial, Serial, MIDI);


MidiData data;

void handleSystemExclusive(byte *array, unsigned size);

void handleSongPosition(unsigned int beats);

void handleSongSelect(byte songNumber);

void handleTuneRequest();

void handleControlChange(byte channel, byte number, byte value);

void handleProgramChange(byte channel, byte number);

void handlePitchBend(byte channel, int bend);

void handleNoteOn(byte channel, byte note, byte velocity);

void handleAfterTouchPoly(byte channel, byte note, byte pressure);

void handleNoteOff(byte channel, byte note, byte velocity);

void handleError(int8_t error);

void MIDIC_init() {
    // Create and bind the MIDI interface to the default hardware Serial port
    //midiSerial.begin(31250); // MIDI Baudrate für den SoftwareSerial Port
    MIDI.begin(MIDI_CHANNEL);
    MIDI.turnThruOff();
    MIDI.setHandleControlChange(handleNoteOn);
    MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleNoteOff(handleNoteOff);
    MIDI.setHandleError(handleError);
    MIDI.setHandleSystemExclusive(handleSystemExclusive);
    MIDI.setHandleAfterTouchPoly(handleAfterTouchPoly);
}

MidiData *MIDIC_read() {
    MIDI.read(MIDI_CHANNEL);
    return &data;
}

// private

void handleSystemExclusive(byte *array, unsigned size) {
    /*serialPrintf(midiLogSerial, "SysEx:");
    for(unsigned int i = 0; i < size; i++) {
        serialPrintf(midiLogSerial, " %02X", array[i]);
    }*/
}

void handleControlChange(byte channel, byte number, byte value) {
    data.controls[number] = value;
}

void handleNoteOn(byte channel, byte note, byte velocity) {
    //serialPrintf(midiLogSerial, "NoteOn: %d %d %d", channel, note, velocity);
    data.noteOn[note] = 2 * velocity;
    digitalWrite(LED_BUILTIN, HIGH);
}

void handleAfterTouchPoly(byte channel, byte note, byte pressure) {
    data.noteOn[note] = 2*pressure;
}

void handleNoteOff(byte channel, byte note, byte velocity) {
    //serialPrintf(midiLogSerial, "NoteOff: %d %d %d", channel, note, velocity);
    data.noteOn[note] = 0;
    digitalWrite(LED_BUILTIN, LOW);
}

void handleError(int8_t error) {
    /*serialPrintf(midiLogSerial, "MIDI error: %d", error);*/
};
