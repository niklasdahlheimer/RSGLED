#ifndef RSGLED_RELAYCONTROLLER_H
#define RSGLED_RELAYCONTROLLER_H

#include <Arduino.h>
#include "midiConsts.h"
#include "config.h"

#define RELAY1                 16
#define RELAY2                 17

#define TOTAL_RESET            Note_C_2

#define RELAY_1_ON             Note_F_2
#define RELAY_2_ON             Note_G_2

void RELAYC_init(const Config *config);

void RELAYC_update(const byte *noteData, const byte *controllerData);

#endif //RSGLED_RELAYCONTROLLER_H
