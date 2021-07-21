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
  /**
   * @brief Constructs a new Dynamic Component Registration Service
   * 
   * @param areEqualChecker function that returns true if the two given variables are equal, or false if not
   */
  DynamicComponentRegistrationService(std::function<bool(T, T)> areEqualChecker);

  /**
   * @brief Registers the dynamic data getter of the component in the service
   * 
   * @param dynamicDataGetter the dynamic data getter of the component
   * @return unregisterFn function that performs component unregistration from the service
   */
  unregisterFn registerDynamicGetter(T dynamicDataGetter);

  /**
   * @brief Unregisters a component with the given dynamic value getter of the component
   * 
   * @param dynamicDataGetter 
   */
  void unregisterGetter(T dynamicDataGetter);

  /**
   * @brief Gets all registered components
   * 
   * @return std::vector<T> the vector which contains all registered components
   */
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