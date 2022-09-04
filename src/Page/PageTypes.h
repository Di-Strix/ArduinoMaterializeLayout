#pragma once

#include <Arduino.h>
#include <functional>

#include "HTMLElement/HTMLElement.h"

struct PageSource {
  String fileName;
  String inlineSrc;
};

struct PageSources {
  std::list<PageSource> styles;
  std::list<PageSource> scripts;
};

struct PageArgs : public HTMLElementArgsBase {
  HTMLElement<HTMLElementArgsBase>* rootPortal = nullptr;
};
