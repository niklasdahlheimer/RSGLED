#ifndef FX_GRADIENT_WALK_H
#define FX_GRADIENT_WALK_H

#include <fxBase.h>

class FXGradientWalk final : public FXBase {
public:
    DEFINE_GETNAME(FXGradientWalk)

    explicit FXGradientWalk(const byte TRIGGER_NOTE) : FXBase(TRIGGER_NOTE) {
    }

protected:
    void makeEffect(LEDConfig &ledConfig, const byte velocity) override {
        const double period = static_cast<double>(getBeatLenInMillis(ledConfig.tempo, 64)) * ledConfig.tempoTrim;

        const unsigned long elapsedTime = ledConfig.timestamp - lastUpdateTime;

        const double progress = static_cast<double>(elapsedTime) / period;
        lastStep = (lastStep + static_cast<unsigned int>(progress)) % ledConfig.LINE_NUM;

        if (progress >= 1.0) {
            lastUpdateTime = ledConfig.timestamp;
        }

        // circling offset
        for (int i = 0; i < ledConfig.LINE_NUM; i++) {
            ledConfig.lineOn(ledConfig.lines[(i + lastStep) % ledConfig.LINE_NUM], &ledConfig.lineGradientLEDs[i],
                             velocity);
        }
    }

    void onReset() override {
        lastStep = 0;
        lastUpdateTime = 0;
    }

    void onStart(LEDConfig &ledConfig) override {
        fl::fill_gradient_RGB(ledConfig.lineGradientLEDs, ledConfig.LINE_NUM, COLORS[2], COLORS[11]);
        lastStep = 0;
        lastUpdateTime = ledConfig.timestamp;
    }

    void onFinish(LEDConfig &ledConfig) override {
        lastStep = 0;
        lastUpdateTime = 0;
    }

private:
    unsigned int lastStep = 0;
    unsigned long lastUpdateTime = 0;
};

#endif //FX_GRADIENT_WALK_H
