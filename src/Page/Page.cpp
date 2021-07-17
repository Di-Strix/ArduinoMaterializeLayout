#include "Page.h"

Page::Page(String title)
{
  this->setPageTitle(title);
  this->registrationService = std::make_shared<DynamicComponentRegistrationServiceT>([](dynamicValueGetter f, dynamicValueGetter s) -> bool
                                                                                     { return f.id == s.id; });
}

String Page::getHTML()
{
  // String elemTemplate = F("<!DOCTYPE html><html lang=\"ru\"><head><meta charset=\"UTF-8\"/><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"/><title>${PAGE_TITLE}</title><link rel=\"stylesheet\" href=\"normalize.css\"/><link rel=\"stylesheet\" href=\"materialize.css\"/></head><body><div class=\"container\">${CONTENTS}</div><script src=\"materialize.js\"></script><script src=\"" APPLICATION_JS_HASH ".js\"></script></body></html>");
  String contents;

  for (auto v : this->page)
  {
    if (auto t = std::get_if<std::shared_ptr<TabGroup<dynamicValueGetter>>>(&v))
    {
      if (*t)
      {
        contents += t->get()->getHTML();
      }
    }
    else if (auto l = std::get_if<std::shared_ptr<Layout<dynamicValueGetter>>>(&v))
    {
      if (*l)
      {
        contents += l->get()->getHTML();
      }
    }
  }

  String elemTemplate = F("<!DOCTYPE html><html lang=\"ru\"><head><meta charset=\"UTF-8\"/><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"/><title>");
  elemTemplate += this->pageTitle;
  elemTemplate += F("</title><link rel=\"stylesheet\" href=\"normalize.css\"/><link rel=\"stylesheet\" href=\"materialize.css\"/></head><body><div class=\"container row\">");
  elemTemplate += contents;
  elemTemplate += F("</div><script src=\"materialize.js\"></script><script src=\"" APPLICATION_JS_HASH ".js\"></script></body></html>");

  return elemTemplate;
}

std::shared_ptr<TabGroup<dynamicValueGetter>> Page::createTabGroup()
{
  auto tg = std::make_shared<TabGroup<dynamicValueGetter>>(this->registrationService);
  this->page.push_back(tg);

  return tg;
}

std::shared_ptr<Layout<dynamicValueGetter>> Page::createLayout()
{
  auto l = std::make_shared<Layout<dynamicValueGetter>>(this->registrationService);
  this->page.push_back(l);

  return l;
}

bool Page::emit(size_t id, String value)
{
  bool found = false;

  for (auto v : this->page)
  {
    if (auto el = std::get_if<std::shared_ptr<TabGroup<dynamicValueGetter>>>(&v))
    {
      if (*el)
      {
        found = el->get()->emit(id, value);
      }
    }
    else if (auto el = std::get_if<std::shared_ptr<Layout<dynamicValueGetter>>>(&v))
    {
      if (*el)
      {

        found = el->get()->emit(id, value);
      }
    }
    if (found)
      break;
  }

  return found;
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

DynamicComponentRegistrationServiceSharedPtr Page::getRegistrationService()
{
  return this->registrationService;
}