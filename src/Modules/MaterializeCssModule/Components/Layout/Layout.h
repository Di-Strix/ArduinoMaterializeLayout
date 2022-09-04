#pragma once

#include <Arduino.h>
#include <list>
#include <memory>
#include <variant>

#include "HTMLElement/HTMLElement.h"

#include "LayoutHelperFunctions.h"
#include "LayoutTypes.h"

template <typename T>
class Layout : public HTMLElement<T> {
  private:
  LayoutType layoutType = LayoutType::NONE;

  public:
  using HTMLElement<T>::HTMLElement;

  virtual ~Layout() = default;

  virtual String getHTML();

  /**
   * @brief Enables or disables vertical align in the container
   *
   * @param state
   *
   * @return void
   */
  void setVerticalAlign(bool state);

  /**
   * @brief Sets layout type for the node
   *
   * @param lt layout type
   */
  void setLayoutType(LayoutType lt);

  /**
   * @brief Returns current layout type
   *
   * @return current layout type
   */
  LayoutType getLayoutType();
};

// ======================= IMPLEMENTATION =======================

template <typename T>
String Layout<T>::getHTML()
{
  String componentTemplate, nestedLayout, className;

  for (auto l : this->children) {
    nestedLayout += l->getHTML();
  }

  componentTemplate = F("<div class=\"");
  componentTemplate += this->classList.value();
  componentTemplate += F("\" ");
  componentTemplate += F("style=\"");
  componentTemplate += this->getInlineStyles();
  componentTemplate += F("\">");
  componentTemplate += nestedLayout;
  componentTemplate += F("</div>");

  return componentTemplate;
}

template <typename T>
void Layout<T>::setVerticalAlign(bool state)
{
  state ? this->classList.add(F("valign-wrapper")) : this->classList.remove(F("valign-wrapper"));
}

template <typename T>
void Layout<T>::setLayoutType(LayoutType lt)
{
  this->classList.remove(getLayoutTypeClass(this->layoutType));

  this->layoutType = lt;
  this->classList.add(getLayoutTypeClass(this->layoutType));
}

template <typename T>
LayoutType Layout<T>::getLayoutType()
{
  return this->layoutType;
}
