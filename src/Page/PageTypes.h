#ifndef _MATERIALIZE_LAYOUT_PAGE_TYPES_H_
#define _MATERIALIZE_LAYOUT_PAGE_TYPES_H_

#include <functional>

#include "../DynamicComponentRegistrationService/DynamicComponentRegistrationService.h"

struct PageSources
{
  std::list<String> stylesFileNames;
  std::list<String> scriptFileNames;
};

struct dynamicValueGetter
{
  size_t id;
  std::function<String()> getter;
};

template <template <typename> class TemplateClass>
using MaterializeLayoutComponent_t = TemplateClass<dynamicValueGetter>;

template <template <typename> class TemplateClass>
using MaterializeLayoutComponent = MaterializeLayoutComponent_t<TemplateClass> *;

using DCRS_t = MaterializeLayoutComponent_t<DynamicComponentRegistrationService>;

#endif //_MATERIALIZE_LAYOUT_PAGE_TYPES_H_