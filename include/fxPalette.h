#ifndef FX_PALETTE_H
#define FX_PALETTE_H

#include <fxBase.h>

class FXPalette final : public FXBase {
public:
    DEFINE_GETNAME(FXPalette)

    explicit FXPalette(const byte TRIGGER_NOTE) : FXBase(TRIGGER_NOTE) {
    }

    explicit FXPalette(const byte TRIGGER_NOTE, CRGBPalette16 palette) : FXBase(TRIGGER_NOTE) {
        p = palette;
    }

    void makeEffect(LEDConfig &ledConfig, const byte velocity) override {
        const unsigned int step = getSteppedSawValue(ledConfig.timestamp - startMillis,
                                                     getBeatLenInMillis(ledConfig.tempo, 64),
                                                     ledConfig.LINE_NUM);
        // circling offset
        for (int i = 0; i < ledConfig.LINE_NUM; i++) {
            byte palettePos = map(i, 0, ledConfig.LINE_NUM, 0, 255);
            CRGB colorFromPalette = ColorFromPalette(p, palettePos);
            ledConfig.lineOn(ledConfig.lines[(i + step) % ledConfig.LINE_NUM], &colorFromPalette, velocity);
        }
    };

    void onReset() override {
    };

    void onStart(LEDConfig &ledConfig) override {
    }

    void onFinish(LEDConfig &ledConfig) override {
    }

private:
    CRGBPalette16 p = OceanColors_p;
};

#endif //FX_PALETTE_H
