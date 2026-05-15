#ifndef FX_LEVEL_PUMP_H
#define FX_LEVEL_PUMP_H

#include <fxBase.h>

/**
 * @brief Overlay: Erzeugt einen pulsierenden Effekt auf den Level-Gruppen.
 * 
 * Basierend auf dem Tempo wird ein Sägezahn-Wert berechnet, der nacheinander die verschiedenen
 * Helligkeitsstufen (Level) anspricht.
 */
class OverlayLevelPump final : public FXBase {
public:
    DEFINE_GETNAME(OverlayLevelPump)

    explicit OverlayLevelPump(const byte TRIGGER_NOTE) : FXBase(TRIGGER_NOTE) {
    }

protected:
    void makeEffect(LEDConfig &ledConfig, const byte velocity) override {
        const unsigned int currentStep =
                getSteppedSawValue(ledConfig.timestamp - startMillis, getBeatLenInMillis(ledConfig.tempo, 8, true), 6);
        ledConfig.levelSolo(currentStep);
    }

    void onReset() override {
    }

    void onStart(LEDConfig &ledConfig) override {
    }

    void onFinish(LEDConfig &ledConfig) override {
    }

private:
};

#endif //FX_LEVEL_PUMP_H
