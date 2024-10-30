#ifndef FXGRADIENTFADE_H
#define FXGRADIENTFADE_H

#include <fxBase.h>
#include "helper.h"

class FXGradientFade final : public FXBase {

public:
    explicit FXGradientFade(const byte TRIGGER_NOTE) : FXBase(TRIGGER_NOTE) {}

    void makeEffect(LEDConfig &ledConfig, const byte velocity) override {
        if (startMillis == ledConfig.timestamp) {
            fill_gradient_RGB(ledConfig.fullGradientLEDs, FULL_GRADIENT_STEPS, COLOR_1, COLOR_6, COLOR_1);
            gradientFadeLastStep = 0;
            gradientFadeLastUpdateTime = ledConfig.timestamp;
        }

        const double period = static_cast<double>(helper_getBeatLenInMillis(ledConfig.tempo, 64)) * ledConfig.tempoTrim;

        unsigned long elapsedTime = ledConfig.timestamp - gradientFadeLastUpdateTime;

        double progress = static_cast<double>(elapsedTime) / period;
        gradientFadeLastStep = (gradientFadeLastStep + static_cast<unsigned int>(progress)) % FULL_GRADIENT_STEPS;

        if (progress >= 1.0) {
            gradientFadeLastUpdateTime = ledConfig.timestamp;
        }

        ledConfig.LED_all_on(&ledConfig.fullGradientLEDs[gradientFadeLastStep % FULL_GRADIENT_STEPS], velocity);
    };

    void onReset() override {
        gradientFadeLastStep = 0;
        gradientFadeLastUpdateTime = 0;
    };

    void onStart(LEDConfig &ledConfig) override {
       
    }

    void onFinish(LEDConfig &ledConfig) override {
        gradientFadeLastStep = 0;
        gradientFadeLastUpdateTime = 0;
    }

private:
    unsigned int gradientFadeLastStep = 0;
    unsigned long gradientFadeLastUpdateTime = 0;
};

#endif //FXGRADIENTFADE_H
