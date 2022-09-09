#pragma once

#include <Arduino.h>
#include <functional>
#include <list>
#include <memory>

#include "HTMLElement/HTMLElement.h"

#include "PageTypes.h"

template <typename T>
class Page : public HTMLElement<T> {
  static_assert(std::is_base_of_v<PageArgs, T>, "Template arg of the Page must derive from PageArgs");

  private:
  String pageTitle;
  String lang;

  protected:
  virtual PageSources compileSrc();

  public:
  /**
   * @brief Constructs a new Page
   *
   * @param title the title of the page which is displayed on the tab
   */
  Page(T* argCollection, String title);

  virtual ~Page();

  /**
   * @brief Recursively compiles all nested elements and returns HTML layout of the page using current params
   *
   * @return String
   */
  virtual void getHTML(ResponseWriter writer);

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
   * @brief Sets the language code of the contents on the page. 'en' by default
   *
   * @param languageCode language code in ISO 639-1 format
   */
  void setPageLanguage(String languageCode);

  /**
   * @brief Returns page contents' language code (that was previously set or 'en' by default)
   *
   * @return String language code in ISO 639-1 format
   */
  String getPageLanguage();
};

// ======================= IMPLEMENTATION =======================

template <typename T>
Page<T>::Page(T* argCollection, String title)
    : HTMLElement<T>(argCollection)
{
  this->setPageTitle(title);
  this->setPageLanguage("en");

  this->getArgCollection()->rootPortal = decltype(PageArgs::rootPortal)(new HTMLElement(this->getArgCollection()));
}

template <typename T>
Page<T>::~Page()
{
  delete this->getArgCollection()->rootPortal;
  this->getArgCollection()->rootPortal = nullptr;
}

template <typename T>
void Page<T>::getHTML(ResponseWriter writer)
{
  PageSources src = this->compileSrc();

  writer(F("<!DOCTYPE html><html lang=\""));
  writer(this->lang);
  writer(F("\"><head><meta charset=\"UTF-8\"/><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"/><title>"));
  writer(this->pageTitle);
  writer(F("</title>"));

  for (auto source : src.styles) {
    if (!source.fileName.isEmpty()) {
      writer(F("<link rel=\"stylesheet\" href=\""));
      writer(source.fileName);
      writer(F(".css\"/>"));
    }
  }
  writer(F("<link rel=\"stylesheet\" href=\"inlineStyles"));
  writer(String(this->getId()));
  writer(F("\">"));
  writer(F("</link>"));

  writer(F("</head><body class=\""));
  writer(this->classList.value());
  writer(F("\">"));

  if (this->getArgCollection()->rootPortal != nullptr)
    this->getArgCollection()->rootPortal->getHTML(writer);

  writer(F("<div class=\"container row\">"));

  for (auto el : this->children) {
    el->getHTML(writer);
  }

  writer(F("</div>"));

  for (auto source : src.scripts) {
    if (!source.fileName.isEmpty()) {
      writer(F("<script src=\""));
      writer(source.fileName);
      writer(F(".js\"></script>"));
    }
  }
  writer(F("<script src=\"inlineScripts"));
  writer(String(this->getId()));
  writer(F("\">"));
  writer(F("</script>"));

  writer(F("</body></html>"));
}

template <typename T>
String Page<T>::getPageTitle()
{
  return this->pageTitle;
}

template <typename T>
void Page<T>::setPageTitle(String title)
{
  title.trim();
  this->pageTitle = title;
}

template <typename T>
PageSources Page<T>::compileSrc()
{
  return PageSources();
}

template <typename T>
void Page<T>::setPageLanguage(String languageCode)
{
  languageCode.trim();
  if (!languageCode.isEmpty())
    this->lang = languageCode;
}

template <typename T>
String Page<T>::getPageLanguage()
{
  return this->lang;
}
