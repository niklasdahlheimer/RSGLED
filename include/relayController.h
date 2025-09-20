#ifndef RSGLED_RELAYCONTROLLER_H
#define RSGLED_RELAYCONTROLLER_H

#include <Arduino.h>
#include "midiConsts.h"
#include "config.h"

// 23 is reserved for LED!
// https://devices.esphome.io/devices/ESP32-Relay-X2
#define RELAY1_PIN             GPIO_NUM_16 // internal Relay outer
#define RELAY2_PIN             GPIO_NUM_17 // internal Relay inner
#define RELAY3_PIN             GPIO_NUM_15 // external Relay

#define TOTAL_RESET            Note_C_2

#define RELAY_1_ON             Note_F_2
#define RELAY_2_ON             Note_G_2
#define RELAY_3_ON             Note_B_2

void RELAYC_init();

void RELAYC_update(const byte *note, const byte *controller, const int debugControl);

#endif //RSGLED_RELAYCONTROLLER_H
