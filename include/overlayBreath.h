#ifndef FX__BREATH_H
#define FX__BREATH_H

#include <fxBase.h>


/**
 * @brief Overlay: Erzeugt einen pulsierenden "Atmungs"-Effekt.
 * 
 * Die Helligkeit aller LEDs wird sinusförmig über einen Zeitraum (abhängig vom Tempo)
 * variiert, was ein langsames Aufleuchten und Abblenden bewirkt.
 */
class OverlayBreath final : public FXBase {
public:
    DEFINE_GETNAME(OverlayBreath)

    explicit OverlayBreath(const byte TRIGGER_NOTE) : FXBase(TRIGGER_NOTE) {
    }

protected:
    void makeEffect(LEDConfig &ledConfig, const byte velocity) override {
        const double timeFactor = pow((1 +
                                       sin(2 * M_PI * static_cast<double>(ledConfig.timestamp - startMillis) /
                                           getBeatLenInMillis(ledConfig.tempo, 1) -
                                           M_PI / 2)) / 2, 2); // between 0 and 1
        const double currBrightness = (velocity / 255.0) * timeFactor * LED_BRIGHTNESS_MAX;
        ledConfig.allBrighten(static_cast<byte>(currBrightness));
    }

    void onReset() override {
    }

    void onStart(LEDConfig &ledConfig) override {
    }

    void onFinish(LEDConfig &ledConfig) override {
    }
};

#endif //FX__BREATH_H
