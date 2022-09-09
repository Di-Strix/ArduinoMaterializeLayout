#pragma once

#include <Arduino.h>
#include <functional>

#include "HTMLElement/HTMLElement.h"

struct PageSource {
  String fileName;
  std::function<void(ResponseWriter writer)> getInlineSrc = [](ResponseWriter writer) {};
};

struct PageSources {
  std::list<PageSource> styles;
  std::list<PageSource> scripts;
};

struct PageArgs : public HTMLElementArgsBase {
  /**
   * @brief Used to inject elements to the root of the page
   *
   */
  HTMLElement<HTMLElementArgsBase>* rootPortal = nullptr;
};
