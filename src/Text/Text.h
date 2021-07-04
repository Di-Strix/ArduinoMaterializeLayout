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
  StaticText(String text, TextType textType = TextType::p);
  String getText();
  void setText(String text);
  void setTextType(TextType textType);
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
  DynamicText(std::shared_ptr<DynamicComponentRegistrationService<T>> registrationService, String text, TextType textType = TextType::p);
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