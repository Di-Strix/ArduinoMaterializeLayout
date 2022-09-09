#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <functional>

#include "../BaseClass/MaterializeCssBaseClass.h"

typedef std::function<void()> ButtonCallback;

template <typename T>
class Button : public MaterializeCssBaseClass<T> {
  private:
  using HTMLElement<T>::appendChild;
  using HTMLElement<T>::removeAllChildren;
  using HTMLElement<T>::removeChild;

  String caption;
  ButtonCallback callback;

  void onEmit(String value);

  public:
  /**
   * @brief Constructs a new Button object
   *
   * @param caption the caption of the button
   * @param callback callback that is called when the button is clicked
   */
  Button(T* argCollection);

  virtual ~Button() = default;

  virtual void getHTML(ResponseWriter writer);

  /**
   * @brief Gets the caption of the button
   *
   * @return String
   */
  String getCaption();

  /**
   * @brief Sets the caption of the button
   *
   * @param caption the new caption of the button
   *
   * @return void
   */
  void setCaption(String caption);

  /**
   * @brief Sets callback that is called when the button is clicked
   *
   * @param callback
   *
   * @return void
   */
  void setCallback(ButtonCallback callback);
};

// ======================= IMPLEMENTATION =======================

template <typename T>
Button<T>::Button(T* argCollection)
    : MaterializeCssBaseClass<T>(argCollection)
{
  this->classList.add(F("waves-effect"));
  this->classList.add(F("waves-light"));
  this->classList.add(F("btn"));

  this->setAttribute("data-id", (String)this->getId());
  this->setAttribute("data-MCSS-emitOnClick", "true");
}

template <typename T>
String Button<T>::getCaption()
{
  return this->caption;
}

template <typename T>
void Button<T>::setCaption(String caption)
{
  caption.trim();
  this->caption = caption;
}

template <typename T>
void Button<T>::setCallback(ButtonCallback callback)
{
  this->callback = callback;
}

template <typename T>
void Button<T>::getHTML(ResponseWriter writer)
{
  writer(F("<a "));
  writer(this->getAttributes());
  if (this->getWidth() > 0) {
    writer(F(" style=\""));
    writer(F("width:calc(100%/12*"));
    writer(String(this->getWidth()));
    writer(F(");\""));
  }
  writer(F(">"));
  writer(this->caption);
  writer(F("</a>\n"));
}

template <typename T>
void Button<T>::onEmit(String value)
{
  if (this->callback)
    this->callback();
}
