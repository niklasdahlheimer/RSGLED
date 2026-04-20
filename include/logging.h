#ifndef RSGLED_LOGGING_H
#define RSGLED_LOGGING_H

#include <Arduino.h>

#if DEBUG_LOG_ENABLE
  #define LOGD(...) Serial.printf(__VA_ARGS__)
  #define LOGN(x) Serial.println(x)
#else
  #define LOGD(...)
  #define LOGN(x)
#endif

#endif // RSGLED_LOGGING_H
