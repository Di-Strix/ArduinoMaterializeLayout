#include "Button.h"

Button::Button(String caption, ButtonCallback callback)
{
  caption.trim();
  this->caption = caption;
  this->callback = callback;

  this->classList.add("waves-effect");
  this->classList.add("waves-light");
  this->classList.add("btn");
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
  elemTemplate += F("\" data-emitOnClick=\"true\" class=\"");
  elemTemplate += this->classList.value();
  elemTemplate += F("\"");
  if (this->getWidth() > 0)
  {
    elemTemplate += F("\"style=\"width:calc(100%/12*");
    elemTemplate += this->getWidth();
    elemTemplate += F(");");
  }
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