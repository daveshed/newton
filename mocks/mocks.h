/*
 * Mock api only available to tests
 */
#ifndef MOCKS_H
#define MOCKS_H

#include "ArduinoLinux.h"

// allow tests to modify timestamp
void millis(uint32_t value);

#endif // MOCKS_H
