#ifndef _MATERIALIZE_LAYOUT_COMPONENT_FACTORY_H_
#define _MATERIALIZE_LAYOUT_COMPONENT_FACTORY_H_

#include "../HTMLElement/HTMLElement.h"
#include "../Page/Page.h"
#include <Arduino.h>
#include <map>

class AbstractComponentCreator {
  public:
  virtual MaterializeLayoutComponent<HTMLElement> create(DCRS_t*) const = 0;
};

template <class C>
class ComponentCreator : public AbstractComponentCreator {
  public:
  MaterializeLayoutComponent<HTMLElement> create(DCRS_t* s) const { return new C(s); }
};

#endif //_MATERIALIZE_LAYOUT_COMPONENT_FACTORY_H_