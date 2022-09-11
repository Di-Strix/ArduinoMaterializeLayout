#pragma once

#include <Arduino.h>
#include <list>
#include <memory>
#include <variant>

#include "../BaseClass/MaterializeCssBaseClass.h"

#include "LayoutHelperFunctions.h"
#include "LayoutTypes.h"

template <typename T>
class Layout : public MaterializeCssBaseClass<T> {
  private:
  LayoutType layoutType = LayoutType::NONE;

  public:
  using MaterializeCssBaseClass<T>::MaterializeCssBaseClass;

  virtual ~Layout() = default;

  virtual void getHTML(ResponseWriter writer) override;

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
void Layout<T>::getHTML(ResponseWriter writer)
{
  writer(F("<div "));
  writer(this->getAttributes());
  writer(F(">"));
  for (auto l : this->children) {
    l->getHTML(writer);
  }
  writer(F("</div>"));
}

template <typename T>
void Layout<T>::setVerticalAlign(bool state)
{
  if (state) {
    if (this->classList.contains(F("valign-wrapper")))
      return;
    this->classList.add(F("valign-wrapper"));
  } else {
    this->classList.remove(F("valign-wrapper"));
  }
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
