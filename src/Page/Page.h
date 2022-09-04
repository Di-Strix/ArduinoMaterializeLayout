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

  virtual ~Page() = default;

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
Page<T>::Page(T *argCollection, String title)
    : HTMLElement<T>(argCollection)
{
  this->setPageTitle(title);
  this->setPageLanguage("en");
}

template <typename T>
String Page<T>::getHTML()
{
  String contents;

  PageSources src = this->compileSrc();

  for (auto el : this->children) {
    contents += el->getHTML();
  }

  String elemTemplate = F("<!DOCTYPE html><html lang=\"");
  elemTemplate += this->lang;
  elemTemplate += F("\"><head><meta charset=\"UTF-8\"/><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"/><title>");
  elemTemplate += this->pageTitle;
  elemTemplate += F("</title>");

  for (auto source : src.styles) {
    if (!source.fileName.isEmpty()) {
      elemTemplate += F("<link rel=\"stylesheet\" href=\"");
      elemTemplate += source.fileName;
      elemTemplate += F(".css\"/>");
    }
    if (!source.inlineSrc.isEmpty()) {
      elemTemplate += F("<style>");
      elemTemplate += source.inlineSrc;
      elemTemplate += F("</style>");
    }
  }

  elemTemplate += F("</head><body class=\"");
  elemTemplate += this->classList.value();
  elemTemplate += F("\"><div class=\"container row\">");
  elemTemplate += contents;
  elemTemplate += F("</div>");

  for (auto source : src.scripts) {
    if (!source.fileName.isEmpty()) {
      elemTemplate += F("<script src=\"");
      elemTemplate += source.fileName;
      elemTemplate += F(".js\"></script>");
    }
    if (!source.inlineSrc.isEmpty()) {
      elemTemplate += F("<script>");
      elemTemplate += source.inlineSrc;
      elemTemplate += F("</script>");
    }
  }

  elemTemplate += F("</body></html>");

  return elemTemplate;
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
