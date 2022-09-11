#pragma once

#include <Arduino.h>
#include <memory>
#include <vector>

#include "../BaseClass/MaterializeCssBaseClass.h"

template <class T>
class NavHeader : public MaterializeCssBaseClass<T> {
  protected:
  WebSourceHandler* icon = nullptr;

  public:
  NavHeader(T* argCollection);

  virtual ~NavHeader();

  void getHTML(ResponseWriter writer) override;
};

// ======================= IMPLEMENTATION =======================

template <class T>
NavHeader<T>::NavHeader(T* argCollection)
    : MaterializeCssBaseClass<T>(argCollection)
{
  this->classList.add(F("top-nav"));
  this->setStyle(CSSStyleKey::backgroundColor, F("transparent"));
  this->setStyle(CSSStyleKey::color, F("#ee6e73"));
  this->setStyle(CSSStyleKey::boxShadow, F("none"));
  this->setStyle(CSSStyleKey::borderBottom, F("1px solid rgba(0,0,0,0.14)"));
  this->setStyle(CSSStyleKey::display, F("flex"));
  this->setStyle(CSSStyleKey::alignItems, F("center"));
  this->setStyle(CSSStyleKey::marginBottom, F(".5rem"));
  this->setStyle(CSSStyleKey::height, F("auto"));

  this->icon = this->registerSource(F("menu-icon"), MENU_BLACK_24DP_SVG, MENU_BLACK_24DP_SVG_LENGTH, F("image/svg+xml"));
}

template <class T>
NavHeader<T>::~NavHeader()
{
  this->icon->cancelRegistration();
}

template <class T>
void NavHeader<T>::getHTML(ResponseWriter writer)
{
  writer(F("<nav "));
  writer(this->getAttributes());
  writer(F(">"));
  writer(F("<a data-target=\"slide-out\" class=\"sidenav-trigger show-on-large\" style=\"display:flex;align-items:center;\"><img src=\""));
  writer(this->icon->getPath());
  writer(F("\"></a>"));
  writer(F("<div class=\"container\" style=\"display:flex;\"><h1 style=\"margin:0;\""
           "data-MCSS-tabHeader></h1></div></nav>"));
}
