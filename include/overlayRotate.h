#ifndef FXROTATE_H
#define FXROTATE_H

#include <fxBase.h>
#include "helper.h"

// Overlay
class OverlayRotate final : public FXBase {
public:
    explicit OverlayRotate(const byte TRIGGER_NOTE) : FXBase(TRIGGER_NOTE) {}

    void makeEffect(LEDConfig &ledConfig, const byte velocity) override {
        const unsigned int currentStep = helper_getSteppedSawValue(ledConfig.timestamp - startMillis,
                                                       helper_getBeatLenInMillis(ledConfig.tempo, 16),
                                                       10); // returns 0-9
        ledConfig.groupSolo(ledConfig.groups[currentStep+1]); // group 1 to group 10 (group 0 is all on)
    };

    void onReset() override {
		
    };

    void onStart(LEDConfig &ledConfig) override {
       
    }

    void onFinish(LEDConfig &ledConfig) override {
       
    }

private:

};

#endif //FXROTATE_H
