#pragma once

#include <Arduino.h>
#include <list>
#include <memory>

#include "../BaseClass/MaterializeCssBaseClass.h"

#include "../Tab/Tab.h"

template <class T>
class TabGroup : public MaterializeCssBaseClass<T> {
  public:
  TabGroup(T* argCollection);

  virtual ~TabGroup() = default;

  void getHTML(ResponseWriter writer);
};

// ======================= IMPLEMENTATION =======================

template <class T>
TabGroup<T>::TabGroup(T* argCollection)
    : MaterializeCssBaseClass<T>(argCollection)
{
  this->classList.add("tabs");
  this->classList.add("tabs-fixed-width");
  this->classList.add("z-depth-1");
}

template <class T>
void TabGroup<T>::getHTML(ResponseWriter writer)
{
  writer(F("<div class=\"row\"><ul class=\""));
  writer(this->classList.value());
  writer(F("\" "));
  writer(F("style=\""));
  writer(this->getInlineStyles());
  writer(F("\">"));

  for (auto ch : this->children) {
    auto t = static_cast<Tab<T>*>(ch);
    String tabRef = String(t->getId());

    writer(F("<li class=\"tab col\"><a href=\"#"));
    writer(tabRef);
    writer(F("\">"));
    writer(t->getTabName());
    writer(F("</a></li>"));
  }

  writer(F("</ul></div>"));

  for (auto ch : this->children) {
    auto t = static_cast<Tab<T>*>(ch);
    String tabRef = String(t->getId());

    writer(F("<div id=\""));
    writer(tabRef);
    writer(F("\" class=\"col s12\">"));

    t->getHTML(writer);

    writer(F("</div>"));
  }
}
