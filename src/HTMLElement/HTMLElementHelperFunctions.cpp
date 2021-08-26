#include "HTMLElementHelperFunctions.h"

String getWidthClassPrefix(ScreenSize screenSize)
{
  switch (screenSize) {
  default:
  case ScreenSize::small:
    return "s";
    break;
  case ScreenSize::medium:
    return "m";
    break;
  case ScreenSize::large:
    return "l";
    break;
  case ScreenSize::extraLarge:
    return "xl";
    break;
  }
}

String colorShadeToString(ColorShade cs)
{
  switch (cs) {
  case ColorShade::lighten5:
    return F("lighten-5");
    break;
  case ColorShade::lighten4:
    return F("lighten-4");
    break;
  case ColorShade::lighten3:
    return F("lighten-3");
    break;
  case ColorShade::lighten2:
    return F("lighten-2");
    break;
  case ColorShade::lighten1:
    return F("lighten-1");
    break;
  default:
  case ColorShade::noShade:
    return "";
    break;
  case ColorShade::darken1:
    return F("darken-1");
    break;
  case ColorShade::darken2:
    return F("darken-2");
    break;
  case ColorShade::darken3:
    return F("darken-3");
    break;
  case ColorShade::darken4:
    return F("darken-4");
    break;
  case ColorShade::accent1:
    return F("accent-1");
    break;
  case ColorShade::accent2:
    return F("accent-2");
    break;
  case ColorShade::accent3:
    return F("accent-3");
    break;
  case ColorShade::accent4:
    return F("accent-4");
    break;
  }
}

bool colorShadeIsValid(Color c, ColorShade cs)
{
  switch (c) {
  case Color::brown:
  case Color::grey:
  case Color::blueGrey:
    if (!colorShadeToString(cs).startsWith(F("accent")))
      return true;
    break;

  case Color::black:
  case Color::white:
  case Color::transparent:
    break;

  default:
    return true;
    break;
  }

  return false;
}

String colorToString(Color c)
{
  switch (c) {
  default:
  case Color::defaultColor:
    return "";
    break;
  case Color::red:
    return F("red");
    break;
  case Color::pink:
    return F("pink");
    break;
  case Color::purple:
    return F("purple");
    break;
  case Color::deepPurple:
    return F("deep-purple");
    break;
  case Color::indigo:
    return F("indigo");
    break;
  case Color::blue:
    return F("blue");
    break;
  case Color::lightBlue:
    return F("light-blue");
    break;
  case Color::cyan:
    return F("cyan");
    break;
  case Color::teal:
    return F("teal");
    break;
  case Color::green:
    return F("green");
    break;
  case Color::lightGreen:
    return F("light-green");
    break;
  case Color::lime:
    return F("lime");
    break;
  case Color::yellow:
    return F("yellow");
    break;
  case Color::amber:
    return F("amber");
    break;
  case Color::orange:
    return F("orange");
    break;
  case Color::deepOrange:
    return F("deep-orange");
    break;
  case Color::brown:
    return F("brown");
    break;
  case Color::grey:
    return F("grey");
    break;
  case Color::blueGrey:
    return F("blue-grey");
    break;
  case Color::black:
    return F("black");
    break;
  case Color::white:
    return F("white");
    break;
  case Color::transparent:
    return F("transparent");
    break;
  }
}
