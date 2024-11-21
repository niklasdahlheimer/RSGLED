#ifndef FX_FREE_RUN_H
#define FX_FREE_RUN_H

#include <fxBase.h>

#define FREE_RUN_PERIOD_IN_MILLIS 10000

CRGB white = CRGB::DarkOrange;

class FXFreeRun final : public FXBase {
public:
    DEFINE_GETNAME(FXFreeRun)

    explicit FXFreeRun(const byte TRIGGER_NOTE) : FXBase(TRIGGER_NOTE) {
    }

    void makeEffect(LEDConfig &ledConfig, const byte velocity) override {
        ledConfig.allOn(&white, 30);
    };

    void onReset() override {
    };

    void onStart(LEDConfig &ledConfig) override {
    }

    void onFinish(LEDConfig &ledConfig) override {
    }

private:
};

#endif //FX_FREE_RUN_H
