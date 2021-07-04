#include "Text.h"

StaticText::StaticText(String text, TextType textType)
{
  this->setText(text);
  this->setTextType(textType);
}

void StaticText::setText(String text)
{
  text.trim();
  this->text = text;
}

void StaticText::setTextType(TextType textType)
{
  this->textType = textType;
}

String StaticText::getText()
{
  return this->text;
}

TextType StaticText::getTextType()
{
  return this->textType;
}

String StaticText::getHTML()
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
  elemTemplate += this->getWidthClass();
  elemTemplate += F("\" data-id=\"");
  elemTemplate += id;
  elemTemplate += F("\" data-dynamic=\"");
  elemTemplate += (String)(this->dynamic ? F("true") : F("false"));
  elemTemplate += F("\">");
  elemTemplate += this->getText();
  elemTemplate += F("</");
  elemTemplate += tag;
  elemTemplate += F(">");

  return elemTemplate;
}