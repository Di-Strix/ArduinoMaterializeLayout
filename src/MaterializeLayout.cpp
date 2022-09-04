#include "MaterializeLayout.h"

MaterializeLayout::MaterializeLayout(String pageTitle)
    : Page(new MLArgs(), pageTitle)
{
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

    this->ws.textAll(json);
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
    if ((moduleInfo.CSS.fileName && moduleInfo.CSS.file) || moduleInfo.inlineCSS) {
      src.styles.push_front({ moduleInfo.CSS.fileName, moduleInfo.inlineCSS });
    }
    if ((moduleInfo.JS.fileName && moduleInfo.JS.file) || moduleInfo.inlineJS) {
      src.scripts.push_front({ moduleInfo.JS.fileName, moduleInfo.inlineJS });
    }

    for (auto handler : moduleInfo.handlers) {
      if (handler.onInitFN.isEmpty() && handler.updateFN.isEmpty())
        continue;

      moduleHandlers += F("class ");
      moduleHandlers += handler.name;
      moduleHandlers += F(" extends Handler {"
                          "onInit() {");
      moduleHandlers += handler.onInitFN;
      moduleHandlers += F("}"
                          "update(el, value) {");
      moduleHandlers += handler.updateFN;
      moduleHandlers += F("}};");
      moduleHandlers += F("dynamicUpdateService.addHandler('");
      moduleHandlers += handler.name;
      moduleHandlers += F("', new ");
      moduleHandlers += handler.name;
      moduleHandlers += F("());");
    }
  }

  moduleHandlers += F("dynamicUpdateService.init(\"");
  moduleHandlers += String(this->ws.url()).substring(1);
  moduleHandlers += F("\");");

  src.scripts.push_back({ "", moduleHandlers });

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

  this->handlers.push_back(this->server->on("/index.html", HTTP_GET, [=](AsyncWebServerRequest* request) {
    String page = this->getHTML();
    AsyncWebServerResponse* res = request->beginResponse(200, F("text/html;charset=utf-8"), page);
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
