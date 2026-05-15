#ifndef FX_ROTATE_GROUP_H
#define FX_ROTATE_GROUP_H

#include <fxBase.h>

/**
 * @brief Overlay: Rotiert den Effekt über die verschiedenen LED-Gruppen.
 * 
 * Basierend auf dem Tempo wird nacheinander jeweils eine der LED-Gruppen (1 bis 10)
 * exklusiv aktiviert (Solo-Modus).
 */
class OverlayRotateGroup final : public FXBase {
public:
    DEFINE_GETNAME(OverlayRotateGroup)

    explicit OverlayRotateGroup(const byte TRIGGER_NOTE) : FXBase(TRIGGER_NOTE) {
    }

protected:
    void makeEffect(LEDConfig &ledConfig, const byte velocity) override {
        const unsigned int currentStep = getSteppedSawValue(ledConfig.timestamp - startMillis,
                                                            getBeatLenInMillis(ledConfig.tempo, 16),
                                                            10); // returns 0-9
        ledConfig.groupSolo(ledConfig.groups[currentStep + 1]); // group 1 to group 10 (group 0 is all on)
    }

    void onReset() override {
    }

    void onStart(LEDConfig &ledConfig) override {
    }

    void onFinish(LEDConfig &ledConfig) override {
    }

private:
};

#endif //FX_ROTATE_GROUP_H
