#ifndef HELPER_H
#define HELPER_H

#define SERIAL_PRINTF_MAX_BUFF      256
void serialPrintf(const char *fmt, ...);

void displayFreeRam();

void maybeDisplayCriticalRam();

#endif //HELPER_H


