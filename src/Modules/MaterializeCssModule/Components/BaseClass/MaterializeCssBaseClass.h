#pragma once

#include <Arduino.h>

#include "HTMLElement/HTMLElement.h"

template <typename T>
class MaterializeCssBaseClass : public HTMLElement<T> {
  public:
  MaterializeCssBaseClass(T* argCollection);
  virtual ~MaterializeCssBaseClass() = default;
};

// ======================= IMPLEMENTATION =======================

template <typename T>
MaterializeCssBaseClass<T>::MaterializeCssBaseClass(T* argCollection)
    : HTMLElement<T>(argCollection)
{
  this->classList.onChange.subscribe([this](String className, String newClassName, ClassChangeType changeType) {
    String result;

    do {
      String type;

      switch (changeType) {
      case ClassChangeType::add:
        type = F("add");
        break;
      case ClassChangeType::remove:
        type = F("remove");
        break;
      case ClassChangeType::replace:
        type = F("replace");
        break;
      }

      DynamicJsonDocument doc(JSON_OBJECT_SIZE(1) + JSON_ARRAY_SIZE(1) * (JSON_OBJECT_SIZE(2) + JSON_STRING_SIZE(className.length()) + JSON_STRING_SIZE(type.length())) + 128);
      auto obj = doc.createNestedArray(F("classListDiff")).createNestedObject();
      obj[F("className")] = className;
      obj[F("newClassName")] = newClassName;
      obj[F("changeType")] = type;

      serializeJson(doc, result);
    } while (false);

    this->dispatch(result);
  });

  this->onInlineStylesChange.subscribe([this](String key, String value) {
    String result;

    do {
      DynamicJsonDocument doc(JSON_OBJECT_SIZE(1) + JSON_STRING_SIZE(value.length()) + 128);
      auto obj = doc.createNestedObject(F("styles"));
      obj[key] = value;

      serializeJson(doc, result);
    } while (false);

    this->dispatch(result);
  });

  this->onAttributeChange.subscribe([this](String key, String value) {
    String result;

    do {
      DynamicJsonDocument doc(JSON_OBJECT_SIZE(1) + JSON_STRING_SIZE(value.length()) + 128);
      auto obj = doc.createNestedObject(F("attributes"));
      obj[key] = value;

      serializeJson(doc, result);
    } while (false);

    this->dispatch(result);
  });
}
