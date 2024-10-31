#ifndef FX_BREATH_H
#define FX_BREATH_H

#include <fxBase.h>

#define BREATH_PERIOD_IN_MILLIS     5000

class OverlayBreath final : public FXBase {
public:
    explicit OverlayBreath(const byte TRIGGER_NOTE) : FXBase(TRIGGER_NOTE) {}

    void makeEffect(LEDConfig &ledConfig, const byte velocity) override {
        const double timeFactor = (1 +
                                   sin(2 * M_PI * static_cast<double>(ledConfig.timestamp - startMillis) / BREATH_PERIOD_IN_MILLIS -
                                       M_PI / 2))/2; // between 0 and 1
        const double currBrightness = (velocity / 255.0) * timeFactor * LED_BRIGHTNESS_MAX;
        ledConfig.allBrighten(static_cast<byte>(currBrightness));
    };

    void onReset() override {
		
    };

    void onStart(LEDConfig &ledConfig) override {
       
    }

    void onFinish(LEDConfig &ledConfig) override {
       
    }


};

#endif //FX_BREATH_H
