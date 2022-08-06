#pragma once
#ifdef __arm__
    #include "Arduino.h"
    // had to reimplement code from avr/include/assert.h - handler wasn't being called.
    #define ASSERT(e) ((e) ? (void)0 : __assert(__func__, __FILE__, __LINE__, #e))

    void __assert(const char* __func, const char* __file, int __lineno, const char* __sexp);

#else
    #include "ArduinoLinux.h"
    #include <cassert>
    #include <climits>
    #define ASSERT(e) (assert(e))
#endif

// https://isocpp.org/wiki/faq/pointers-to-members£macro-for-ptr-to-memfn
#define CALL_MEMBER_FN(ptr_to_object, ptr_to_member)    ((ptr_to_object)->*(ptr_to_member))
