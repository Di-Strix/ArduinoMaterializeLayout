#pragma once

#include <Arduino.h>
#include <memory>
#include <vector>

#include "../BaseClass/MaterializeCssBaseClass.h"
#include "../Tab/Tab.h"

template <class T>
class NavContents : public MaterializeCssBaseClass<T> {
  public:
  NavContents(T* argCollection);

  virtual ~NavContents() = default;

  void getHTML(ResponseWriter writer);
};

// ======================= IMPLEMENTATION =======================

template <class T>
NavContents<T>::NavContents(T* argCollection)
    : MaterializeCssBaseClass<T>(argCollection)
{
  this->setAttribute(F("id"), F("slide-out"));
  this->classList.add(F("sidenav"));
  this->classList.add(F("sidenav-fixed"));
}

template <class T>
void NavContents<T>::getHTML(ResponseWriter writer)
{
  writer(F("<ul "));
  writer(this->getAttributes());
  writer(F(">"));

  // clang-format off
  writer(F("<script>"));
  writer(F("(() => {"));
  writer( F("const el = document.createElement('style');"));
  writer( F("el.innerHTML = 'body > .root {padding-left: 300px;}@media only screen and (max-width : 992px) {body > .root {padding-left: 0;}}';"));
  writer( F("document.head.insertAdjacentElement('afterbegin', el);"));
  writer( F("})()"));
  writer(F("</script>"));
  // clang-format on

  for (auto ch : this->children) {
    auto t = static_cast<Tab<T>*>(ch);

    String tabRef = String(t->getId());

    writer(F("<li><a href=\"#"));
    writer(tabRef);
    writer(F("\" class=\"waves-effect sidenav-close\" data-MCSS-virtual-router-tab-name=\""));
    writer(t->getTabName());
    writer(F("\">"));
    writer(t->getTabName());
    writer(F("</a></li>"));
  }

  writer(F("</ul>"));
}
