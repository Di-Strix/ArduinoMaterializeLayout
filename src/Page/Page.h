#pragma once

#include <Arduino.h>
#include <functional>
#include <list>
#include <memory>

#include "../DynamicComponentRegistrationService/DynamicComponentRegistrationService.h"
#include "../HTMLElement/HTMLElement.h"
#include "PageTypes.h"

class Page : public HTMLElement<dynamicValueGetter> {
  private:
  String pageTitle;
  DCRS_t registrationService = DCRS_t([](dynamicValueGetter f, dynamicValueGetter s) { return f.id == s.id; });
  std::function<PageSources()> srcGetter;

  protected:
  DCRS_t* getRegistrationService();

  public:
  using MaterializeLayoutComponent_t<HTMLElement>::emit;

  /**
   * @brief Constructs a new Page
   * 
   * @param title the title of the page which is displayed on the tab
   */
  Page(
      String title, std::function<PageSources()> srcGetter = []() -> PageSources { return {}; });

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
