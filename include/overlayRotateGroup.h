#ifndef FX_ROTATE_GROUP_H
#define FX_ROTATE_GROUP_H

#include <fxBase.h>

// Overlay
class OverlayRotateGroup final : public FXBase {
public:
    DEFINE_GETNAME(OverlayRotateGroup)

    explicit OverlayRotateGroup(const byte TRIGGER_NOTE) : FXBase(TRIGGER_NOTE) {}

    void makeEffect(LEDConfig &ledConfig, const byte velocity) override {
        const unsigned int currentStep = getSteppedSawValue(ledConfig.timestamp - startMillis,
                                                       getBeatLenInMillis(ledConfig.tempo, 16),
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

#endif //FX_ROTATE_GROUP_H
