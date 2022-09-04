#if UNIT_TEST && UNIT_TEST_NATIVE

#pragma once

#include <inttypes.h>
#include <chrono>
#include <unistd.h>
#include <thread>
#include <Esp.h>

#include "WString.h"
#include "stdlib_noniso.h"

#define PROGMEM

uint32_t millis();

void delay(unsigned long);

#endif