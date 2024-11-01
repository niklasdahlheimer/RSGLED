#ifndef FX_TEST_H
#define FX_TEST_H

#include <fxBase.h>

#define RAINBOW_PERIOD_IN_MILLIS    10

class FXTest final : public FXBase {
public:
    DEFINE_GETNAME(FXTest)

    explicit FXTest(const byte TRIGGER_NOTE) : FXBase(TRIGGER_NOTE) {
    }

    void makeEffect(LEDConfig &ledConfig, const byte velocity) override {
        const byte newPart = getSteppedSawValue(ledConfig.timestamp - startMillis, 5000, 3);

        if (newPart != currentPart) {
            partStartTime = ledConfig.timestamp;
            currentPart = newPart;
        }

        if (currentPart == 0) {
            const byte lineIndex = getSteppedSawValue(ledConfig.timestamp - partStartTime, 5000 / ledConfig.LINE_NUM,
                                                      ledConfig.LINE_NUM);
            ledConfig.lineOn(ledConfig.lines[lineIndex], &COLORS[0], 255);
        } else if (currentPart == 1) {
            const byte groupIndex = getSteppedSawValue(ledConfig.timestamp - partStartTime, 5000 / ledConfig.GROUP_NUM,
                                                       ledConfig.GROUP_NUM);
            ledConfig.groupOn(ledConfig.groups[groupIndex], &COLORS[0], 255);
        } else {
            fill_rainbow_circular(ledConfig.LEDs,
                                  ledConfig.LED_NUM,
                                  ((ledConfig.timestamp - partStartTime) / RAINBOW_PERIOD_IN_MILLIS),
                                  (uint8_t) (10 * (velocity / 127.0)));
        }
    };

    void onReset() override {
    };

    void onStart(LEDConfig &ledConfig) override {
    }

    void onFinish(LEDConfig &ledConfig) override {
        currentPart = 0;
        partStartTime = 0;
    }

private:
    byte currentPart = 0;
    unsigned long partStartTime = 0;
};

#endif //FX_TEST_H
