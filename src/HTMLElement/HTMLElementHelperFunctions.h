#pragma once

#include "HTMLElemetTypes.h"
#include <Arduino.h>

String getWidthClassPrefix(ScreenSize screenSize);

String colorShadeToString(ColorShade cs);

bool colorShadeIsValid(Color c, ColorShade cs);

String colorToString(Color c);
