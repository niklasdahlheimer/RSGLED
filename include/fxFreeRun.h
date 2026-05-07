#ifndef FX_FREE_RUN_2_H
#define FX_FREE_RUN_2_H

#include <fxBase.h>

const CRGBPalette16 p16blue = {
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

const CRGBPalette16 p16white = {
    CRGB::White,
   CRGB::Snow,
   CRGB::WhiteSmoke,
   CRGB::Gainsboro,

   CRGB::FloralWhite,
   CRGB::Ivory,
   CRGB::Seashell,
   CRGB::OldLace,

    CRGB::Linen,
    CRGB::AntiqueWhite,
    CRGB::Beige,
    CRGB::WhiteSmoke,

   CRGB::Silver,
   CRGB::Gainsboro,
   CRGB::WhiteSmoke,
   CRGB::White
};

class FXFreeRun final : public FXBase {
public:
    DEFINE_GETNAME(FXFreeRun)
    static constexpr uint16_t FREE_RUN_DIM_IN_MILLIS = 4000;
    static constexpr uint8_t FREE_RUN_BRIGHTNESS = 20;

    explicit FXFreeRun(const byte TRIGGER_NOTE) : FXBase(TRIGGER_NOTE) {
        UpscalePalette(p16white, p256);
        brightness = 0;
        palettePos = 0;
        colorOffset = 0;
    }

    void  makeEffect(LEDConfig &ledConfig, const byte velocity) override {
        colorOffset = beat8(colorSpeed);
        brightness = FREE_RUN_BRIGHTNESS * getRampValue(millis() - startMillis, FREE_RUN_DIM_IN_MILLIS);

        for (int i = 0; i < ledConfig.LINE_NUM; i++) {
            palettePos = (i * 190 / ledConfig.LINE_NUM) - colorOffset;
            color = ColorFromPalette(p256, palettePos);
            ledConfig.lineOn(ledConfig.lines[i], &color, static_cast<byte>(brightness));
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
    float brightness;
    CRGB color;
    uint8_t palettePos;
    uint8_t colorOffset;
};

#endif //FX_FREE_RUN_2_H
