#pragma once

#include <Arduino.h>
#include <list>
#include <memory>

#include "HTMLElement/HTMLElement.h"

#include "../Tab/Tab.h"

template <class T>
class TabGroup : public HTMLElement<T> {
  public:
  TabGroup(T* argCollection);

  virtual ~TabGroup() = default;

  String getHTML();
};

// ======================= IMPLEMENTATION =======================

template <class T>
TabGroup<T>::TabGroup(T* argCollection)
    : HTMLElement<T>(argCollection)
{
  this->classList.add("tabs");
  this->classList.add("tabs-fixed-width");
  this->classList.add("z-depth-1");
}

template <class T>
String TabGroup<T>::getHTML()
{

  String contentsList, contents;
  for (auto ch : this->children) {
    auto t = static_cast<Tab<T>*>(ch);

    String tabRef = String(t->getId());

    contentsList += F("<li class=\"tab col\"><a href=\"#");
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

  String elemTemplate = F("<div class=\"row\"><ul class=\"");
  elemTemplate += this->classList.value();
  elemTemplate += F("\" ");
  elemTemplate += F("style=\"");
  elemTemplate += this->getInlineStyles();
  elemTemplate += F("\">");
  elemTemplate += contentsList;
  elemTemplate += F("</ul></div>");
  elemTemplate += contents;

  return elemTemplate;
}
