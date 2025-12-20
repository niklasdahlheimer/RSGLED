#ifndef FX_FREE_RUN_H
#define FX_FREE_RUN_H

#include <fxBase.h>

CRGB white = CRGB::LightSkyBlue;

class FXFreeRun final : public FXBase {
public:
    DEFINE_GETNAME(FXFreeRun)
    static constexpr uint16_t FREE_RUN_DIM_IN_MILLIS = 2000;
    static constexpr uint8_t FREE_RUN_BRIGHTNESS = 30;

    explicit FXFreeRun(const byte TRIGGER_NOTE) : FXBase(TRIGGER_NOTE) {
    }

    void makeEffect(LEDConfig &ledConfig, const byte velocity) override {
        const float brightness = FREE_RUN_BRIGHTNESS * getRampValue(millis() - startMillis, FREE_RUN_DIM_IN_MILLIS);
        ledConfig.allOn(&white, static_cast<byte>(brightness));
        //Serial.printf("set velocity to %d at %d \n", brightness, millis() - startMillis);
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
