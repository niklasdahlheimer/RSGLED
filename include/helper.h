#ifndef HELPER_H
#define HELPER_H
#include <SoftwareSerial.h>

#define SERIAL_PRINTF_MAX_BUFF      256

void serialPrintf(SoftwareSerial *serial, const char *fmt, ...);

void displayFreeRam(SoftwareSerial *serial);

void maybeDisplayCriticalRam(SoftwareSerial *serial);

#endif //HELPER_H


