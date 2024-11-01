#ifndef EFFECT_H
#define EFFECT_H

#include <Arduino.h>
#include "ledController.h"

#define DEFINE_GETNAME(className) const char* getName() const override { return #className; }

class FXBase {
public:
    virtual ~FXBase() = default;

    explicit FXBase(const byte TRIGGER_NOTE) : triggerNote(TRIGGER_NOTE), startMillis(0), effectRunCount(0) {
    }

    virtual const char *getName() const = 0;

    virtual void handle(LEDConfig &ledConfig) final {
        if (ledConfig.note[triggerNote] && startMillis == 0) {
            startMillis = ledConfig.timestamp;
            effectRunCount++;
            Serial.printf("Starting effect (%d) ", effectRunCount);
            Serial.println(getName());
            onStart(ledConfig);
        } else if (!ledConfig.note[triggerNote] && startMillis != 0) {
            startMillis = 0;
            onFinish(ledConfig);
        }
        if (ledConfig.note[triggerNote]) {
            makeEffect(ledConfig, ledConfig.note[triggerNote]);
        }
    };

    virtual void reset() final {
        startMillis = 0;
        effectRunCount = 0;
        onReset();
    };

    byte getTriggerNote() const {
        return triggerNote;
    }

    /// helper

    /// Returns 0 or 1, starts at 1
    static int getRectValue(const unsigned long currentTime, const unsigned int period, const float onFactor) {
        const unsigned long phase = currentTime % period;
        const unsigned long onPeriod = period * onFactor;
        return phase < onPeriod ? 1 : 0;
    }

    /// Returns stepped values from 0 to infinity
    static unsigned long getSteppedValue(const unsigned long currentTime, const unsigned int period) {
        return (currentTime / period);
    }

    /// Returns periodic stepped value from 0 to numOfSteps-1
    static unsigned long getSteppedSawValue(const unsigned long currentTime, const unsigned int period,
                                            const unsigned int numOfSteps) {
        return getSteppedValue(currentTime, period) % numOfSteps;
    }

    /// returns millis for beat (div 4 = Quarter, 1 = whole,...)
    static unsigned int getBeatLenInMillis(unsigned int tempo, unsigned int div, boolean isTrip = false,
                                           boolean isDot = false) {
        double beatLengthInMillis = 60000.0 / tempo;
        beatLengthInMillis *= 4.0 / div;

        if (isTrip) {
            beatLengthInMillis *= 2.0 / 3.0;
        }
        if (isDot) {
            beatLengthInMillis *= 1.5;
        }

        return static_cast<unsigned int>(beatLengthInMillis);
    }

protected:
    byte triggerNote;
    unsigned long startMillis;
    unsigned int effectRunCount;

    virtual void onStart(LEDConfig &ledConfig) = 0;

    virtual void onFinish(LEDConfig &ledConfig) = 0;

    virtual void makeEffect(LEDConfig &ledConfig, byte velocity) = 0;

    virtual void onReset() = 0;
};

#endif //EFFECT_H
