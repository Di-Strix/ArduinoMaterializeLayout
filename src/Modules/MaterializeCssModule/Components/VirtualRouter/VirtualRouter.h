#pragma once

#include <Arduino.h>
#include <list>
#include <memory>

#include "../../Static/menu_black_24dp.svg.h"
#include "../Tab/Tab.h"
#include "../TabGroup/TabGroup.h"
#include "./NavContents.h"
#include "./NavHeader.h"

template <class T>
class VirtualRouter : public TabGroup<T> {
  protected:
  NavContents<T> navContents;
  NavHeader<T> navHeader;

  public:
  VirtualRouter(T* argCollection);

  virtual ~VirtualRouter();

  void getHTML(ResponseWriter writer) override;
};

// ======================= IMPLEMENTATION =======================

template <class T>
VirtualRouter<T>::VirtualRouter(T* argCollection)
    : TabGroup<T>(argCollection)
    , navContents(argCollection)
    , navHeader(argCollection)
{
  this->classList.add(F("tabs"));
  this->setAttribute(F("data-MCSS-virtual-router"), "");

  auto rootPortal = this->getArgCollection()->rootPortal;
  rootPortal->appendChild((decltype(rootPortal))&this->navContents);
  rootPortal->appendChild((decltype(rootPortal))&this->navHeader);

  this->onChildChange.subscribe([this](ChildChangeType changeType, HTMLElement<T>* el) {
    switch (changeType) {
    case ChildChangeType::add:
      this->navContents.appendChild(el);
      break;
    case ChildChangeType::remove:
      this->navContents.removeChild(el);
      break;
    }
  });
}

template <class T>
VirtualRouter<T>::~VirtualRouter()
{
  auto rootPortal = this->getArgCollection()->rootPortal;
  rootPortal->removeChild((decltype(rootPortal))&this->navContents);
  rootPortal->removeChild((decltype(rootPortal))&this->navHeader);
}

template <class T>
void VirtualRouter<T>::getHTML(ResponseWriter writer)
{
  writer(F("<div class=\"row\" style=\"display:none\"><ul "));
  writer(this->getAttributes());
  writer(F(">"));

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
    ch->setStyle(CSSStyleKey::display, F("none"));
    ch->getHTML(writer);
  }
}
