#include "HTMLElement.h"

HTMLElement::HTMLElement()
{
  this->id = IdGenerator::Instance().getId();
}

String HTMLElement::getHTML()
{
  return "";
}

void HTMLElement::setWidth(uint8_t width)
{
  if (width <= 12)
    this->width = width;
}

uint8_t HTMLElement::getWidth()
{
  return this->width;
}

size_t HTMLElement::getId()
{
  return this->id;
}

String HTMLElement::getWidthClass()
{
  if (this->width > 0)
    return "s" + (String)this->width;
  return "";
}