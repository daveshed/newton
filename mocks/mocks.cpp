/*
 * Implementation of Arduino.h utilities
 *
 * Code under test has access to these functions and tests may also modify
 * time for example.
 *
 * Only built on Linux for tests.
*/
#include "ArduinoUtils.h"
#include "mocks.h"

static uint32_t g_millis_value = 0L;

uint32_t millis(void)
{
    return g_millis_value;
}

void millis(uint32_t value)
{
    g_millis_value = value;
}
