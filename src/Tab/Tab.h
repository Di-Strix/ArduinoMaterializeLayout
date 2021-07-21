#ifndef _MATERIALIZE_LAYOUT_TAB_H_
#define _MATERIALIZE_LAYOUT_TAB_H_

#include <Arduino.h>
#include <vector>
#include <variant>
#include <memory>

#include "../DynamicComponentRegistrationService/DynamicComponentRegistrationService.h"
#include "../Layout/Layout.h"

template <typename T>
class Tab : public HTMLElement
{
  template <typename U>
  friend class TabGroup;

private:
  String name;
  std::vector<std::shared_ptr<Layout<T>>> layout;
  std::shared_ptr<DynamicComponentRegistrationService<T>> registrationService;

protected:
  virtual String getHTML();
  virtual bool emit(size_t id, String value);

public:
  /**
   * @brief Constructs a new Tab object
   * 
   * @param registrationService dynamic component registration service that is used in the current page
   * @param tabName the name of the tab
   */
  Tab(std::shared_ptr<DynamicComponentRegistrationService<T>> registrationService, String tabName);

  /**
   * @brief Gets the name of the tab
   * 
   * @return String 
   */
  String getName();

  /**
   * @brief Creates a Layout in the tab
   * 
   * @return std::shared_ptr<Layout<T>> where T is type of dynamic component registration service. If the page is created with MaterializeLayout, you can use 'Layout_t' shortcut instead
   */
  std::shared_ptr<Layout<T>> createLayout();
};

// ======================= IMPLEMENTATION =======================

template <typename T>
Tab<T>::Tab(std::shared_ptr<DynamicComponentRegistrationService<T>> registrationService, String tabName)
{
  this->registrationService = registrationService;

  tabName.trim();
  this->name = tabName;
}

template <typename T>
String Tab<T>::getHTML()
{
  String html;

  for (auto l : this->layout)
  {
    if (!l)
      continue;
    html += l->getHTML();
  }

  return html;
}

template <typename T>
String Tab<T>::getName()
{
  return this->name;
}

template <typename T>
std::shared_ptr<Layout<T>> Tab<T>::createLayout()
{
  auto l = std::make_shared<Layout<T>>(this->registrationService, LayoutTypes::COLUMN);
  l->setWidth(12);
  this->layout.push_back(l);

  return l;
}

template <typename T>
bool Tab<T>::emit(size_t id, String value)
{
  bool found = false;
  for (auto l : this->layout)
  {
    if (!l)
      continue;
    found = l->emit(id, value);

    if (found)
      break;
  }

  return found;
}

#endif //_MATERIALIZE_LAYOUT_TAB_H_