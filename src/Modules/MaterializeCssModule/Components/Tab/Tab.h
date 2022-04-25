#pragma once

#include <Arduino.h>
#include <list>
#include <memory>
#include <variant>

#include "../Layout/Layout.h"

template <typename T>
class Tab : public HTMLElement<T> {
  private:
  String name;

  public:
  /**
   * @brief Constructs a new Tab object
   *
   * @param registrationService dynamic component registration service that is used in the current page
   * @param tabName the name of the tab
   */
  Tab(T argCollection);

  /**
   * @brief Gets the name of the tab
   *
   * @return String
   */
  String getTabName();

  /**
   * @brief Sets the name of the tab 
   *
   * @param tabName
   */
  void setTabName(String tabName);
};

// ======================= IMPLEMENTATION =======================

template <typename T>
Tab<T>::Tab(T argCollection)
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
