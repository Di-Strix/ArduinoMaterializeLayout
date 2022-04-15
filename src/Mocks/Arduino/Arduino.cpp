#if UNIT_TEST && UNIT_TEST_NATIVE

#include "Arduino.h"

uint32_t millis()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void delay(unsigned long t) {
    std::this_thread::sleep_for(std::chrono::milliseconds(t));
}

int main()
{
    setup();
}

#endif