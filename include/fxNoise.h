#ifndef FXNOISE_H
#define FXNOISE_H

#include <fxBase.h>

#define NOISE_PERIOD_IN_MILLIS      10

class FXNoise final : public FXBase {
public:
    explicit FXNoise(const byte TRIGGER_NOTE) : FXBase(TRIGGER_NOTE) {}

    void makeEffect(LEDConfig &ledConfig, const byte velocity) override {
        if (ledConfig.timestamp - noiseLastUpdateMillis > NOISE_PERIOD_IN_MILLIS) {
            noiseLastUpdateMillis = ledConfig.timestamp;
            noiseCurrentVal += ((double) random(0, 10) - 5) * 0.01;
            noiseCurrentVal = noiseCurrentVal > 0.9 ? 0.9 : (noiseCurrentVal < 0.1 ? 0.1 : noiseCurrentVal);
        }
        ledConfig.allOn(ledConfig.globalColor,
                   LED_BRIGHTNESS_MAX * noiseCurrentVal * (velocity / 127.0));
    };

    void onReset() override {
        noiseCurrentVal = 0.5;
        noiseLastUpdateMillis = 0;
    };

    void onStart(LEDConfig &ledConfig) override {
       
    }

    void onFinish(LEDConfig &ledConfig) override {
        noiseCurrentVal = 0.5;
        noiseLastUpdateMillis = 0;
    }

private:
    double noiseCurrentVal = 0.5;
    unsigned long noiseLastUpdateMillis = 0;
};

#endif //FXNOISE_H
