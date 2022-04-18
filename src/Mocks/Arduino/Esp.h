#if UNIT_TEST && UNIT_TEST_NATIVE

#pragma once
#include <Arduino.h>

class EspClass
{
    public:
    uint32_t getMaxFreeBlockSize()
    {
        return 520000;
    }
};

extern EspClass ESP;

#endif