#ifndef _MATERIALIZE_LAYOUT_TAB_GROUP_H_
#define _MATERIALIZE_LAYOUT_TAB_GROUP_H_

#include <Arduino.h>
#include <vector>
#include <memory>

#include "../DynamicComponentRegistrationService/DynamicComponentRegistrationService.h"
#include "../HTMLElement/HTMLElement.h"
#include "../Tab/Tab.h"

template <class T>
class TabGroup : public HTMLElement
{
  friend class Page;

private:
  std::vector<std::shared_ptr<Tab<T>>> tabs;
  std::shared_ptr<DynamicComponentRegistrationService<T>> registrationService;

protected:
  virtual String getHTML();
  virtual bool emit(size_t id, String value);

public:
  TabGroup(std::shared_ptr<DynamicComponentRegistrationService<T>> registrationService);

  std::shared_ptr<Tab<T>> createTab(String tabName);
};

// ======================= IMPLEMENTATION =======================

template <class T>
TabGroup<T>::TabGroup(std::shared_ptr<DynamicComponentRegistrationService<T>> registrationService) : HTMLElement()
{
  this->registrationService = registrationService;
}

template <class T>
String TabGroup<T>::getHTML()
{

  String contentsList, contents;
  for (auto t : this->tabs)
  {
    if (!t)
      continue;

    String tabRef = String(t->getId());
    tabRef.replace(" ", "");
    tabRef.replace("#", "");
    tabRef.replace("%", "");

    contentsList += F("<li class=\"tab col s3\"><a href=\"#");
    contentsList += tabRef;
    contentsList += F("\">");
    contentsList += t->getName();
    contentsList += F("</a></li>");

    contents += F("<div id=\"");
    contents += tabRef;
    contents += F("\" class=\"col s12\">");
    contents += t->getHTML();
    contents += F("</div>");
  }

  String elemTemplate = F("<div class=\"row\"><ul class=\"tabs\">");
  elemTemplate += contentsList;
  elemTemplate += F("</ul></div>");
  elemTemplate += contents;

  return elemTemplate;
}

template <class T>
std::shared_ptr<Tab<T>> TabGroup<T>::createTab(String tabName)
{
  auto t = std::make_shared<Tab<T>>(this->registrationService, tabName);
  this->tabs.push_back(t);
  return t;
}

template <class T>
bool TabGroup<T>::emit(size_t id, String value)
{
  bool found = false;
  for (auto t : this->tabs)
  {
    if (!t)
      continue;
    found = t->emit(id, value);

    if (found)
      break;
  }

  return found;
}

#endif //_MATERIALIZE_LAYOUT_TAB_GROUP_H_