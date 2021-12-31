#ifndef _MATERIALIZE_LAYOUT_H_
#define _MATERIALIZE_LAYOUT_H_

#if (defined(_ESPAsyncWebServer_H_) || defined(MATERIALIZE_LAYOUT_USE_ESP_ASYNC_WEB_SERVER))
#define MATERIALIZE_LAYOUT_ENABLE_ESP_ASYNC_WEB_SERVER
#endif

#ifdef MATERIALIZE_LAYOUT_ENABLE_ESP_ASYNC_WEB_SERVER
#include "ESPAsyncWebServer.h"
#endif

#include <ArduinoJson.h>
#include <map>

#include "Debugging/Debugging.h"

#include "ComponentFactory/ComponentFactory.h"
#include "MaterializeLayoutTypes.h"
#include "Modules/ChartistModule/ChartistModule.h"
#include "Modules/MainAppModule/MainAppModule.h"
#include "Modules/MaterializeCssModule/MaterializeCssModule.h"
#include "Modules/NormalizeCssModule/NormalizeCssModule.h"
#include "Page/Page.h"

template <typename T>
inline DynamicJsonDocument dynamiclyDeserializeJson(T data)
{
#if defined(ARDUINO_ARCH_ESP8266)
  DynamicJsonDocument doc(ESP.getMaxFreeBlockSize() - 512);
#elif defined(ARDUINO_ARCH_ESP32)
  DynamicJsonDocument doc(ESP.getMaxAllocHeap());
#endif
  deserializeJson(doc, data);
  doc.shrinkToFit();
  return doc;
}

class MaterializeLayout : public Page {
  private:
  using Page::getRegistrationService;
  String tempData;

  std::map<String, MaterializeLayoutModule> modules;

  PageSources compileSrc();

  public:
  MaterializeLayout(String pageTitle);

  bool injectModule(String moduleName, MaterializeLayoutModule moduleInfo);

  template <template <class> class C>
  MaterializeLayoutComponent<C> createComponent(String moduleName, int component);

  template <template <class> class C>
  MaterializeLayoutComponent<C> createAndAppendComponent(String moduleName, int component, MaterializeLayoutComponent<HTMLElement> parent);

  template <template <class> class C>
  MaterializeLayoutComponent<C> createAndAppendComponent(String moduleName, int component, MaterializeLayout* parent);

#ifdef MATERIALIZE_LAYOUT_ENABLE_ESP_ASYNC_WEB_SERVER
  /**
   * @brief registers callbacks to provide MaterializeLayout functionality
   * 
   * @param s pointer to the the AsyncWebServer instance
   */
  void registerInEspAsyncWebServer(AsyncWebServer* s);

  /**
   * @brief responds to the AsyncWebServerRequest request with the necessary headers and data according to the given SharedStaticType type
   * 
   * @param request AsyncWebServerRequest request
   * @param type SharedStaticType content type
   */
  void serveSharedStatic(AsyncWebServerRequest* request, SharedStaticType type, const uint8_t* file, size_t fileLength);
#endif
};

template <template <class> class C>
MaterializeLayoutComponent<C> MaterializeLayout::createComponent(String moduleName, int component)
{
  auto it = this->modules.find(moduleName);
  if (it == this->modules.end())
    return 0;

  auto moduleInfo = this->modules[moduleName];

  auto mit = moduleInfo.declarations.find(component);
  if (mit == moduleInfo.declarations.end())
    return 0;

  auto componentCreator = moduleInfo.declarations[component];
  return static_cast<MaterializeLayoutComponent<C>>(componentCreator->create(this->getRegistrationService()));
}

template <template <class> class C>
MaterializeLayoutComponent<C> MaterializeLayout::createAndAppendComponent(String moduleName, int component, MaterializeLayoutComponent<HTMLElement> parent)
{
  auto el = this->createComponent<C>(moduleName, component);
  if (!el)
    return 0;

  parent->appendChild(el);
  return el;
}

template <template <class> class C>
MaterializeLayoutComponent<C> MaterializeLayout::createAndAppendComponent(String moduleName, int component, MaterializeLayout* parent)
{
  auto el = this->createComponent<C>(moduleName, component);
  if (!el)
    return 0;

  parent->appendChild(el);
  return el;
}

#ifdef MATERIALIZE_LAYOUT_ENABLE_ESP_ASYNC_WEB_SERVER
void MaterializeLayout::registerInEspAsyncWebServer(AsyncWebServer* s)
{
  s->on("/index.html", HTTP_GET, [=](AsyncWebServerRequest* request) {
    String page = this->getHTML();
    AsyncWebServerResponse* res = request->beginResponse(200, F("text/html;charset=utf-8"), page);
    res->addHeader(F("Cache-Control"), F("no-cache"));
    res->addHeader(F("X-Content-Type-Options"), F("nosniff"));
    request->send(res);
  });

  for (auto [moduleName, moduleInfo] : this->modules) {
    if (moduleInfo.CSS.fileName && moduleInfo.CSS.file) {
      s->on(("/" + moduleInfo.CSS.fileName + ".css").c_str(), [=](AsyncWebServerRequest* r) { this->serveSharedStatic(r, SharedStaticType::CSS, moduleInfo.CSS.file, moduleInfo.CSS.fileLength); });
    }
    if (moduleInfo.JS.fileName && moduleInfo.JS.file) {
      s->on(("/" + moduleInfo.JS.fileName + ".js").c_str(), [=](AsyncWebServerRequest* r) { this->serveSharedStatic(r, SharedStaticType::JS, moduleInfo.JS.file, moduleInfo.JS.fileLength); });
    }
  }

  s->on(
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
      });

  s->on(
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
      });
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

#endif //MATERIALIZE_LAYOUT_ENABLE_ESP_ASYNC_WEB_SERVER

#endif