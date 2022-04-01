#ifndef LOGGING_H
#define LOGGING_H

#ifdef __AVR__
    #define LOG_DEBUG(x)
#else
    #include <iostream>
    #define LOG_DEBUG(x) (std::cout << x)
#endif

#endif // LOGGING_H