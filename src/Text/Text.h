#ifndef _MATERIALIZE_LAYOUT_TEXT_H_
#define _MATERIALIZE_LAYOUT_TEXT_H_

#include <Arduino.h>
#include <memory>

#include "../DynamicComponentRegistrationService\DynamicComponentRegistrationService.h"
#include "../HTMLElement/HTMLElement.h"

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

template <typename T>
class StaticText : public HTMLElement<T>
{
private:
  using HTMLElement<T>::appendChild;
  using HTMLElement<T>::removeAllChildren;
  using HTMLElement<T>::removeChild;

  String text;
  TextType textType = TextType::p;

protected:
  constexpr virtual bool isDynamic() { return false; };

public:
  String getHTML();
  /**
   * @brief Constructs a new Static Text object
   * 
   * @param text
   * @param textType text type from the TextType enum
   */
  using HTMLElement<T>::HTMLElement;

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
class DynamicText : public StaticText<T>
{
private:
  unregisterFn unregister;

  constexpr virtual bool isDynamic() { return true; };

public:
  DynamicText(DynamicComponentRegistrationService<T> *registrationService);
  ~DynamicText();
};

// ======================= IMPLEMENTATION =======================

template <typename T>
void StaticText<T>::setText(String text)
{
  text.trim();
  this->text = text;
}

template <typename T>
void StaticText<T>::setTextType(TextType textType)
{
  this->textType = textType;
}

template <typename T>
String StaticText<T>::getText()
{
  return this->text;
}

template <typename T>
TextType StaticText<T>::getTextType()
{
  return this->textType;
}

template <typename T>
String StaticText<T>::getHTML()
{
  String tag = "";
  String id = (String)this->getId();

  switch (this->textType)
  {
  case TextType::h1:
    tag = "h1";
    break;
  case TextType::h2:
    tag = "h2";
    break;
  case TextType::h3:
    tag = "h3";
    break;
  case TextType::h4:
    tag = "h4";
    break;
  case TextType::h5:
    tag = "h5";
    break;
  case TextType::h6:
    tag = "h6";
    break;
  case TextType::p:
  default:
    tag = "p";
    break;
  }

  String elemTemplate = F("<");
  elemTemplate += tag;
  elemTemplate += F(" class=\"");
  elemTemplate += this->classList.value();
  elemTemplate += F("\" data-id=\"");
  elemTemplate += id;
  elemTemplate += F("\" data-dynamic=\"");
  elemTemplate += this->isDynamic() ? F("true") : F("false");
  elemTemplate += F("\">");
  elemTemplate += this->getText();
  elemTemplate += F("</");
  elemTemplate += tag;
  elemTemplate += F(">");

  return elemTemplate;
}

template <typename T>
DynamicText<T>::DynamicText(DynamicComponentRegistrationService<T> *registrationService) : StaticText<T>(registrationService)
{
  this->unregister = registrationService->registerDynamicGetter({this->getId(), [this]() -> UpdateMsg
                                                                 { return {F("MaterializeCssHandler"), this->getText()}; }});
}

template <typename T>
DynamicText<T>::~DynamicText()
{
  this->unregister();
}

#endif //_MATERIALIZE_LAYOUT_TEXT_H_