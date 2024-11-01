#ifndef FX_WHITE_SEGMENTS_H
#define FX_WHITE_SEGMENTS_H

#include <fxBase.h>
#include "helper.h"

const byte NUM_OF_STEPS = 20;
const byte GROUPS_PER_STEP = 5;
const CRGB color = CRGB::White;

byte groups[NUM_OF_STEPS][GROUPS_PER_STEP] = {
    {1, 4, 6, 7, 9},    {3, 5, 7, 9, 10},    {2, 3, 5, 8, 10},     {1, 4, 6, 9, 10},    {1, 2, 5, 7, 10},
    {3, 4, 6, 8, 9},    {2, 4, 6, 7, 9},    {1, 3, 5, 8, 10},    {1, 3, 4, 7, 10},    {2, 5, 6, 8, 9},
    {2, 4, 5, 6, 10},    {1, 3, 7, 8, 9},    {1, 4, 5, 6, 8},    {2, 3, 7, 9, 10},    {2, 5, 6, 7, 8},
    {1, 3, 4, 9, 10},    {1, 2, 3, 7, 8},    {4, 5, 6, 9, 10},    {3, 4, 5, 6, 7},    {1, 2, 8, 9, 10}
};

class FXWhiteSegments final : public FXBase {
public:
    DEFINE_GETNAME(FXWhiteSegments)

    explicit FXWhiteSegments(const byte TRIGGER_NOTE) : FXBase(TRIGGER_NOTE) {}

    void makeEffect(LEDConfig &ledConfig, const byte velocity) override {
        const unsigned int currentStep =
                     helper_getSteppedSawValue(ledConfig.timestamp - startMillis, helper_getBeatLenInMillis(ledConfig.tempo, 4), NUM_OF_STEPS);

        for(int i = 0; i < GROUPS_PER_STEP; ++i){
            ledConfig.groupOn(ledConfig.groups[groups[currentStep][i]], &color , velocity);
        }
    };

    void onReset() override {

    };

    void onStart(LEDConfig &ledConfig) override {

    }

    void onFinish(LEDConfig &ledConfig) override {

    }

private:

};

#endif //FX_WHITE_SEGMENTS_H
