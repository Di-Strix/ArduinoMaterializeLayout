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

#include "MaterializeLayoutTypes.h"
#include "Page/Page.h"
#include "ComponentFactory/ComponentFactory.h"
#include "DefaultModules/NormalizeCssModule/NormalizeCssModule.h"
#include "DefaultModules/MaterializeCss/MaterializeCssModule.h"
#include "DefaultModules/MainAppModule/MainAppModule.h"
#include "SharedStatic.h"


template <typename T>
inline DynamicJsonDocument dynamiclyDeserializeJson(T data)
{
  DynamicJsonDocument doc(ESP.getMaxFreeBlockSize() - 512);
  deserializeJson(doc, data);
  doc.shrinkToFit();
  return doc;
}

class MaterializeLayout : public Page
{
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
  MaterializeLayoutComponent<C> createAndAppendComponent(String moduleName, int component, MaterializeLayout *parent);

#ifdef MATERIALIZE_LAYOUT_ENABLE_ESP_ASYNC_WEB_SERVER
  /**
   * @brief registers callbacks to provide MaterializeLayout functionality
   * 
   * @param s pointer to the the AsyncWebServer instance
   */
  void registerInEspAsyncWebServer(AsyncWebServer *s);

  /**
   * @brief responds to the AsyncWebServerRequest request with the necessary headers and data according to the given SharedStaticType type
   * 
   * @param request AsyncWebServerRequest request
   * @param type SharedStaticType content type
   */
  void serveSharedStatic(AsyncWebServerRequest *request, SharedStaticType type, const uint8_t *file, size_t fileLength);
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
MaterializeLayoutComponent<C> MaterializeLayout::createAndAppendComponent(String moduleName, int component, MaterializeLayout *parent)
{
  auto el = this->createComponent<C>(moduleName, component);
  if (!el)
    return 0;

  parent->appendChild(el);
  return el;
}

#ifdef MATERIALIZE_LAYOUT_ENABLE_ESP_ASYNC_WEB_SERVER
void MaterializeLayout::registerInEspAsyncWebServer(AsyncWebServer *s)
{
  s->on("/index.html", HTTP_GET, [=](AsyncWebServerRequest *request)
        {
          String page = this->getHTML();
          AsyncWebServerResponse *res = request->beginResponse(200, F("text/html;charset=utf-8"), page);
          res->addHeader(F("Cache-Control"), F("no-cache"));
          request->send(res);
        });

  for (auto [moduleName, moduleInfo] : this->modules)
  {
    if (moduleInfo.CSSFileName && moduleInfo.CSSFile)
    {
      s->on(("/" + moduleInfo.CSSFileName + ".css").c_str(), [=](AsyncWebServerRequest *r)
            { this->serveSharedStatic(r, SharedStaticType::CSS, moduleInfo.CSSFile, moduleInfo.CSSFileLength); });
    }
    if (moduleInfo.JSFileName && moduleInfo.JSFile)
    {
      s->on(("/" + moduleInfo.JSFileName + ".js").c_str(), [=](AsyncWebServerRequest *r)
            { this->serveSharedStatic(r, SharedStaticType::JS, moduleInfo.JSFile, moduleInfo.JSFileLength); });
    }
  }

  s->on(
      String(F("/materializeLayoutActions/emitAction")).c_str(), HTTP_POST, [=](AsyncWebServerRequest *request) {}, NULL,
      [&](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
      {
        for (size_t i = 0; i < len; i++)
        {
          this->tempData += (char)data[i];
        }
        if (index + len == total)
        {
          AsyncWebServerResponse *response = request->beginResponse(200, F("text/plain;charset=utf-8"), "");
          response->addHeader(F("Cache-Control"), F("no-cache"));
          request->send(response);

          DynamicJsonDocument doc = dynamiclyDeserializeJson(this->tempData);
          this->tempData.clear();
          this->emit(doc["id"].as<size_t>(), doc["value"].as<String>());
        }
      });

  s->on(
      String(F("/materializeLayoutActions/update")).c_str(), HTTP_GET, [=](AsyncWebServerRequest *request)
      {
        auto registrations = this->getRegistrationService()->getRegistrations();

        struct updateValue_t
        {
          String id;
          String data;
        };

        std::list<updateValue_t> updateData;
        for (auto reg : registrations)
        {
          updateData.push_back({(String)reg.id, reg.getter()});
        }

        DynamicJsonDocument doc(ESP.getMaxFreeBlockSize() - 512);
        for (auto val : updateData)
        {
          doc[val.id] = val.data;
        }
        doc.shrinkToFit();

        String res;
        serializeJson(doc, res);

        AsyncWebServerResponse *response = request->beginResponse(200, F("application/json;charset=utf-8"), res);
        response->addHeader(F("Cache-Control"), F("no-cache"));
        request->send(response);
      });
}

void MaterializeLayout::serveSharedStatic(AsyncWebServerRequest *request, SharedStaticType type, const uint8_t *file, size_t fileLength)
{
  AsyncWebServerResponse *res = nullptr;

  switch (type)
  {
  case SharedStaticType::CSS:
    res = request->beginResponse_P(200, F("text/css;charset=utf-8"), file, fileLength);
    break;
  case SharedStaticType::JS:
    res = request->beginResponse_P(200, F("application/javascript;charset=utf-8"), file, fileLength);
    break;
  }

  res->addHeader(F("Content-Encoding"), F("gzip"));
  res->addHeader(F("Cache-Control"), F("max-age=31536000, immutable"));
  request->send(res);
}

#endif //MATERIALIZE_LAYOUT_ENABLE_ESP_ASYNC_WEB_SERVER

#endif