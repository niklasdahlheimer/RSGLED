#ifndef FX_COLOR_ALL_H
#define FX_COLOR_ALL_H

#include <fxBase.h>

class FXColorAll final : public FXBase {
public:
    DEFINE_GETNAME(FXColorAll)

    explicit FXColorAll(const byte TRIGGER_NOTE) : FXBase(TRIGGER_NOTE) {
        this->color = DEFAULT_COLOR;
    }

    FXColorAll(byte TRIGGER_NOTE, const CRGB *color) : FXBase(TRIGGER_NOTE) {
        this->color = color;
    }

    void makeEffect(LEDConfig &ledConfig, const byte velocity) override {
        if (ledConfig.timestamp - startMillis < ledConfig.fadeInTime) {
            const byte fadedVelocity = map(ledConfig.timestamp - startMillis, 0, ledConfig.fadeInTime, 0, velocity);
            //Serial.printf("set velocity to %d from %d time %d\n", fadedVelocity, velocity, ledConfig.timestamp - startMillis);
            ledConfig.allOn(color, fadedVelocity);
        } else {
            ledConfig.allOn(color, velocity);
        }
    };

    void onReset() override {
    };

    void onStart(LEDConfig &ledConfig) override {
    }

    void onFinish(LEDConfig &ledConfig) override {
    }

private:
    const CRGB *color;
};

#endif //FX_COLOR_ALL_H
