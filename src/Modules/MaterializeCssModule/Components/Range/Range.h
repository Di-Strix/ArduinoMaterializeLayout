#pragma once

#include <Arduino.h>
#include <utility>

#include "../BaseClass/MaterializeCssBaseClass.h"
#include "../Field/Field.h"

template <typename T>
class Range : public MaterializeCssBaseClass<T> {
  protected:
  using MaterializeCssBaseClass<T>::appendChild;
  using MaterializeCssBaseClass<T>::removeAllChildren;
  using MaterializeCssBaseClass<T>::removeChild;

  String name;

  public:
  Field<T> field;

  Range(T* argCollection);
  virtual ~Range() = default;

  void getHTML(ResponseWriter writer) override;

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

  /**
   * @brief Sets the range
   *
   * @param min this is what you think it is
   * @param max this is what you think it is
   */
  void setRange(String min, String max);

  /**
   * @brief Gets the current range
   *
   * @return std::pair<String, String>  min, max
   */
  std::pair<String, String> getRange();

  /**
   * @brief Sets step size for the input
   *
   * @param step step size
   */
  void setStep(String step);

  /**
   * @brief Gets current step size
   *
   * @return String step size
   */
  String getStep();
};

template <typename T>
Range<T>::Range(T* argCollection)
    : MaterializeCssBaseClass<T>(argCollection)
    , field(argCollection)
{
  this->field.inputElement.setAttribute(F("type"), F("range"));
  this->field.setWidth(12);

  this->classList.add(F("col"));
  this->setStyle(CSSStyleKey::display, F("flex"));
  this->setStyle(CSSStyleKey::alignItems, F("center"));

  this->setName(String(this->getId()));

  this->appendChild(&this->field);
}

template <typename T>
void Range<T>::getHTML(ResponseWriter writer)
{
  writer(F("<div "));
  writer(this->getAttributes());
  writer(F(">"));
  writer(F("<span>"));
  writer(this->getName());
  writer(F("</span>"));
  this->MaterializeCssBaseClass<T>::getHTML(writer);
  writer(F("</div>"));
}

template <typename T>
String Range<T>::getName()
{
  return this->name;
}

template <typename T>
void Range<T>::setName(String name)
{
  this->name = name;
}

template <typename T>
String Range<T>::getDefaultValue()
{
  return this->field.getDefaultValue();
}

template <typename T>
void Range<T>::setDefaultValue(String defaultValue)
{
  this->field.setDefaultValue(defaultValue);
}

template <typename T>
String Range<T>::getCurrentValue()
{
  return this->field.getCurrentValue();
}

template <typename T>
void Range<T>::setRange(String min, String max)
{
  this->field.inputElement.setAttribute(F("min"), min);
  this->field.inputElement.setAttribute(F("max"), max);
}

template <typename T>
std::pair<String, String> Range<T>::getRange()
{
  return {
    this->field.inputElement.getAttribute(F("min")), this->field.inputElement.getAttribute(F("max"))
  };
}

template <typename T>
void Range<T>::setStep(String step)
{
  this->field.inputElement.setAttribute(F("step"), step);
}

template <typename T>
String Range<T>::getStep()
{
  return this->field.inputElement.getAttribute(F("step"));
}
