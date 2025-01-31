#ifndef FX_FREE_RUN_2_H
#define FX_FREE_RUN_2_H

#include <fxBase.h>

#define FREE_RUN_DIM_IN_MILLIS 3000
#define FREE_RUN_BRIGHTNESS 40

const CRGBPalette16 p16 = {
    CRGB::DarkSlateBlue,
    CRGB::RoyalBlue,
    CRGB::DarkSlateBlue,
    CRGB::SteelBlue,

    CRGB::RoyalBlue,
    CRGB::DodgerBlue,
    CRGB::SkyBlue,
    CRGB::LightBlue,

    CRGB::SteelBlue,
    CRGB::DeepSkyBlue,
    CRGB::LightCyan,
    CRGB::SteelBlue,

    CRGB::PaleTurquoise,
    CRGB::MediumAquamarine,
    CRGB::LightCyan,
    CRGB::LightBlue
};

class FXFreeRun2 final : public FXBase {
public:
    DEFINE_GETNAME(FXFreeRun2)

    explicit FXFreeRun2(const byte TRIGGER_NOTE) : FXBase(TRIGGER_NOTE) {
        UpscalePalette(p16, p256);
    }

    void makeEffect(LEDConfig &ledConfig, const byte velocity) override {
        colorOffset = beat8(colorSpeed);
        brightness = FREE_RUN_BRIGHTNESS * getRampValue(millis() - startMillis, FREE_RUN_DIM_IN_MILLIS);

        for (int i = 0; i < ledConfig.LINE_NUM; i++) {
            palettePos = (i * 190 / ledConfig.LINE_NUM) - colorOffset;
            color = ColorFromPalette(p256, palettePos);
            ledConfig.lineOn(ledConfig.lines[i], &color, brightness);
        }
    }

    void onReset() override {
    }

    void onStart(LEDConfig &ledConfig) override {
    }

    void onFinish(LEDConfig &ledConfig) override {
    }

private:
    CRGBPalette256 p256;
    uint8_t colorSpeed = 5;
    uint8_t brightness;
    CRGB color;
    uint8_t palettePos;
    uint8_t colorOffset;
};

#endif //FX_FREE_RUN_2_H
