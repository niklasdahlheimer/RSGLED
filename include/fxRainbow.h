#ifndef FX_RAINBOW_H
#define FX_RAINBOW_H

#include <fxBase.h>

#define RAINBOW_PERIOD_IN_MILLIS    10

class FXRainbow final : public FXBase {
public:
    DEFINE_GETNAME(FXRainbow)

    explicit FXRainbow(const byte TRIGGER_NOTE) : FXBase(TRIGGER_NOTE) {
    }

    void makeEffect(LEDConfig &ledConfig, const byte velocity) override {
        fill_rainbow_circular(ledConfig.LEDs,
                              ledConfig.LED_NUM,
                              ((ledConfig.timestamp - startMillis) / RAINBOW_PERIOD_IN_MILLIS),
                              (uint8_t) (10 * (velocity / 127.0)));
    };

    void onReset() override {
    };

    void onStart(LEDConfig &ledConfig) override {
    }

    void onFinish(LEDConfig &ledConfig) override {
    }

private:
};

#endif //FX_RAINBOW_H
