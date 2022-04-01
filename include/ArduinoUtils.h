#ifndef ARDUINOUTILS_H
#define ARDUINOUTILS_H
#ifdef __AVR__
    #include "Arduino.h"
    #define ASSERT(x)
#else
    #include "ArduinoLinux.h"
    #include <cassert>
    #define ASSERT(x) (assert(x))
#endif
#endif // ARDUINOUTILS_H
