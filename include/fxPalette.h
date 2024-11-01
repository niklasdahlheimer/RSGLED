#ifndef FX_PALETTE_H
#define FX_PALETTE_H

#include <fxBase.h>
#include "helper.h"

CRGBPalette16 palettes[] = {
    OceanColors_p, RainbowColors_p, PartyColors_p, HeatColors_p, LavaColors_p, CloudColors_p
};

class FXPalette final : public FXBase {
public:
    DEFINE_GETNAME(FXPalette)

    explicit FXPalette(const byte TRIGGER_NOTE) : FXBase(TRIGGER_NOTE) {
    }

    void makeEffect(LEDConfig &ledConfig, const byte velocity) override {
        const unsigned int step = helper_getSteppedSawValue(ledConfig.timestamp - startMillis,
                                                   helper_getBeatLenInMillis(ledConfig.tempo, 64),
                                                   ledConfig.LINE_NUM);
        // circling offset
        for (int i = 0; i < ledConfig.LINE_NUM; i++) {
            byte palettePos = map(i, 0, ledConfig.LINE_NUM, 0, 255);
            CRGB colorFromPalette = ColorFromPalette(palettes[currentPalette], palettePos);
            ledConfig.lineOn(ledConfig.lines[(i + step) % ledConfig.LINE_NUM], &colorFromPalette, velocity);
        }
    };

    void onReset() override {
    };

    void onStart(LEDConfig &ledConfig) override {
    }

    void onFinish(LEDConfig &ledConfig) override {
        currentPalette++;
    }

private:
  byte currentPalette = 0;

};

#endif //FX_PALETTE_H
