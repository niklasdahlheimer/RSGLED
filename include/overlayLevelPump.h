#ifndef FX_LEVEL_PUMP_H
#define FX_LEVEL_PUMP_H

#include <fxBase.h>
#include "helper.h"

#define PUMP_PERIOD_IN_MILLIS       100

// Overlay
class OverlayLevelPump final : public FXBase {
public:
    DEFINE_GETNAME(OverlayLevelPump)

    explicit OverlayLevelPump(const byte TRIGGER_NOTE) : FXBase(TRIGGER_NOTE) {}

    void makeEffect(LEDConfig &ledConfig, const byte velocity) override {
        const unsigned int currentStep =
              helper_getSteppedSawValue(ledConfig.timestamp - startMillis, PUMP_PERIOD_IN_MILLIS, 6);
              ledConfig.levelSolo(currentStep);
    };

    void onReset() override {
		
    };

    void onStart(LEDConfig &ledConfig) override {
       
    }

    void onFinish(LEDConfig &ledConfig) override {
       
    }

private:

};

#endif //FX_LEVEL_PUMP_H
