#include "Field.h"

Field::Field(String name, String defaultValue)
{
  name.trim();
  defaultValue.trim();

  this->name = name;
  this->defaultValue = defaultValue;
}

String Field::getName()
{
  return this->name;
}

void Field::setName(String name)
{
  name.trim();
  this->name = name;
}

String Field::getDefaultValue()
{
  return this->defaultValue;
}

void Field::setDefaultValue(String defaultValue)
{
  defaultValue.trim();
  this->defaultValue = defaultValue;
}

String Field::getCurrentValue()
{
  return this->currentValue;
}

String Field::getHTML()
{
  String id = (String)this->getId();

  String elemTemplate = F("<div class=\"input-field col ");
  elemTemplate += this->getWidthClass();
  elemTemplate += F("\"><input value=\"");
  elemTemplate += this->defaultValue;
  elemTemplate += F("\" type=\"text\" data-id=\"");
  elemTemplate += id;
  elemTemplate += F("\" data-emitOnChange=\"true\" id=\"");
  elemTemplate += id;
  elemTemplate += F("\"><label for=\"");
  elemTemplate += id;
  elemTemplate += F("\">");
  elemTemplate += this->name;
  elemTemplate += F("</label></div>");

  return elemTemplate;
}

bool Field::emit(size_t id, String value)
{
  if (this->getId() == id)
  {
    this->currentValue = value;
    return true;
  }
  return false;
}