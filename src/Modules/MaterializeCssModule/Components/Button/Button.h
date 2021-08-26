#ifndef _MATERIALIZE_LAYOUT_BUTTON_H_
#define _MATERIALIZE_LAYOUT_BUTTON_H_

#include <Arduino.h>
#include <functional>

#include "../../../../DynamicComponentRegistrationService/DynamicComponentRegistrationService.h"
#include "../../../../HTMLElement/HTMLElement.h"

typedef std::function<void()> ButtonCallback;

template <typename T>
class Button : public HTMLElement<T>
{
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
  Button(DynamicComponentRegistrationService<T> *registrationService);

  virtual String getHTML();

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
Button<T>::Button(DynamicComponentRegistrationService<T> *registrationService) : HTMLElement<T>(registrationService)
{
  this->classList.add(F("waves-effect"));
  this->classList.add(F("waves-light"));
  this->classList.add(F("btn"));
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
String Button<T>::getHTML()
{
  String elemTemplate = F("<a data-id=\"");
  elemTemplate += (String)this->getId();
  elemTemplate += F("\" data-MCSS-emitOnClick=\"true\" class=\"");
  elemTemplate += this->classList.value();
  elemTemplate += F("\"");
  if (this->getWidth() > 0)
  {
    elemTemplate += F("\"style=\"width:calc(100%/12*");
    elemTemplate += this->getWidth();
    elemTemplate += F(");\"");
  }
  elemTemplate += F(">");
  elemTemplate += this->caption;
  elemTemplate += F("</a>\n");

  return elemTemplate;
}

template <typename T>
void Button<T>::onEmit(String value)
{
  this->callback();
}

#endif //_MATERIALIZE_LAYOUT_BUTTON_H_