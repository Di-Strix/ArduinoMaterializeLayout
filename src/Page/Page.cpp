#include "Page.h"

Page::Page(String title, std::function<PageSources()> srcGetter)
    : MaterializeLayoutComponent_t<HTMLElement>(&this->registrationService)
{
  this->setPageTitle(title);
  this->srcGetter = srcGetter;
}

String Page::getHTML()
{
  String contents;

  PageSources src = this->srcGetter();

  for (auto el : this->children) {
    contents += el->getHTML();
  }

  String elemTemplate = F("<!DOCTYPE html><html lang=\"ru\"><head><meta charset=\"UTF-8\"/><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"/><title>");
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

  elemTemplate += F("</head><body><div class=\"container row\">");
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

String Page::getPageTitle()
{
  return this->pageTitle;
}

void Page::setPageTitle(String title)
{
  title.trim();
  this->pageTitle = title;
}

DCRS_t* Page::getRegistrationService()
{
  return &this->registrationService;
}