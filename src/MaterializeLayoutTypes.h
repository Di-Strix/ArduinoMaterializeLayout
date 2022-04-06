#pragma once

#include <Arduino.h>
#include <map>

#include "ComponentFactory/ComponentFactory.h"

enum class SharedStaticType {
  CSS,
  JS
};

struct SrcFile {
  String fileName;
  const uint8_t* file;
  size_t fileLength;
};

struct Handler {
  String name;
  String onInitFN;
  String updateFN;
};

struct MaterializeLayoutModule {
  SrcFile CSS;
  SrcFile JS;

  String inlineCSS;
  String inlineJS;

  std::map<int, AbstractComponentCreator*> declarations;

  std::list<Handler> handlers;
};
