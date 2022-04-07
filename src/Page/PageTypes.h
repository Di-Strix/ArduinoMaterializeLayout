#pragma once

#include <functional>

struct PageSource {
  String fileName;
  String inlineSrc;
};

struct PageSources {
  std::list<PageSource> styles;
  std::list<PageSource> scripts;
};
