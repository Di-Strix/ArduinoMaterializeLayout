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

  void getHTML(ResponseWriter writer) override;

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
void Input<T>::getHTML(ResponseWriter writer)
{
  writer(String(F("<input ")));
  writer(String(this->getAttributes()));
  writer(String(F(">")));
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