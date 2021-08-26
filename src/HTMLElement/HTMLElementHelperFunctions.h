#ifndef _MATERIALIZE_LAYOUT_HTML_ELEMENT_HELPER_FUNCTIONS_H_
#define _MATERIALIZE_LAYOUT_HTML_ELEMENT_HELPER_FUNCTIONS_H_

#include "HTMLElemetTypes.h"
#include <Arduino.h>

String getWidthClassPrefix(ScreenSize screenSize);

String colorShadeToString(ColorShade cs);

bool colorShadeIsValid(Color c, ColorShade cs);

String colorToString(Color c);

#endif //_MATERIALIZE_LAYOUT_HTML_ELEMENT_HELPER_FUNCTIONS_H_