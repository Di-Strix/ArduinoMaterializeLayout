#ifndef _MATERIALIZE_LAYOUT_PAGE_H_
#define _MATERIALIZE_LAYOUT_PAGE_H_

#include <Arduino.h>
#include <list>
#include <memory>
#include <functional>

#include "PageTypes.h"
#include "../DynamicComponentRegistrationService/DynamicComponentRegistrationService.h"
#include "../HTMLElement/HTMLElement.h"
#include "../Layout/Layout.h"
#include "../TabGroup/TabGroup.h"

#include "../SharedStatic.h"

class Page : private HTMLElement<dynamicValueGetter>
{
private:
  String pageTitle;
  DCRS_t registrationService = DCRS_t([](dynamicValueGetter f, dynamicValueGetter s)

protected:
  DCRS_t *getRegistrationService();

public:
  using MaterializeLayoutComponent_t<HTMLElement>::emit;

  /**
   * @brief Constructs a new Page
   * 
   * @param title the title of the page which is displayed on the tab
   */
  Page(String title);

  /**
   * @brief Recursively compiles all nested elements and returns HTML layout of the page using current params
   * 
   * @return String 
   */
  virtual String getHTML();

  /**
   * @brief Gets the page title
   * 
   * @return String 
   */
  String getPageTitle();

  /**
   * @brief Sets the page title
   * 
   * @param title the new title
   */
  void setPageTitle(String title);
};

#endif //_MATERIALIZE_LAYOUT_PAGE_H_