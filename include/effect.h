#ifndef EFFECT_H
#define EFFECT_H

#include <Arduino.h>
#include "ledController.h"

class Effect {
public:
    virtual ~Effect() = default;

    explicit Effect(const byte TRIGGER_NOTE) : triggerNote(TRIGGER_NOTE), startMillis(0), effectRunCount(0) {}

    virtual void handle(LEDConfig &ledConfig) final {
        if (ledConfig.note[triggerNote] && startMillis == 0) {
            startMillis = ledConfig.timestamp;
            effectRunCount++;
            onStart(ledConfig);
        } else if (!ledConfig.note[triggerNote] && startMillis != 0) {
            startMillis = 0;
            onFinish(ledConfig);
        }
        if(ledConfig.note[triggerNote]) {
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
