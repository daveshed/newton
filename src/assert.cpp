#include "ArduinoUtils.h"

// report diagnostic information and abort program execution
void __assert(const char *__func, const char *__file, int __lineno, const char *__sexp)
{
    Serial.print("ASSERTION (");
    Serial.print(__sexp);
    Serial.print(") FAILED: ");
    Serial.print(__file);
    Serial.print(":");
    Serial.print(__lineno);
    Serial.println();
    Serial.flush();
    abort();
}
