#ifndef FX_BREATH_H
#define FX_BREATH_H

#include <effect.h>

class EffectBreath final : public Effect {
public:
    explicit EffectBreath(const byte TRIGGER_NOTE) : Effect(TRIGGER_NOTE) {}

    void makeEffect(LEDConfig &ledConfig, const byte velocity) override {
        const double timeFactor = (1 +
                                   sin(2 * M_PI * static_cast<double>(ledConfig.timestamp - startMillis) / BREATH_PERIOD_IN_MILLIS -
                                       M_PI / 2));
        const double currBrightness = (velocity / 127.0) * timeFactor * LED_BRIGHTNESS_MAX;
        ledConfig.LED_all_on(ledConfig.globalColor, static_cast<byte>(currBrightness));
    };

    void onReset() override {
		
    };

    void onStart(LEDConfig &ledConfig) override {
       
    }

    void onFinish(LEDConfig &ledConfig) override {
       
    }


};

#endif //FX_BREATH_H
