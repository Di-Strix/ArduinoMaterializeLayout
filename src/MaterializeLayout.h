#ifndef _MATERIALIZE_LAYOUT_H_
#define _MATERIALIZE_LAYOUT_H_

#if (defined(_ESPAsyncWebServer_H_) || defined(MATERIALIZE_LAYOUT_USE_ESP_ASYNC_WEB_SERVER))
#define MATERIALIZE_LAYOUT_ENABLE_ESP_ASYNC_WEB_SERVER
#endif

#ifdef MATERIALIZE_LAYOUT_ENABLE_ESP_ASYNC_WEB_SERVER
#include "ESPAsyncWebServer.h"
#endif

#include <ArduinoJson.h>
#include "Page/Page.h"
#include "SharedStatic.h"

enum class SharedStaticType
{
  MATERIALIZE_CSS,
  MATERIALIZE_JS,
  NORMALIZE_CSS,
  APPLICATION_JS,
};

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
  using Page::Page;

private:
  using Page::getRegistrationService;
  String tempData;

public:
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
  void serveSharedStatic(AsyncWebServerRequest *request, SharedStaticType type);
#endif
};

// ======================= IMPLEMENTATION =======================

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

  s->on("/materialize.css", [=](AsyncWebServerRequest *r)
        { this->serveSharedStatic(r, SharedStaticType::MATERIALIZE_CSS); });

  s->on("/materialize.js", [=](AsyncWebServerRequest *r)
        { this->serveSharedStatic(r, SharedStaticType::MATERIALIZE_JS); });

  s->on("/normalize.css", [=](AsyncWebServerRequest *r)
        { this->serveSharedStatic(r, SharedStaticType::NORMALIZE_CSS); });

  s->on("/" APPLICATION_JS_HASH ".js", [=](AsyncWebServerRequest *r)
        { this->serveSharedStatic(r, SharedStaticType::APPLICATION_JS); });

  s->on(
      "/materializeLayoutActions/emitAction", HTTP_POST, [=](AsyncWebServerRequest *request) {}, NULL,
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
      "/materializeLayoutActions/update", HTTP_GET, [=](AsyncWebServerRequest *request)
      {
        auto registrations = this->getRegistrationService()->getRegistrations();

        DynamicJsonDocument doc(ESP.getMaxFreeBlockSize() - 512);
        for (auto reg : registrations)
        {
          doc[(String)reg.id] = reg.getter();
        }
        doc.shrinkToFit();

        String res;
        serializeJson(doc, res);

        AsyncWebServerResponse *response = request->beginResponse(200, F("application/json;charset=utf-8"), res);
        response->addHeader(F("Cache-Control"), F("no-cache"));
        request->send(response);
      });
}

void MaterializeLayout::serveSharedStatic(AsyncWebServerRequest *request, SharedStaticType type)
{
  AsyncWebServerResponse *res = nullptr;
  switch (type)
  {
  case SharedStaticType::MATERIALIZE_CSS:
    res = request->beginResponse_P(200, F("text/css;charset=utf-8"), MATERIALIZE_CSS, MATERIALIZE_CSS_LENGTH);
    break;
  case SharedStaticType::MATERIALIZE_JS:
    res = request->beginResponse_P(200, F("application/javascript;charset=utf-8"), MATERIALIZE_JS, MATERIALIZE_JS_LENGTH);
    break;
  case SharedStaticType::NORMALIZE_CSS:
    res = request->beginResponse_P(200, F("text/css;charset=utf-8"), NORMALIZE_CSS, NORMALIZE_CSS_LENGTH);
    break;
  case SharedStaticType::APPLICATION_JS:
    res = request->beginResponse_P(200, F("application/javascript;charset=utf-8"), APPLICATION_JS, APPLICATION_JS_LENGTH);
    break;
  }
  res->addHeader(F("Content-Encoding"), F("gzip"));
  res->addHeader(F("Cache-Control"), F("max-age=31536000, immutable"));
  request->send(res);
}

#endif

#endif