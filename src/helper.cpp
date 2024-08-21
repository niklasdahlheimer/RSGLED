#include "helper.h"
#include <stdarg.h>
#include <Arduino.h>

void serialPrintf(const char *fmt, ...) {
    /* Buffer for storing the formatted data */
    char buff[SERIAL_PRINTF_MAX_BUFF];
    /* pointer to the variable arguments list */
    va_list pargs;
    /* Initialise pargs to point to the first optional argument */
    va_start(pargs, fmt);
    /* create the formatted data and store in buff */
    vsnprintf(buff, SERIAL_PRINTF_MAX_BUFF, fmt, pargs);
    va_end(pargs);
    Serial.println(buff);
}


int getFreeRam() {
    extern int __heap_start,*__brkval;
    int v;
    return (int)&v - (__brkval == 0
      ? (int)&__heap_start : (int) __brkval);
}

void displayFreeRam(){
    Serial.print(F("- SRAM left: "));
    Serial.println(getFreeRam());
}

void maybeDisplayCriticalRam(){
     int freeRam = getFreeRam();
     if(freeRam < 100){
         Serial.print(F("- CRITICAL SRAM left: "));
         Serial.println(freeRam);
     }
}


