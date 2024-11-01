#ifndef FX_SPARKLE_H
#define FX_SPARKLE_H

#include <fxBase.h>

#define NOISE_PERIOD_IN_MILLIS      10

class FXSparkle final : public FXBase {
public:
    DEFINE_GETNAME(FXSparkle)

    explicit FXSparkle(const byte TRIGGER_NOTE) : FXBase(TRIGGER_NOTE) {}

    void makeEffect(LEDConfig &ledConfig, const byte velocity) override {
        // Define the density and brightness of the sparkles based on the velocity
        byte density = map(velocity, 0, 255, 1, 20); // Adjust sparkle density with velocity
        byte sparkleBrightness = velocity; // Adjust brightness with velocity

        // Iterate through the LEDs and randomly assign sparkles
        for (int i = 0; i < density; i++) {
            int ledIndex = random(ledConfig.LED_NUM); // Randomly select an LED index

            // Choose whether to make the sparkle colorful or white
            bool makeWhite = random(100) < 50; // 30% chance of white sparkles, adjust as needed
            if (makeWhite) {
                ledConfig.LEDs[ledIndex] = CRGB::White;
            } else {
                // Randomly choose a vibrant color using HSV
                ledConfig.LEDs[ledIndex] = CHSV(random(0, 255), 200, sparkleBrightness);
                // High saturation for vibrant colors
            }
        }

        // Fade out all LEDs gradually to create a subtle sparkle effect
        for (int i = 0; i < ledConfig.LED_NUM; i++) {
            ledConfig.LEDs[i].fadeToBlackBy(100); // Adjust fade speed as needed
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

#endif //FX_SPARKLE_H
