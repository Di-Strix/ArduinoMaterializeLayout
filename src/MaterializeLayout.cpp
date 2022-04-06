#include "MaterializeLayout.h"

MaterializeLayout::MaterializeLayout(String pageTitle)
    : Page(pageTitle, [this]() -> PageSources { return this->compileSrc(); })
{
  this->injectModule(getNormalizeCssModule());
  this->injectModule(getMaterializeCssModule());
  this->injectModule(getMainAppModule());
}

PageSources MaterializeLayout::compileSrc()
{
  PageSources src;
  String moduleHandlers;
  size_t counter = 0;

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

      counter++;
    }
  }

  moduleHandlers += F("dynamicUpdateService.init();");

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

  this->handlers.push_back(this->server->on(
      String(F("/materializeLayoutActions/emitAction")).c_str(), HTTP_POST, [=](AsyncWebServerRequest* request) {}, NULL,
      [&](AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t index, size_t total) {
        for (size_t i = 0; i < len; i++) {
          this->tempData += (char)data[i];
        }
        if (index + len == total) {
          AsyncWebServerResponse* response = request->beginResponse(200, F("text/plain;charset=utf-8"), "");
          response->addHeader(F("Cache-Control"), F("no-cache"));
          response->addHeader(F("X-Content-Type-Options"), F("nosniff"));
          request->send(response);

          DynamicJsonDocument doc = dynamiclyDeserializeJson(this->tempData);
          this->tempData.clear();
          this->emit(doc["id"].as<size_t>(), doc["value"].as<String>());
        }
      }));

  this->handlers.push_back(this->server->on(
      String(F("/materializeLayoutActions/update")).c_str(), HTTP_GET, [=](AsyncWebServerRequest* request) {
        auto registrations = this->getRegistrationService()->getRegistrations();

        struct updateValue_t {
          String id;
          UpdateMsg data;
        };

        std::list<updateValue_t> updateData;
        size_t size = 0;

        for (auto reg : registrations) {
          updateValue_t v = { (String)reg.id, reg.getter() };

          size += v.id.length() + 1;
          size += v.data.handlerId.length() + 1;
          size += v.data.value.length() + 1;

          updateData.push_back(v);
        }

        DynamicJsonDocument doc(size + JSON_OBJECT_SIZE(updateData.size()) * JSON_OBJECT_SIZE(2));
        for (auto val : updateData) {
          auto obj = doc.createNestedObject(val.id);

          obj[F("handlerId")] = val.data.handlerId;
          obj[F("value")] = val.data.value;
        }
        doc.shrinkToFit();

        String res;
        serializeJson(doc, res);

        if (res == F("null"))
          res = F("{}");

        AsyncWebServerResponse* response = request->beginResponse(200, F("application/json;charset=utf-8"), res);
        response->addHeader(F("Cache-Control"), F("no-cache"));
        response->addHeader(F("X-Content-Type-Options"), F("nosniff"));
        request->send(response);
      }));
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
