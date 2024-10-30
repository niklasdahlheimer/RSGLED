#ifndef FXLEVEL_PUMP_H
#define FXLEVEL_PUMP_H

#include <fxBase.h>
#include "helper.h"

#define PUMP_PERIOD_IN_MILLIS       100

class FXLevelPump final : public FXBase {
public:
    explicit FXLevelPump(const byte TRIGGER_NOTE) : FXBase(TRIGGER_NOTE) {}

    void makeEffect(LEDConfig &ledConfig, const byte velocity) override {
        const unsigned int currentStep =
              helper_getSteppedSawValue(ledConfig.timestamp - startMillis, PUMP_PERIOD_IN_MILLIS, 6);
              ledConfig.levelOn(currentStep, ledConfig.globalColor, velocity);
    };

    void onReset() override {
		
    };

    void onStart(LEDConfig &ledConfig) override {
       
    }

    void onFinish(LEDConfig &ledConfig) override {
       
    }

private:

};

#endif //FXLEVEL_PUMP_H
