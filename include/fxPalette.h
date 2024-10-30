#ifndef FXPALETTE_H
#define FXPALETTE_H

#include <fxBase.h>
#include "helper.h"

#define RAINBOW_PERIOD_IN_MILLIS    10

static CRGBPalette16 palettes[] = {
    OceanColors_p, RainbowColors_p, PartyColors_p, HeatColors_p, LavaColors_p, CloudColors_p
};

class FXPalette final : public FXBase {
public:
    explicit FXPalette(const byte TRIGGER_NOTE) : FXBase(TRIGGER_NOTE) {
    }

    void makeEffect(LEDConfig &ledConfig, const byte velocity) override {
        const unsigned int step = helper_getSteppedSawValue(ledConfig.timestamp - startMillis,
                                                   helper_getBeatLenInMillis(ledConfig.tempo, 64),
                                                   ledConfig.LINE_NUM);
        // circling offset
        for (int i = 0; i < ledConfig.LINE_NUM; i++) {
            byte paletteIndex = map(i, 0, ledConfig.LINE_NUM, 0, 255);
            CRGB colorFromPalette = ColorFromPalette(palettes[currentPalette], paletteIndex);
            ledConfig.lineOn(ledConfig.lines[(i + step) % ledConfig.LINE_NUM], &colorFromPalette, velocity);
        }
    };

    void onReset() override {
    };

    void onStart(LEDConfig &ledConfig) override {
        currentPalette++;
    }

    void onFinish(LEDConfig &ledConfig) override {
    }

private:
  byte currentPalette = 0;

};

#endif //FXPALETTE_H
