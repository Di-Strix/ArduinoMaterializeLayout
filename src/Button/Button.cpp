#include "Button.h"

Button::Button(String caption, ButtonCallback callback)
{
  caption.trim();
  this->caption = caption;
  this->callback = callback;
}

String Button::getCaption()
{
  return this->caption;
}

void Button::setCaption(String caption)
{
  caption.trim();
  this->caption = caption;
}

void Button::setCallback(ButtonCallback callback)
{
  this->callback = callback;
}

String Button::getHTML()
{
  String elemTemplate = F("<a data-id=\"");
  elemTemplate += (String)this->getId();
  elemTemplate += F("\" data-emitOnClick=\"true\" class=\"waves-effect waves-light btn ");
  elemTemplate += this->getWidthClass();
  elemTemplate += F("\">");
  elemTemplate += this->caption;
  elemTemplate += F("</a>\n");

  return elemTemplate;
}

bool Button::emit(size_t id, String value)
{
  if (id == this->getId())
  {
    this->callback();
    return true;
  }

  return false;
}