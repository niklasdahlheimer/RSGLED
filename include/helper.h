#ifndef HELPER_H
#define HELPER_H

#include <Arduino.h>

static int helper_getRectValue(const unsigned long currentTime, const unsigned int period, const float onFactor) {
    const unsigned long phase = currentTime % period;
    const unsigned long onPeriod = period * onFactor;
    return phase < onPeriod ? 1 : 0;
}

static unsigned long helper_getSteppedValue(const unsigned long currentTime, const unsigned int period) {
    return (currentTime / period);
}

/**
 * Returns periodic stepped value from 0 to numOfSteps-1
 */
static unsigned long helper_getSteppedSawValue(const unsigned long currentTime, const unsigned int period,
                                        const unsigned int numOfSteps) {
    return helper_getSteppedValue(currentTime, period) % numOfSteps;
}

static unsigned int helper_getBeatLenInMillis(unsigned int tempo, unsigned int div, boolean isTrip = false, boolean isDot = false) {
    double beatLengthInMillis = 60000.0 / tempo;
    beatLengthInMillis *= 4.0 / div; // a quarter is equivalent to 1 "beat" for simplicity

    if (isTrip) {
        beatLengthInMillis *= 2.0 / 3.0;
    }
    if (isDot) {
        beatLengthInMillis *= 1.5;
    }

    return static_cast<unsigned int>(beatLengthInMillis);
}

#endif //HELPER_H
