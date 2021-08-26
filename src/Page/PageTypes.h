#ifndef _MATERIALIZE_LAYOUT_PAGE_TYPES_H_
#define _MATERIALIZE_LAYOUT_PAGE_TYPES_H_

#include <functional>

#include "../DynamicComponentRegistrationService/DynamicComponentRegistrationService.h"

struct PageSource {
  String fileName;
  String inlineSrc;
};

struct PageSources {
  std::list<PageSource> styles;
  std::list<PageSource> scripts;
};

struct UpdateMsg {
  String handlerId;
  String value;
};

struct dynamicValueGetter {
  size_t id;
  std::function<UpdateMsg()> getter;
};

template <template <typename> class TemplateClass>
using MaterializeLayoutComponent_t = TemplateClass<dynamicValueGetter>;

template <template <typename> class TemplateClass>
using MaterializeLayoutComponent = MaterializeLayoutComponent_t<TemplateClass>*;

using DCRS_t = MaterializeLayoutComponent_t<DynamicComponentRegistrationService>;

#endif //_MATERIALIZE_LAYOUT_PAGE_TYPES_H_