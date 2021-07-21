#ifndef _MATERIALIZE_LAYOUT_ELEMENT_H_
#define _MATERIALIZE_LAYOUT_ELEMENT_H_

#include <Arduino.h>
#include <memory>
#include <variant>
#include <vector>

#include "../DynamicComponentRegistrationService/DynamicComponentRegistrationService.h"
#include "../Container/Container.h"

enum class LayoutTypes
{
  NONE,
  ROW,
  COLUMN
};

template <typename T>
class Layout : public Container<T>
{
  template <typename U>
  friend class Tab;
  friend class Page;

private:
  std::vector<std::shared_ptr<Layout<T>>> layout;
  LayoutTypes layoutType = LayoutTypes::NONE;
  using Container<T>::getDynamicComponentRegistrationServerPtr;

protected:
  virtual String getHTML();
  virtual bool emit(size_t id, String value);

public:
  /**
   * @brief Constructs a new Layout object
   * 
   * @param registrationService dynamic component registration service that is used in the current page
   * @param lt Layout type
   */
  Layout(std::shared_ptr<DynamicComponentRegistrationService<T>> registrationService, LayoutTypes lt = LayoutTypes::NONE);

  /**
   * @brief Enables or disables vertical align in the container
   * 
   * @param state 
   * 
   * @return void
   */
  void setVerticalAlign(bool state);

  /**
   * @brief Creates a Row in the current layout
   * 
   * @return std::shared_ptr<Layout<T>> where T is type of dynamic component registration service. If the page is created with MaterializeLayout, you can use 'Layout_t' shortcut instead
   */
  std::shared_ptr<Layout<T>> createRow();

  /**
   * @brief Creates a Column in the current layout
   * 
   * @return std::shared_ptr<Layout<T>> where T is type of dynamic component registration service. If the page is created with MaterializeLayout, you can use 'Layout_t' shortcut instead
   */
  std::shared_ptr<Layout<T>> createColumn();
};

// ======================= IMPLEMENTATION =======================

template <typename T>
Layout<T>::Layout(std::shared_ptr<DynamicComponentRegistrationService<T>> registrationService, LayoutTypes lt) : Container<T>(registrationService)
{
  this->layoutType = lt;

  switch (this->layoutType)
  {
  case LayoutTypes::NONE:
  default:
    break;
  case LayoutTypes::ROW:
    this->classList.add(F("row"));
    break;
  case LayoutTypes::COLUMN:
    this->classList.add(F("col"));
    break;
  }
}

template <typename T>
String Layout<T>::getHTML()
{
  String componentTemplate, nestedLayout, className;

  for (auto l : this->layout)
  {
    if (l)
    {
      nestedLayout += l->getHTML();
    }
  }

  componentTemplate = F("<div class=\"");
  componentTemplate += this->classList.value();
  componentTemplate += F("\">");
  componentTemplate += this->compileComponents();
  componentTemplate += nestedLayout;
  componentTemplate += F("</div>");

  return componentTemplate;
}

template <typename T>
std::shared_ptr<Layout<T>> Layout<T>::createRow()
{
  auto r = std::make_shared<Layout<T>>(this->getDynamicComponentRegistrationServerPtr(), LayoutTypes::ROW);
  this->layout.push_back(r);

  return r;
}

template <typename T>
std::shared_ptr<Layout<T>> Layout<T>::createColumn()
{
  auto c = std::make_shared<Layout<T>>(this->getDynamicComponentRegistrationServerPtr(), LayoutTypes::COLUMN);
  this->layout.push_back(c);

  return c;
}

template <typename T>
bool Layout<T>::emit(size_t id, String value)
{
  bool found = false;
  if (this->emitOnContainer(id, value))
    found = true;
  else
    for (auto l : this->layout)
    {
      if (l)
        found = l->emit(id, value);
      if (found)
        break;
    }

  return found;
}

template <typename T>
void Layout<T>::setVerticalAlign(bool state)
{
  state ? this->classList.add(F("valign-wrapper")) : this->classList.remove(F("valign-wrapper"));
}

#endif //_MATERIALIZE_LAYOUT_H_