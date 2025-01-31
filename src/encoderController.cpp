#include "encoderController.h"
#include "AiEsp32RotaryEncoder.h"

AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ENCODER_PIN_A, ENCODER_PIN_B, ENCODER_BUTTON_PIN, -1, 2);

byte currentModeIndex = 0;

State *statePointer;

#define ACTIVE_MILLIS 2000

#define DEBOUNCE_INTERVAL 500

#define NUM_OF_MODES 5
ModeConfig ModeConfigs[NUM_OF_MODES] = {
    {RUN_BLE, 0, 0, 255, false},
    {BRIGHTNESS, 255, 0, 255, false},
    {TEST, 255, 0, 255, false},
    {LINE, 0, 0, 255, true},
    {RUN_CABLE, 0, 0, 255, false},
};

void IRAM_ATTR readEncoderISR() {
    rotaryEncoder.readEncoder_ISR();
}

void ENCODER_init(State *state) {
    statePointer = state;

    pinMode(ENCODER_PIN_A, INPUT_PULLUP);
    pinMode(ENCODER_PIN_B, INPUT_PULLUP);

    rotaryEncoder.areEncoderPinsPulldownforEsp32 = false;
    rotaryEncoder.setup(readEncoderISR);
    rotaryEncoder.setBoundaries(ModeConfigs[0].minValue, ModeConfigs[0].maxValue, ModeConfigs[0].circleValues);
    rotaryEncoder.begin();
}

void ENCODER_loop() {
    if (rotaryEncoder.isEncoderButtonClicked() &&
        millis() - statePointer->lastChanged > DEBOUNCE_INTERVAL) {
        // debounce
        currentModeIndex = (currentModeIndex + 1) % NUM_OF_MODES;
        statePointer->mode = ModeConfigs[currentModeIndex].mode;
        statePointer->value = ModeConfigs[currentModeIndex].value;
        statePointer->lastChanged = millis();
        statePointer->active = true;
        rotaryEncoder.setBoundaries(ModeConfigs[currentModeIndex].minValue, ModeConfigs[currentModeIndex].maxValue,
                                    ModeConfigs[currentModeIndex].circleValues);
        rotaryEncoder.reset(ModeConfigs[currentModeIndex].value);
        Serial.printf("enc mode: %d, value: %d\n", statePointer->mode, statePointer->value);
    }
    if (rotaryEncoder.encoderChanged()) {
        const int encValue = (int) rotaryEncoder.readEncoder();
        ModeConfigs[currentModeIndex].value = encValue;
        statePointer->value = encValue;
        statePointer->lastChanged = millis();
        Serial.printf("enc value: %d\n", encValue);
    }

    if (millis() - statePointer->lastChanged > ACTIVE_MILLIS) {
        statePointer->active = false;
    }
}
