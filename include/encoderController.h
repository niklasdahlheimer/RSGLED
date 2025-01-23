#ifndef ENCODERCONTROLLER_H
#define ENCODERCONTROLLER_H

#include <Arduino.h>

#define ENCODER_PIN_A         22
#define ENCODER_PIN_B         23
#define ENCODER_BUTTON_PIN    33


enum MODE {
    INITIAL,
    BRIGHTNESS,
    TEST,
    LINE
};

typedef struct {
    MODE mode;
    int value;
} ModeConfig;

typedef struct {
    MODE mode;
    int value;
    long lastChanged;
    bool active;
} State;

void ENCODER_init(State* state);

void ENCODER_loop();

State ENCODER_getState();


#endif //ENCODERCONTROLLER_H
