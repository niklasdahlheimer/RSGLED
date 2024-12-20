#ifndef FX_GRADIENTFADE_H
#define FX_GRADIENTFADE_H

#include <fxBase.h>

class FXGradientFade final : public FXBase {
public:
    DEFINE_GETNAME(FXGradientFade)

    explicit FXGradientFade(const byte TRIGGER_NOTE) : FXBase(TRIGGER_NOTE) {}

    void makeEffect(LEDConfig &ledConfig, const byte velocity) override {
        if (startMillis == ledConfig.timestamp) {
            fill_gradient_RGB(ledConfig.fullGradientLEDs, FULL_GRADIENT_STEPS, COLORS[0], COLORS[5], COLORS[0]);
            gradientFadeLastStep = 0;
            gradientFadeLastUpdateTime = ledConfig.timestamp;
        }

        const double period = static_cast<double>(getBeatLenInMillis(ledConfig.tempo, 64)) * ledConfig.tempoTrim;

        unsigned long elapsedTime = ledConfig.timestamp - gradientFadeLastUpdateTime;

        double progress = static_cast<double>(elapsedTime) / period;
        gradientFadeLastStep = (gradientFadeLastStep + static_cast<unsigned int>(progress)) % FULL_GRADIENT_STEPS;

        if (progress >= 1.0) {
            gradientFadeLastUpdateTime = ledConfig.timestamp;
        }

        ledConfig.allOn(&ledConfig.fullGradientLEDs[gradientFadeLastStep % FULL_GRADIENT_STEPS], velocity);
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

#endif //FX_GRADIENTFADE_H
