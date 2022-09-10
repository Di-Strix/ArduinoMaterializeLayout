#include "MaterializeLayout.h"

MaterializeLayout::MaterializeLayout(String pageTitle, String baseURL)
    : Page(new MLArgs(), pageTitle)
{
  if (!baseURL.startsWith("/"))
    baseURL = "/" + baseURL;
  this->baseURL = baseURL;

  this->injectModule(getNormalizeCssModule());
  this->injectModule(getMaterializeCssModule());
  this->injectModule(getMainAppModule());

  this->ws.onEvent([this](AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len) {
    if (type == WS_EVT_DATA) {
      AwsFrameInfo* info = (AwsFrameInfo*)arg;
      if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
        String json;
        for (size_t i = 0; i < len; i++)
          json += (char)data[i];

        auto doc = dynamiclyDeserializeJson(json);
        json.clear();

        this->emit(doc["id"].as<size_t>(), doc["value"].as<String>());
      }
    }
  });

  this->getArgCollection()->dispatch.dispatcher = [this](String handlerId, size_t id, String value) {
    DynamicJsonDocument doc(JSON_OBJECT_SIZE(3) + JSON_STRING_SIZE(value.length()) + JSON_STRING_SIZE(handlerId.length()));
    doc["handlerId"] = handlerId;
    doc["id"] = id;
    doc["value"] = value;

    String json;
    serializeJson(doc, json);

    if (json == F("null"))
      json = F("{}");

    this->bakedEvents.push_back(json);
    this->triggerDispatch();
  };
  this->getArgCollection()->dispatch.throttleTime = 0;

  this->getArgCollection()->registerSource = [this](String path, const uint8_t* content, size_t contentLength, String contentType) -> WebSourceHandler* {
    auto srcHandler = new WebSourceHandler(path, content, contentLength, contentType);
    this->registeredSources.push_back(srcHandler);

    this->registerInEspAsyncWebServer(this->server);

    return srcHandler;
  };
}

MaterializeLayout::~MaterializeLayout()
{
  this->unregisterHandlers();

  for (auto srcHandler : this->registeredSources) {
    delete srcHandler;
  }

  this->registeredSources.clear();

  delete this->getArgCollection();
}

PageSources MaterializeLayout::compileSrc()
{
  PageSources src;
  String moduleHandlers;

  for (auto moduleInfo : this->modules) {
    if ((moduleInfo.CSS.fileName && moduleInfo.CSS.file) || moduleInfo.getInlineCSS) {
      src.styles.push_front({ moduleInfo.CSS.fileName, moduleInfo.getInlineCSS });
    }
    if ((moduleInfo.JS.fileName && moduleInfo.JS.file) || moduleInfo.getInlineJS) {
      src.scripts.push_front({ moduleInfo.JS.fileName, moduleInfo.getInlineJS });
    }

    for (auto handler : moduleInfo.handlers) {
      src.scripts.push_back({ "", [handler](ResponseWriter writer) {
                               writer(F("class "));
                               writer(handler.name);
                               writer(F(" extends Handler {"
                                        "onInit() {"));

                               handler.getOnInitFN(writer);

                               writer(F("}"
                                        "update(el, value) {"));

                               handler.getUpdateFN(writer);

                               writer(F("}};"));
                               writer(F("dynamicUpdateService.addHandler('"));
                               writer(handler.name);
                               writer(F("', new "));
                               writer(handler.name);
                               writer(F("());"));
                             } });
    }
  }

  src.scripts.push_back({ "", [this](ResponseWriter writer) {
                           writer(F("dynamicUpdateService.init(\""));
                           writer(String(this->ws.url()).substring(1));
                           writer(F("\");"));
                         } });

  return src;
}

bool MaterializeLayout::injectModule(const MaterializeLayoutModule& moduleInfo)
{
  auto it = std::find(this->modules.begin(), this->modules.end(), moduleInfo);
  if (it != this->modules.end())
    return false;

  this->modules.push_back(moduleInfo);

  return true;
}

void MaterializeLayout::unregisterHandlers()
{
  if (!this->server || this->handlers.size() <= 0)
    return;

  for (auto handler : this->handlers) {
    this->server->removeHandler(&handler);
  }

  this->handlers.clear();
}

void MaterializeLayout::registerInEspAsyncWebServer(AsyncWebServer* s)
{
  if (!s)
    return;

  this->unregisterHandlers();

  this->server = s;

  this->handlers.push_back(this->server->on(this->baseURL.c_str(), HTTP_GET, [=](AsyncWebServerRequest* request) {
    AsyncWebServerResponse* res = nullptr;

    if (LittleFS.begin()) {
      auto file = LittleFS.open(F("/__MLTemp/tmp.html"), "w+");
        this->getHTML([&file](String data) {
        Serial.println(ESP.getMaxFreeBlockSize());
        file.print(data);
      });
      file.close();
      res = request->beginResponse(LittleFS, F("/__MLTemp/tmp.html"), F("text/html;charset=utf-8"));
    } else {
      auto resStream = request->beginResponseStream(F("text/html;charset=utf-8"));
      res = resStream;
      this->getHTML([&resStream](String data) {
        resStream->print(data);
      });
    }

    res->setCode(200);
    res->addHeader(F("Cache-Control"), F("no-cache"));
    res->addHeader(F("X-Content-Type-Options"), F("nosniff"));
    request->send(res);
  }));

  auto inlineStylesUrl = String(F("/inlineStyles")) + String(this->getId()) + String(F(".css"));
  auto inlineScriptsUrl = String(F("/inlineScripts")) + String(this->getId()) + String(F(".js"));

  this->handlers.push_back(this->server->on(inlineStylesUrl.c_str(), HTTP_GET, [=](AsyncWebServerRequest* request) {
    auto sources = this->compileSrc();
    AsyncWebServerResponse* res = nullptr;

    if (LittleFS.begin()) {
      auto file = LittleFS.open(F("/__MLTemp/tmp.css"), "w+");
      for (auto src : sources.styles) {
        src.getInlineSrc([&file](String data) {
          Serial.println(ESP.getMaxFreeBlockSize());
          file.print(data);
        });
      }
      file.close();
      res = request->beginResponse(LittleFS, F("/__MLTemp/tmp.css"), F("text/css;charset=utf-8"));
    } else {
      auto resStream = request->beginResponseStream(F("text/css;charset=utf-8"));
      res = resStream;
      for (auto src : sources.styles) {
        src.getInlineSrc([&resStream](String data) {
          resStream->print(data);
        });
      }
    }

    res->setCode(200);
    res->addHeader(F("Cache-Control"), F("no-cache"));
    res->addHeader(F("X-Content-Type-Options"), F("nosniff"));
    request->send(res);
  }));

  this->handlers.push_back(this->server->on(inlineScriptsUrl.c_str(), HTTP_GET, [=](AsyncWebServerRequest* request) {
    auto sources = this->compileSrc();

    AsyncWebServerResponse* res = nullptr;

    if (LittleFS.begin()) {
      auto file = LittleFS.open(F("/__MLTemp/tmp.js"), "w+");
      for (auto src : sources.styles) {
        src.getInlineSrc([&file](String data) {
          Serial.println(ESP.getMaxFreeBlockSize());
          file.print(data);
        });
      }
      file.close();
      res = request->beginResponse(LittleFS, F("/__MLTemp/tmp.js"), F("text/javascript;charset=utf-8"));
    } else {
      auto resStream = request->beginResponseStream(F("text/javascript;charset=utf-8"));
      res = resStream;
      for (auto src : sources.scripts) {
        src.getInlineSrc([&resStream](String data) {
          resStream->print(data);
        });
      }
    }

    res->setCode(200);
    res->addHeader(F("Cache-Control"), F("no-cache"));
    res->addHeader(F("X-Content-Type-Options"), F("nosniff"));
    request->send(res);
  }));

  for (auto moduleInfo : this->modules) {

    if (moduleInfo.CSS.fileName && moduleInfo.CSS.file) {
      this->handlers.push_back(this->server->on(("/" + moduleInfo.CSS.fileName + ".css").c_str(), [=](AsyncWebServerRequest* r) { this->serveSharedStatic(r, SharedStaticType::CSS, moduleInfo.CSS.file, moduleInfo.CSS.fileLength); }));
    }
    if (moduleInfo.JS.fileName && moduleInfo.JS.file) {
      this->handlers.push_back(this->server->on(("/" + moduleInfo.JS.fileName + ".js").c_str(), [=](AsyncWebServerRequest* r) { this->serveSharedStatic(r, SharedStaticType::JS, moduleInfo.JS.file, moduleInfo.JS.fileLength); }));
    }
  }

  this->handlers.push_back(server->addHandler(&this->ws));

  for (auto srcHandler : this->registeredSources) {
    srcHandler->performRegistration(this->server);
  }
}

void MaterializeLayout::serveSharedStatic(AsyncWebServerRequest* request, SharedStaticType type, const uint8_t* file, size_t fileLength)
{
  AsyncWebServerResponse* res = nullptr;

  switch (type) {
  case SharedStaticType::CSS:
    res = request->beginResponse_P(200, F("text/css;charset=utf-8"), file, fileLength);
    break;
  case SharedStaticType::JS:
    res = request->beginResponse_P(200, F("application/javascript;charset=utf-8"), file, fileLength);
    break;
  }

  res->addHeader(F("Content-Encoding"), F("gzip"));
  res->addHeader(F("Cache-Control"), F("max-age=31536000, immutable"));
  res->addHeader(F("X-Content-Type-Options"), F("nosniff"));
  request->send(res);
}

void MaterializeLayout::triggerDispatch()
{
  size_t totalLength = 0;
  for (auto event : this->bakedEvents) {
    totalLength += event.length();
  }

  if (totalLength >= ML_MAX_BAKED_EVENT_SIZE_BEFORE_FORCED_PUSH) {
    this->dispatchTicker.detach();
    this->_performDispatch();
  } else if (!this->dispatchTicker.active()) {
    this->dispatchTicker.once_ms(ML_EVENT_DISPATCH_POLLING_TIME, [this]() { this->_performDispatch(); });
  }
}

void MaterializeLayout::_performDispatch()
{
  this->dispatchTicker.detach();

  this->ws.cleanupClients();
  if (!this->ws.count()) {
    this->bakedEvents.clear();
    return;
  };

  size_t totalLength = 0;
  for (auto event : this->bakedEvents) {
    totalLength += event.length();
  }

  String result;

  do {
    DynamicJsonDocument doc(JSON_ARRAY_SIZE(this->bakedEvents.size()) + JSON_STRING_SIZE(totalLength) + this->bakedEvents.size() + 128);
    auto arr = doc.to<JsonArray>();

    for (auto event : this->bakedEvents) {
      arr.add(dynamiclyDeserializeJson(event));
    }

    serializeJson(doc, result);
    this->bakedEvents.clear();
  } while (false);

  this->ws.textAll(result);
}