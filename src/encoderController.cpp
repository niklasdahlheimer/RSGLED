#include "encoderController.h"
#include "AiEsp32RotaryEncoder.h"

AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ENCODER_PIN_A, ENCODER_PIN_B, ENCODER_BUTTON_PIN, -1, 2);

byte currentModeIndex = 0;

State* statePointer;

#define ACTIVE_MILLIS 2000

#define NUM_OF_MODES 4
ModeConfig ModeConfigs[NUM_OF_MODES] = {
    {INITIAL, 255},
    {BRIGHTNESS, 255},
    {TEST, 255},
    {LINE, 0},
};

void IRAM_ATTR readEncoderISR() {
    rotaryEncoder.readEncoder_ISR();
}

void ENCODER_init(State* state){
    statePointer = state;

    pinMode(ENCODER_PIN_A, INPUT_PULLUP);
    pinMode(ENCODER_PIN_B, INPUT_PULLUP);

    rotaryEncoder.areEncoderPinsPulldownforEsp32 = false;
    rotaryEncoder.setup(readEncoderISR);
    rotaryEncoder.setBoundaries(0, 255, false);
    rotaryEncoder.begin();
}

void ENCODER_loop(){
    if (rotaryEncoder.isEncoderButtonClicked()) {
        currentModeIndex = (currentModeIndex + 1) % NUM_OF_MODES;
        statePointer->mode = ModeConfigs[currentModeIndex].mode;
        statePointer->value = ModeConfigs[currentModeIndex].value;
        statePointer->lastChanged = millis();
        statePointer->active = true;
        rotaryEncoder.reset(statePointer->value);
        Serial.printf("enc mode: %d, value: %d\n", statePointer->mode, statePointer->value);
    }
    if (rotaryEncoder.encoderChanged()) {
        int encValue = (int) rotaryEncoder.readEncoder();
        ModeConfigs[currentModeIndex].value = encValue;
        statePointer->value = encValue;
        statePointer->lastChanged = millis();
        Serial.printf("enc value: %d\n", encValue);
    }

    if(millis() - statePointer->lastChanged > ACTIVE_MILLIS){
        statePointer->active = false;
    }

}
