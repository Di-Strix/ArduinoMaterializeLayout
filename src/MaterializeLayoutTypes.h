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

struct HTMLElementArgs {
  struct {
    std::function<void(String handlerId, size_t elementId, String value)> dispatcher;
    uint32_t throttleTime;
  } dispatch;
  std::function<WebSourceHandler*(String path, const uint8_t* content, size_t contentLength, String contentType)> registerSource;
};

template <template <typename> class TemplateClass>
using MaterializeLayoutComponent_t = TemplateClass<HTMLElementArgs*>;

template <template <typename> class TemplateClass>
using MaterializeLayoutComponent = MaterializeLayoutComponent_t<TemplateClass>*;