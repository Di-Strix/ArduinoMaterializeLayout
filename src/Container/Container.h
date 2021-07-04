#ifndef _CONTAINER_H_
#define _CONTAINER_H_

#include <Arduino.h>
#include <variant>
#include <vector>
#include <memory>

#include "../DynamicComponentRegistrationService/DynamicComponentRegistrationService.h"
#include "../HTMLElement/HTMLElement.h"
#include "../Field/Field.h"
#include "../Button/Button.h"
#include "../Text/Text.h"

template <typename A>
using availableContainerTypes = std::variant<
    std::shared_ptr<Field>,
    std::shared_ptr<Button>,
    std::shared_ptr<StaticText>,
    std::shared_ptr<DynamicText<A>>>;

template <typename B>
class Container : public HTMLElement
{
  friend class Page;
  template <typename C>
  friend class TabGroup;
  template <typename D>
  friend class Tab;

private:
  std::vector<availableContainerTypes<B>> container;
  std::shared_ptr<DynamicComponentRegistrationService<B>> registrationService;

protected:
  Container(std::shared_ptr<DynamicComponentRegistrationService<B>> registrationService);
  std::shared_ptr<DynamicComponentRegistrationService<B>> getDynamicComponentRegistrationServerPtr();

  virtual String getHTML();
  String compileComponents();
  bool emitOnContainer(size_t id, String value);

public:
  std::shared_ptr<Field> createField(String name, String defaultValue);
  std::shared_ptr<Button> createButton(String caption, ButtonCallback callback);
  std::shared_ptr<StaticText> createStaticText(String text, TextType textType = TextType::p);
  template <typename E>
  std::shared_ptr<DynamicText<B>> createDynamicText(E text, TextType textType = TextType::p);
};

// ======================= IMPLEMENTATION =======================

template <typename T>
Container<T>::Container(std::shared_ptr<DynamicComponentRegistrationService<T>> registrationService)
{
  this->registrationService = registrationService;
}

template <typename T>
std::shared_ptr<DynamicComponentRegistrationService<T>> Container<T>::getDynamicComponentRegistrationServerPtr()
{
  return this->registrationService;
}

template <typename T>
String Container<T>::getHTML()
{
  return this->compileComponents();
}

template <typename T>
String Container<T>::compileComponents()
{
  String compiledComponents = "";

  for (auto v : this->container)
  {
    if (auto el = std::get_if<std::shared_ptr<Field>>(&v))
    {
      if (*el)
      {
        compiledComponents += el->get()->getHTML();
      }
    }
    if (auto el = std::get_if<std::shared_ptr<Button>>(&v))
    {
      if (*el)
      {
        compiledComponents += el->get()->getHTML();
      }
    }
    if (auto el = std::get_if<std::shared_ptr<StaticText>>(&v))
    {
      if (*el)
      {
        compiledComponents += el->get()->getHTML();
      }
    }
    if (auto el = std::get_if<std::shared_ptr<DynamicText<T>>>(&v))
    {
      if (*el)
      {
        compiledComponents += el->get()->getHTML();
      }
    }
  }

  return compiledComponents;
}

template <typename T>
std::shared_ptr<Field> Container<T>::createField(String name, String defaultValue)
{
  auto f = std::make_shared<Field>(name, defaultValue);
  this->container.push_back(f);

  return f;
}

template <typename T>
std::shared_ptr<Button> Container<T>::createButton(String caption, ButtonCallback callback)
{
  auto b = std::make_shared<Button>(caption, callback);
  this->container.push_back(b);

  return b;
}

template <typename T>
std::shared_ptr<StaticText> Container<T>::createStaticText(String text, TextType textType)
{
  auto st = std::make_shared<StaticText>(text, textType);
  this->container.push_back(st);

  return st;
}

template <typename T>
template <typename E>
std::shared_ptr<DynamicText<T>> Container<T>::createDynamicText(E text, TextType textType)
{
  auto dt = std::make_shared<DynamicText<T>>(this->registrationService, text, textType);
  this->container.push_back(dt);

  return dt;
}

template <typename T>
bool Container<T>::emitOnContainer(size_t id, String value)
{
  bool found = false;

  for (auto v : this->container)
  {
    if (auto el = std::get_if<std::shared_ptr<Field>>(&v))
    {
      if (*el)
      {
        found = el->get()->emit(id, value);
      }
    }
    else if (auto el = std::get_if<std::shared_ptr<Button>>(&v))
    {
      if (*el)
      {
        found = el->get()->emit(id, value);
      }
    }
    if (found)
      break;
  }

  return found;
}

#endif //_CONTAINER_H_