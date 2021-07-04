#ifndef _MATERIALIZE_LAYOUT_DYNAMIC_COMPONENT_REGISTRATION_SERVICE_H_
#define _MATERIALIZE_LAYOUT_DYNAMIC_COMPONENT_REGISTRATION_SERVICE_H_

#include <Arduino.h>
#include <vector>
#include <functional>

typedef std::function<void()> unregisterFn;

template <typename T>
class DynamicComponentRegistrationService
{
private:
  std::vector<T> registrations;
  std::function<bool(T, T)> areEqual;

public:
  DynamicComponentRegistrationService(std::function<bool(T, T)> areEqualChecker);
  unregisterFn registerDynamicGetter(T dynamicDataGetter);
  void unregisterGetter(T dynamicDataGetter);
  std::vector<T> getRegistrations();
};

// ======================= IMPLEMENTATION =======================

template <typename T>
DynamicComponentRegistrationService<T>::DynamicComponentRegistrationService(std::function<bool(T, T)> areEqualChecker)
{
  this->areEqual = areEqualChecker;
}

template <typename T>
unregisterFn DynamicComponentRegistrationService<T>::registerDynamicGetter(T dynamicDataGetter)
{
  this->registrations.push_back(dynamicDataGetter);

  return [=]()
  {
    this->unregisterGetter(dynamicDataGetter);
  };
}

template <typename T>
void DynamicComponentRegistrationService<T>::unregisterGetter(T dynamicDataGetter)
{
  std::vector<T> filteredRegistrations;

  for (T registration : this->registrations)
  {
    if (!this->areEqual(registration, dynamicDataGetter))
      filteredRegistrations.push_back(registration);
  }

  this->registrations.clear();
  this->registrations = filteredRegistrations;
}

template <typename T>
std::vector<T> DynamicComponentRegistrationService<T>::getRegistrations()
{
  return this->registrations;
}

#endif //_MATERIALIZE_LAYOUT_DYNAMIC_COMPONENT_REGISTRATION_SERVICE_H_