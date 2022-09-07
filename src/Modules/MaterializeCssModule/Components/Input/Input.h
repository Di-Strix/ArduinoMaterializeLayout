#pragma once

#include <Arduino.h>

#include "../BaseClass/MaterializeCssBaseClass.h"

template <typename T>
class Input : public MaterializeCssBaseClass<T> {
  private:
  using MaterializeCssBaseClass<T>::appendChild;
  using MaterializeCssBaseClass<T>::removeAllChildren;
  using MaterializeCssBaseClass<T>::removeChild;

  void onEmit(String value) override;

  protected:
  String currentValue;

  public:
  Input(T* argCollection);
  virtual ~Input() = default;

  String getHTML() override;

  /**
   * @brief Gets the text that is currently written in the field
   *
   * @return String
   */
  String getCurrentValue();
};

template <typename T>
Input<T>::Input(T* argCollection)
    : MaterializeCssBaseClass<T>(argCollection)
{
  this->setAttribute("type", "text");
  this->setAttribute("data-MCSS-emitOnChange", "true");
  this->setAttribute("id", String(this->getId()));
}

template <typename T>
String Input<T>::getHTML()
{
  String elemTemplate = F("<input ");
  elemTemplate += this->getAttributes();
  elemTemplate += F(">");

  return elemTemplate;
}

template <typename T>
void Input<T>::onEmit(String value)
{
  this->currentValue = value;
}

template <typename T>
String Input<T>::getCurrentValue()
{
  return this->currentValue;
}