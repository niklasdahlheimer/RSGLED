#ifndef FX_RAINBOW_H
#define FX_RAINBOW_H

#include <fxBase.h>

class FXRainbow final : public FXBase {
public:
    DEFINE_GETNAME(FXRainbow)

    explicit FXRainbow(const byte TRIGGER_NOTE) : FXBase(TRIGGER_NOTE) {
    }

protected:
    void makeEffect(LEDConfig &ledConfig, const byte velocity) override {
        // Berechnet den Start-Hue basierend auf der Zeit und dem Tempo.
        // Ein voller Regenbogen-Durchlauf (256 Stufen) pro Takt.
        const uint8_t initialHue = (256 * (ledConfig.timestamp - startMillis)) / getBeatLenInMillis(ledConfig.tempo, 1);
        
        fl::fill_rainbow_circular(ledConfig.LEDs,
                                  ledConfig.LED_NUM,
                                  initialHue,
                                  static_cast<uint8_t>(10 * (velocity / 127.0)));
    }

    void onReset() override {
    }

    void onStart(LEDConfig &ledConfig) override {
    }

    void onFinish(LEDConfig &ledConfig) override {
    }

private:
};

#endif //FX_RAINBOW_H
