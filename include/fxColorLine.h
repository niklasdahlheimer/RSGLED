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
        //if (ledConfig.controller[CONTROLLER_LINE_INDEX] == 0) return;
        const int activeLine = ledConfig.controller[CONTROLLER_LINE_INDEX] % ledConfig.LINE_NUM;
        ledConfig.lineOn(ledConfig.lines[activeLine], color, velocity);
        Serial.printf("set line %d to velocity %d\n", activeLine, velocity);
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
