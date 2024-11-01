#ifndef FX_DAB_H
#define FX_DAB_H
#include <fxBase.h>

#define ANIMATED_LINES_NUM 10

class OverlayDab final : public FXBase {
public:
    DEFINE_GETNAME(OverlayDab)

    explicit OverlayDab(const byte TRIGGER_NOTE) : FXBase(TRIGGER_NOTE) {
        color = CRGB::White;
    }

    void makeEffect(LEDConfig &ledConfig, const byte velocity) override {
        const unsigned long animationLength = helper_getBeatLenInMillis(ledConfig.tempo, 16);
        for (int i = 0; i < ANIMATED_LINES_NUM; ++i) {
            if (ledConfig.timestamp - lineStartTime[i] > animationLength) {
                lineIndex[i] = random8(ledConfig.LINE_NUM);
                lineStartTime[i] = ledConfig.timestamp;
            }
            ledConfig.lineOn(ledConfig.lines[lineIndex[i]], &color);
        }
    };

    void onReset() override {
    }

    void onStart(LEDConfig &ledConfig) override {
    }

    void onFinish(LEDConfig &ledConfig) override {
    }

private:
    CRGB color;
    unsigned long lineStartTime[ANIMATED_LINES_NUM] = {};
    unsigned long lineIndex[ANIMATED_LINES_NUM] = {};
};


#endif //FX_DAB_H
