#ifndef FXGRADIENTFADE_H
#define FXGRADIENTFADE_H

#include <effect.h>
#include "helper.h"

class EffectGradientFade final : public Effect {
public:
    explicit EffectGradientFade(const byte TRIGGER_NOTE) : Effect(TRIGGER_NOTE) {}

    void makeEffect(LEDConfig &ledConfig, const byte velocity) override {
        // Initialisierung bei Start des Gradientenverlaufs
        if (startMillis == ledConfig.timestamp) {
            fill_gradient_RGB(ledConfig.fullGradientLEDs, FULL_GRADIENT_STEPS, COLOR_1, COLOR_6, COLOR_1);
            gradientFadeLastStep = 0; // Setze den Startschritt
            gradientFadeLastUpdateTime = ledConfig.timestamp; // Setze den Startzeitpunkt
        }

        // Berechne die Periode basierend auf dem geglätteten TempoTrim
        const double period = static_cast<double>(helper_getBeatLenInMillis(ledConfig.tempo, 64)) * ledConfig.tempoTrim;

        // Berechne die verstrichene Zeit seit der letzten Aktualisierung
        unsigned long elapsedTime = ledConfig.timestamp - gradientFadeLastUpdateTime;

        // Berechne den Fortschritt basierend auf der verstrichenen Zeit und der geglätteten Periode
        double progress = static_cast<double>(elapsedTime) / period;
        gradientFadeLastStep = (gradientFadeLastStep + static_cast<unsigned int>(progress)) % FULL_GRADIENT_STEPS;

        // Aktualisiere die Zeit, wenn ein Schritt gemacht wurde
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
