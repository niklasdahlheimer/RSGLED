#ifndef FXSTROBE_H
#define FXSTROBE_H

#include <fxBase.h>
#include "helper.h"

#define STROBE_ON_FACTOR            0.2 // Factor between 0 and 1 how long the LEDs stay ON

// Overlay
class OverlayStrobe final : public FXBase {
public:
    explicit OverlayStrobe(const byte TRIGGER_NOTE) : FXBase(TRIGGER_NOTE) {}

    void makeEffect(LEDConfig &ledConfig, const byte velocity) override {
        int state = helper_getRectValue(ledConfig.timestamp - startMillis, helper_getBeatLenInMillis(ledConfig.tempo, 16),
                                    STROBE_ON_FACTOR);
        if (state == 1) {
            //ledConfig.allOn(ledConfig.globalColor, velocity); // Turn all LEDs on to the strobe color
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
