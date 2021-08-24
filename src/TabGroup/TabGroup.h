#ifndef _MATERIALIZE_LAYOUT_TAB_GROUP_H_
#define _MATERIALIZE_LAYOUT_TAB_GROUP_H_

#include <Arduino.h>
#include <list>
#include <memory>

#include "../DynamicComponentRegistrationService/DynamicComponentRegistrationService.h"
#include "../HTMLElement/HTMLElement.h"
#include "../Tab/Tab.h"

template <class T>
class TabGroup : public HTMLElement<T>
{
  using HTMLElement<T>::HTMLElement;

public:
  String getHTML();
};

// ======================= IMPLEMENTATION =======================

template <class T>
String TabGroup<T>::getHTML()
{

  String contentsList, contents;
  for (auto ch : this->children)
  {
    auto t = static_cast<Tab<T> *>(ch);

    String tabRef = String(t->getId());

    contentsList += F("<li class=\"tab col s3\"><a href=\"#");
    contentsList += tabRef;
    contentsList += F("\">");
    contentsList += t->getTabName();
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

#endif //_MATERIALIZE_LAYOUT_TAB_GROUP_H_