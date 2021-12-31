#ifndef _MATERIALIZE_LAYOUT_DEBUGGING_H_
#define _MATERIALIZE_LAYOUT_DEBUGGING_H_

#if defined(ARDUINO_ARCH_ESP8266)
#include <DebugPrintMacros.h>
#define ESP_LOGE(_, ...) DEBUG_ESP_PRINTF(__VA_ARGS__)
#elif defined(ARDUINO_ARCH_ESP32)
#include <esp_log.h>
#else
#error "This platform isn't supported"
#endif

#endif // _MATERIALIZE_LAYOUT_DEBUGGING_H_