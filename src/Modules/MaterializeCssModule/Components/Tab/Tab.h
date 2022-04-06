#pragma once

#include <Arduino.h>
#include <list>
#include <memory>
#include <variant>

#include "DynamicComponentRegistrationService/DynamicComponentRegistrationService.h"

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
  Tab(DynamicComponentRegistrationService<T>* registrationService);

  /**
   * @brief Gets the name of the tab
   *
   * @return String
   */
  String getTabName();

  void setTabName(String tabName);
};

// ======================= IMPLEMENTATION =======================

template <typename T>
Tab<T>::Tab(DynamicComponentRegistrationService<T>* registrationService)
    : HTMLElement<T>(registrationService)
{
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
