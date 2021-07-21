#ifndef _MATERIALIZE_LAYOUT_TEXT_H_
#define _MATERIALIZE_LAYOUT_TEXT_H_

#include <Arduino.h>
#include <memory>

#include "../DynamicComponentRegistrationService\DynamicComponentRegistrationService.h"
#include "../HTMLElement/HTMLElement.h"

using DynamicTextGetter = std::function<String()>;

enum class TextType
{
  h1,
  h2,
  h3,
  h4,
  h5,
  h6,
  p
};

class StaticText : public HTMLElement
{
  template <typename T>
  friend class Container;

private:
  String text;
  TextType textType;

protected:
  bool dynamic = false;
  virtual String getHTML();

public:
  /**
   * @brief Constructs a new Static Text object
   * 
   * @param text
   * @param textType text type from the TextType enum
   */
  StaticText(String text, TextType textType = TextType::p);

  /**
   * @brief Gets the current text
   * 
   * @return String 
   */
  String getText();

  /**
   * @brief Sets the text. If text is static, it will be updated only from the next render
   * 
   * @param text new text
   */
  void setText(String text);

  /**
   * @brief Set the type of the text from the TextType enum. If text is static, it will be updated only from the next render
   * 
   * @param textType 
   */
  void setTextType(TextType textType);

  /**
   * @brief Gets the type of the text
   * 
   * @return TextType enum
   */
  TextType getTextType();
};

template <typename T>
class DynamicText : public StaticText
{
  template <typename A>
  friend class Container;

private:
  std::shared_ptr<DynamicComponentRegistrationService<T>> registrationService;
  unregisterFn unregister;
  using StaticText::dynamic;

public:
  /**
   * @brief Constructs a new Dynamic Text object
   * 
   * @param registrationService dynamic component registration service that is used in the current page
   * @param text 
   * @param textType text type from the TextType enum
   */
  DynamicText(std::shared_ptr<DynamicComponentRegistrationService<T>> registrationService, String text, TextType textType = TextType::p);

  /**
   * @brief Construct a new Dynamic Text object
   * 
   * @param registrationService dynamic component registration service that is used in the current page
   * @param textGetter function which is called when an update cycle is performing
   * @param textType text type from the TextType enum
   */
  DynamicText(std::shared_ptr<DynamicComponentRegistrationService<T>> registrationService, DynamicTextGetter textGetter, TextType textType = TextType::p);
  ~DynamicText();
};

// ======================= IMPLEMENTATION =======================

template <typename T>
DynamicText<T>::DynamicText(std::shared_ptr<DynamicComponentRegistrationService<T>> registrationService, String text, TextType textType) : StaticText(text, textType)
{
  this->registrationService = registrationService;
  this->dynamic = true;

  if (this->registrationService)
    this->unregister = this->registrationService->registerDynamicGetter({this->getId(), [=]()
                                                                         { return this->getText(); }});
  else
    Serial.println("Reg server does not exist");
}

template <typename T>
DynamicText<T>::DynamicText(std::shared_ptr<DynamicComponentRegistrationService<T>> registrationService, DynamicTextGetter textGetter, TextType textType) : StaticText(textGetter(), textType)
{
  this->registrationService = registrationService;
  this->dynamic = true;

  if (this->registrationService)
    this->unregister = this->registrationService->registerDynamicGetter({this->getId(), [=]()
                                                                         {
                                                                           String text = textGetter();
                                                                           this->setText(text);
                                                                           return text;
                                                                         }});
  else
    Serial.println("Reg server does not exist");
}

template <typename T>
DynamicText<T>::~DynamicText()
{
  this->unregister();
}

#endif //_MATERIALIZE_LAYOUT_TEXT_H_