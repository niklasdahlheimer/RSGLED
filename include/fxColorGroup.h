#ifndef FX_COLOR_GROUP_H
#define FX_COLOR_GROUP_H

#include <fxBase.h>

class FXColorGroup final : public FXBase {
public:
    DEFINE_GETNAME(FXColorGroup)

    explicit FXColorGroup(const byte TRIGGER_NOTE) : FXBase(TRIGGER_NOTE) {
        this->color = DEFAULT_COLOR;
        this->groupIndex = 0;
    }

    FXColorGroup(byte TRIGGER_NOTE, const CRGB* color, const byte groupIndex) : FXBase(TRIGGER_NOTE) {
        this->color = color;
        this->groupIndex = groupIndex;
    }
    
    void makeEffect(LEDConfig &ledConfig, const byte velocity) override {
        ledConfig.groupOn(ledConfig.groups[groupIndex], color, velocity);
    };

    void onReset() override {

    };

    void onStart(LEDConfig &ledConfig) override {

    }

    void onFinish(LEDConfig &ledConfig) override {

    }

private:
   const CRGB* color;
   byte groupIndex;
};

#endif //FX_COLOR_GROUP_H
