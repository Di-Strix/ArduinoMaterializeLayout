#pragma once

#include <Arduino.h>
#include <list>
#include <map>

#include "Page/PageTypes.h"
#include "WebSourceHandler/WebSourceHandler.h"

enum class SharedStaticType {
  CSS,
  JS
};

struct SrcFile {
  String fileName;
  const uint8_t* file = nullptr;
  size_t fileLength = 0;

  bool operator==(const SrcFile& f) const
  {
    return (fileName == f.fileName && file == f.file && fileLength == f.fileLength);
  }
};

struct Handler {
  String name;
  std::function<void(ResponseWriter writer)> getOnInitFN = [](ResponseWriter writer) {};
  std::function<void(ResponseWriter writer)> getUpdateFN = [](ResponseWriter writer) {};

  bool operator==(const Handler& h) const
  {
    return (name == h.name);
  }
};

struct MaterializeLayoutModule {
  SrcFile CSS;
  SrcFile JS;

  std::function<void(ResponseWriter writer)> getInlineCSS = [](ResponseWriter writer) {};
  std::function<void(ResponseWriter writer)> getInlineJS = [](ResponseWriter writer) {};

  std::list<Handler> handlers;

  bool operator==(const MaterializeLayoutModule& m) const
  {
    return (CSS == m.CSS && JS == m.JS && handlers == m.handlers);
  }
};

struct MLArgs : public PageArgs { };

template <template <typename> class TemplateClass>
using MaterializeLayoutComponent_t = TemplateClass<MLArgs>;

template <template <typename> class TemplateClass>
using MaterializeLayoutComponent = MaterializeLayoutComponent_t<TemplateClass>*;