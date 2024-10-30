#ifndef FXSTROBE_H
#define FXSTROBE_H

#include <fxBase.h>
#include "helper.h"

class FXStrobe final : public FXBase {
public:
    explicit FXStrobe(const byte TRIGGER_NOTE) : FXBase(TRIGGER_NOTE) {}

    void makeEffect(LEDConfig &ledConfig, const byte velocity) override {
        int state = helper_getRectValue(ledConfig.timestamp - startMillis, helper_getBeatLenInMillis(ledConfig.tempo, 16),
                                    STROBE_ON_FACTOR);
        if (state == 1) {
            ledConfig.LED_all_on(ledConfig.globalColor, velocity); // Turn all LEDs on to the strobe color
        } else {
            FastLED.clear(); // Turn all LEDs off
        }
    };

    void onReset() override {
		
    };

    void onStart(LEDConfig &ledConfig) override {
       
    }

    void onFinish(LEDConfig &ledConfig) override {
       
    }

private:

};

#endif //FXSTROBE_H
