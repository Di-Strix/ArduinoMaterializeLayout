#pragma once

#include <Arduino.h>
#include <memory>

#include "../BaseClass/MaterializeCssBaseClass.h"

#include "MaterializeLayoutTypes.h"

enum class TextType {
  h1,
  h2,
  h3,
  h4,
  h5,
  h6,
  p
};

template <typename T>
class StaticText : public MaterializeCssBaseClass<T> {
  private:
  using HTMLElement<T>::appendChild;
  using HTMLElement<T>::removeAllChildren;
  using HTMLElement<T>::removeChild;

  String text;
  TextType textType = TextType::p;

  protected:
  constexpr virtual bool isDynamic() { return false; };

  public:
  void getHTML(ResponseWriter writer) override;

  using MaterializeCssBaseClass<T>::MaterializeCssBaseClass;

  virtual ~StaticText() = default;

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
class DynamicText : public StaticText<T> {
  private:
  constexpr virtual bool isDynamic() { return true; };

  public:
  using StaticText<T>::StaticText;

  virtual ~DynamicText() = default;
};

// ======================= IMPLEMENTATION =======================

template <typename T>
void StaticText<T>::setText(String text)
{
  text.trim();
  this->text = text;

  if (this->isDynamic()) {
    String result;

    do {
      DynamicJsonDocument doc(JSON_OBJECT_SIZE(1) + JSON_STRING_SIZE(this->text.length()) + 128);
      doc["textContent"] = this->text;

      serializeJson(doc, result);
    } while (false);

    this->dispatch(result);
  }
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
void StaticText<T>::getHTML(ResponseWriter writer)
{
  String tag = "";
  String id = (String)this->getId();

  switch (this->textType) {
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

  writer(F("<"));
  writer(tag);
  writer(F(" class=\""));
  writer(this->classList.value());
  writer(F("\" data-id=\""));
  writer(id);
  writer(F("\" data-dynamic=\""));
  writer(this->isDynamic() ? F("true") : F("false"));
  writer(F("\" "));
  writer(F("style=\""));
  writer(this->getInlineStyles());
  writer(F("\">"));
  writer(this->getText());
  writer(F("</"));
  writer(tag);
  writer(F(">"));
}
