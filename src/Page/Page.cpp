#include "Page.h"

Page::Page(String title) : MaterializeLayoutComponent_t<HTMLElement>(&this->registrationService)
{
  this->setPageTitle(title);
}

String Page::getHTML()
{
  String contents;

  for (auto el : this->children)
  {
    contents += el->getHTML();
  }

  String elemTemplate = F("<!DOCTYPE html><html lang=\"ru\"><head><meta charset=\"UTF-8\"/><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"/><title>");
  elemTemplate += this->pageTitle;
  elemTemplate += F("</title><link rel=\"stylesheet\" href=\"normalize.css\"/><link rel=\"stylesheet\" href=\"materialize.css\"/><link rel=\"stylesheet\" href=\"chartist.css\"/></head><body><div class=\"container row\">");
  elemTemplate += contents;
  elemTemplate += F("</div><script src=\"materialize.js\"></script><script src=\"chartist.js\"></script><script src=\"" APPLICATION_JS_HASH ".js\"></script></body></html>");

  return elemTemplate;
}

String Page::getPageTitle()
{
  return this->pageTitle;
}

void Page::setPageTitle(String title)
{
  title.trim();
  this->pageTitle = title;
}

DCRS_t *Page::getRegistrationService()
{
  return &this->registrationService;
}