#ifndef FX_ROTATE_LINE_H
#define FX_ROTATE_LINE_H

#include <fxBase.h>

// Overlay
class OverlayRotateLine final : public FXBase {
public:
    DEFINE_GETNAME(OverlayRotateLine)

    explicit OverlayRotateLine(const byte TRIGGER_NOTE) : FXBase(TRIGGER_NOTE) {
    }

    void makeEffect(LEDConfig &ledConfig, const byte velocity) override {
        const unsigned int currentStep = getSteppedSawValue(ledConfig.timestamp - startMillis,
                                                            getBeatLenInMillis(ledConfig.tempo, 32),
                                                            ledConfig.LINE_NUM); // returns 0 to LINE_NUM-1
        ledConfig.lineSolo(ledConfig.lines[currentStep]); // line 0 to ledConfig.LINE_NUM - 1
    };

    void onReset() override {
    };

    void onStart(LEDConfig &ledConfig) override {
    }

    void onFinish(LEDConfig &ledConfig) override {
    }

private:
};

#endif //FX_ROTATE_LINE_H
