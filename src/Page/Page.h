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

typedef std::shared_ptr<Button> Button_t;
typedef std::shared_ptr<Field> Field_t;
typedef std::shared_ptr<StaticText> StaticText_t;
typedef std::shared_ptr<DynamicText<dynamicValueGetter>> DynamicText_t;
typedef std::shared_ptr<Layout<dynamicValueGetter>> Layout_t;
typedef std::shared_ptr<Tab<dynamicValueGetter>> Tab_t;
typedef std::shared_ptr<TabGroup<dynamicValueGetter>> TabGroup_t;

class Page : private HTMLElement
{
private:
  String pageTitle;
  pageLayoutType page;
  DynamicComponentRegistrationServiceSharedPtr registrationService;

protected:
  DynamicComponentRegistrationServiceSharedPtr getRegistrationService();

public:
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
   * @brief Recursively emits an event on  all nested elements
   * 
   * @param id id of the element on which event needs to be emitted
   * @param value data to be provided to the element
   * @return true if an element with provided id is found
   * @return false if an element with provided id isn't found
   */
  virtual bool emit(size_t id, String value);

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

  /**
   * @brief Creates a Tab Group on the page
   * 
   * @return TabGroup_t
   */
  TabGroup_t createTabGroup();

  /**
   * @brief Creates a Layout on the page
   * 
   * @return Layout_t
   */
  Layout_t createLayout();
};

#endif //_MATERIALIZE_LAYOUT_PAGE_H_