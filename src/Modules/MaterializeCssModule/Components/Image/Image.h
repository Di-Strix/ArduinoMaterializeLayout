#pragma once

#include <Arduino.h>

#include "DynamicComponentRegistrationService/DynamicComponentRegistrationService.h"
#include "HTMLElement/HTMLElement.h"

template <typename T>
class Image : public HTMLElement<T> {
  private:
  using HTMLElement<T>::appendChild;
  using HTMLElement<T>::removeAllChildren;
  using HTMLElement<T>::removeChild;

  String src;
  String alt;

  public:
  /**
   * @brief Constructs a new Image object
   */
  Image(DynamicComponentRegistrationService<T>* registrationService);

  String getHTML();

  /**
   * @brief Sets the image source
   * 
   * @param newSrc new image source
   */
  void setSrc(String newSrc);

  /**
   * @brief Sets the alternative text for the image
   * 
   * @param newAlt new alternative text
   */
  void setAlt(String newAlt);

  /**
   * @brief Gets the current image source
   * 
   * @return String 
   */
  String getSrc();

  /**
   * @brief Gets the current alternative text
   * 
   * @return String 
   */
  String getAlt();
};

// ======================= IMPLEMENTATION =======================

template <typename T>
Image<T>::Image(DynamicComponentRegistrationService<T>* registrationService)
    : HTMLElement<T>(registrationService)
{
  this->classList.add(F("materialboxed"));
}

template <typename T>
String Image<T>::getHTML()
{
  String id = (String)this->getId();

  String elemTemplate = F("<img class=\"");
  elemTemplate += this->classList.value();
  elemTemplate += F("\">");

  return elemTemplate;
}

template <typename T>
void Image<T>::setSrc(String newSrc)
{

}

template <typename T>
void Image<T>::setAlt(String newAlt)
{

}

template <typename T>
String Image<T>::getSrc()
{

}

template <typename T>
String Image<T>::getAlt()
{
  
}
