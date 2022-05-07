#ifndef ARDUINOUTILS_H
#define ARDUINOUTILS_H
#ifdef __AVR__
    #include "Arduino.h"
    #define ASSERT(x)
#else
    #include "ArduinoLinux.h"
    #include <cassert>
    #include <climits>
    #define ASSERT(x) (assert(x))
#endif

// https://isocpp.org/wiki/faq/pointers-to-members£macro-for-ptr-to-memfn
#define CALL_MEMBER_FN(ptr_to_object, ptr_to_member)    ((ptr_to_object)->*(ptr_to_member))
#endif // ARDUINOUTILS_H
