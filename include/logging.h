#ifndef LOGGING_H
#define LOGGING_H

#ifdef __AVR__
    #include "ArduinoUtils.h"
    #define LOG_DEBUG(x) (Serial.println(x))
#else
    #include <iostream>
    #define LOG_DEBUG(x) (std::cout << x << std::endl)
#endif

#endif // LOGGING_H