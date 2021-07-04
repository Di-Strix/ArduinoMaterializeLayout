#ifndef _MATERIALIZE_LAYOUT_PAGE_H_
#define _MATERIALIZE_LAYOUT_PAGE_H_

#include <Arduino.h>
#include <vector>
#include <variant>
#include <memory>
#include <functional>

#include "../DynamicComponentRegistrationService/DynamicComponentRegistrationService.h"
#include "../HTMLElement/HTMLElement.h"
#include "../Layout/Layout.h"
#include "../TabGroup/TabGroup.h"

#include "../SharedStatic.h"

struct dynamicValueGetter
{
  size_t id;
  std::function<String()> getter;
};
using DynamicComponentRegistrationServiceT = DynamicComponentRegistrationService<dynamicValueGetter>;
typedef std::vector<std::variant<std::shared_ptr<TabGroup<dynamicValueGetter>>, std::shared_ptr<Layout<dynamicValueGetter>>>> pageLayoutType;
typedef std::shared_ptr<DynamicComponentRegistrationServiceT> DynamicComponentRegistrationServiceSharedPtr;

class Page : private HTMLElement
{
private:
  String pageTitle;
  pageLayoutType page;
  DynamicComponentRegistrationServiceSharedPtr registrationService;

protected:
  DynamicComponentRegistrationServiceSharedPtr getRegistrationService();

public:
  Page(String title);

  virtual String getHTML();
  virtual bool emit(size_t id, String value);

  String getPageTitle();
  void setPageTitle(String title);

  std::shared_ptr<TabGroup<dynamicValueGetter>> createTabGroup();
  std::shared_ptr<Layout<dynamicValueGetter>> createLayout();
};

#endif //_MATERIALIZE_LAYOUT_PAGE_H_