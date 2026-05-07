#ifndef FX_COLOR_LEVEL_H
#define FX_COLOR_LEVEL_H

#include <fxBase.h>

class FXColorLevel final : public FXBase {
public:
    DEFINE_GETNAME(FXColorLevel)

    explicit FXColorLevel(const byte TRIGGER_NOTE) : FXBase(TRIGGER_NOTE) {
        this->color = DEFAULT_COLOR;
        this->levelIndex = 0;
    }

    FXColorLevel(const byte TRIGGER_NOTE, const CRGB *color, const byte groupIndex) : FXBase(TRIGGER_NOTE) {
        this->color = color;
        this->levelIndex = groupIndex;
    }

protected:
    void makeEffect(LEDConfig &ledConfig, const byte velocity) override {
        // Horizontal Segment Blocks (for Level Meter etc.)
        ledConfig.levelOn(levelIndex, color, velocity);
    }

    void onReset() override {
    }

    void onStart(LEDConfig &ledConfig) override {
    }

    void onFinish(LEDConfig &ledConfig) override {
    }

private:
    const CRGB *color;
    byte levelIndex;
};

#endif //FX_COLOR_LEVEL_H
