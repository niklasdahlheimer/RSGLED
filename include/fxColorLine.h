#ifndef FX_COLOR_LINE_H
#define FX_COLOR_LINE_H

#include <fxBase.h>

class FXColorLine final : public FXBase {
public:
    DEFINE_GETNAME(FXColorGroup)

    explicit FXColorLine(const byte TRIGGER_NOTE) : FXBase(TRIGGER_NOTE) {
        this->color = DEFAULT_COLOR;
    }

    FXColorLine(byte TRIGGER_NOTE, const CRGB *color) : FXBase(TRIGGER_NOTE) {
        this->color = color;
    }

    void makeEffect(LEDConfig &ledConfig, const byte velocity) override {
        if (ledConfig.controller[CONTROLLER_LINE_INDEX] / 2 == 0) return;
        ledConfig.lineOn(ledConfig.lines[((ledConfig.controller[CONTROLLER_LINE_INDEX] / 2) - 1) % ledConfig.LINE_NUM],
                         color,
                         velocity);
        Serial.printf("set velocity to %d from %d time %d\n", velocity, velocity, ledConfig.timestamp - startMillis);
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

#endif //FX_COLOR_LINE_H
