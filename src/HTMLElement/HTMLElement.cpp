#include "HTMLElement.h"

HTMLElement::HTMLElement()
{
  this->id = IdGenerator::Instance().getId();
}

String HTMLElement::getHTML()
{
  return "";
}

String HTMLElement::getWidthClassPrefix(ScreenSize screenSize)
{
  switch (screenSize)
  {
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

void HTMLElement::setWidth(uint8_t width, ScreenSize screenSize)
{
  if (width > 12)
    return;

  String classPrefix = this->getWidthClassPrefix(screenSize);

  this->classList.remove(this->widths[(size_t)screenSize]);

  if (width != 0)
  {
    this->widths[(size_t)screenSize] = classPrefix + String(width);
    this->classList.add(this->widths[(size_t)screenSize]);
  }
}

uint8_t HTMLElement::getWidth(ScreenSize screenSize)
{
  String className = this->widths[(uint8_t)screenSize];

  if (className.isEmpty())
    return 0;

  size_t classPrefixLength = this->getWidthClassPrefix(screenSize).length();

  return className.substring(classPrefixLength - 1).toInt();
}

size_t HTMLElement::getId()
{
  return this->id;
}

