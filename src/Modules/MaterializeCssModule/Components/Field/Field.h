#pragma once

#include <Arduino.h>

#include "HTMLElement/HTMLElement.h"

template <typename T>
class Field : public HTMLElement<T> {
  private:
  using HTMLElement<T>::appendChild;
  using HTMLElement<T>::removeAllChildren;
  using HTMLElement<T>::removeChild;

  String name;
  String defaultValue;
  String currentValue;

  void onEmit(String value);

  public:
  /**
   * @brief Constructs a new Field object
   *
   * @param name the name of the field
   * @param defaultValue default value of the field
   */
  Field(T argCollection);

  virtual ~Field() = default;

  String getHTML();

  /**
   * @brief Gets the name of the field
   *
   * @return String
   */
  String getName();

  /**
   * @brief Sets the name of the field
   *
   * @param name the new name of the field
   *
   * @return void
   */
  void setName(String name);

  /**
   * @brief Gets the default value of the field
   *
   * @return String
   */
  String getDefaultValue();

  /**
   * @brief Sets the default value of the field
   *
   * @param defaultValue default value of the field
   *
   * @return void
   */
  void setDefaultValue(String defaultValue);

  /**
   * @brief Gets the text that is currently written in the field
   *
   * @return String
   */
  String getCurrentValue();
};

// ======================= IMPLEMENTATION =======================

template <typename T>
Field<T>::Field(T argCollection)
    : HTMLElement<T>(argCollection)
{
  this->name = (String)this->getId();

  this->classList.add(F("input-field"));
  this->classList.add(F("col"));
}

template <typename T>
String Field<T>::getName()
{
  return this->name;
}

template <typename T>
void Field<T>::setName(String name)
{
  name.trim();
  this->name = name;
}

template <typename T>
String Field<T>::getDefaultValue()
{
  return this->defaultValue;
}

template <typename T>
void Field<T>::setDefaultValue(String defaultValue)
{
  defaultValue.trim();
  this->defaultValue = defaultValue;
}

template <typename T>
String Field<T>::getCurrentValue()
{
  return this->currentValue;
}

template <typename T>
String Field<T>::getHTML()
{
  String id = (String)this->getId();

  String elemTemplate = F("<div class=\"");
  elemTemplate += this->classList.value();
  elemTemplate += F("\"><input value=\"");
  elemTemplate += this->defaultValue;
  elemTemplate += F("\" type=\"text\" data-id=\"");
  elemTemplate += id;
  elemTemplate += F("\" data-MCSS-emitOnChange=\"true\" id=\"");
  elemTemplate += id;
  elemTemplate += F("\"><label for=\"");
  elemTemplate += id;
  elemTemplate += F("\">");
  elemTemplate += this->name;
  elemTemplate += F("</label></div>");

  return elemTemplate;
}

template <typename T>
void Field<T>::onEmit(String value)
{
  this->currentValue = value;
}
