#pragma once

#include <Arduino.h>
#include <list>
#include <memory>
#include <variant>

#include "../Layout/Layout.h"

template <typename T>
class Tab : public HTMLElement<T> {
  protected:
  String name;

  public:
  /**
   * @brief Constructs a new Tab object
   *
   * @param registrationService dynamic component registration service that is used in the current page
   * @param tabName the name of the tab
   */
  Tab(T* argCollection);

  virtual ~Tab() = default;

  /**
   * @brief Gets the name of the tab
   *
   * @return String
   */
  virtual String getTabName();

  /**
   * @brief Sets the name of the tab
   *
   * @param tabName
   */
  virtual void setTabName(String tabName);
};

// ======================= IMPLEMENTATION =======================

template <typename T>
Tab<T>::Tab(T* argCollection)
    : HTMLElement<T>(argCollection)
{
  this->setTabName(String(this->getId()));
}

template <typename T>
String Tab<T>::getTabName()
{
  return this->name;
}

template <typename T>
void Tab<T>::setTabName(String tabName)
{
  tabName.trim();
  if (tabName.isEmpty())
    return;

  this->name = tabName;
}
