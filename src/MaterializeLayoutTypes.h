#pragma once

#include <Arduino.h>
#include <list>
#include <map>

#include "Page/PageTypes.h"

enum class SharedStaticType {
  CSS,
  JS
};

struct SrcFile {
  String fileName;
  const uint8_t* file;
  size_t fileLength;

  bool operator==(const SrcFile& f) const
  {
    return (fileName == f.fileName && file == f.file && fileLength == f.fileLength);
  }
};

struct Handler {
  String name;
  String onInitFN;
  String updateFN;

  bool operator==(const Handler& h) const
  {
    return (name == h.name && onInitFN == h.onInitFN && updateFN == h.updateFN);
  }
};

struct MaterializeLayoutModule {
  SrcFile CSS;
  SrcFile JS;

  String inlineCSS;
  String inlineJS;

  std::list<Handler> handlers;

  bool operator==(const MaterializeLayoutModule& m) const
  {
    return (CSS == m.CSS && JS == m.JS && inlineCSS == m.inlineCSS && inlineJS == m.inlineJS && handlers == m.handlers);
  }
};
