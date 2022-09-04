#pragma once

#if UNIT_TEST
#include "Mocks/Arduino/Arduino.h"
#include "Mocks/ESPAsyncWebServer/ESPAsyncWebServer.h"
#else
#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#endif

#include <ArduinoJson.h>
#include <list>
#include <type_traits>
#include <utility>

#include "IdGenerator.h"
#include "MaterializeLayoutTypes.h"
#include "Modules/ChartistModule/ChartistModule.h"
#include "Modules/MainAppModule/MainAppModule.h"
#include "Modules/MaterializeCssModule/MaterializeCssModule.h"
#include "Modules/NormalizeCssModule/NormalizeCssModule.h"
#include "Page/Page.h"

template <typename T>
inline DynamicJsonDocument dynamiclyDeserializeJson(T data)
{
  DynamicJsonDocument doc(ESP.getMaxFreeBlockSize() - 512);
  deserializeJson(doc, data);
  doc.shrinkToFit();
  return doc;
}

class MaterializeLayout : public Page<HTMLElementArgs*> {
  private:
  HTMLElementArgs argCollection;

  std::list<MaterializeLayoutModule> modules;
  std::list<AsyncWebHandler> handlers;
  std::list<WebSourceHandler*> registeredSources;

  AsyncWebServer* server;
  AsyncWebSocket ws = AsyncWebSocket("/ML" + String(this->getId()));

  PageSources compileSrc();
  void unregisterHandlers();

  HTMLElementArgs* getArgs();

  public:
  /**
   * @brief Construct a new Materialize Layout object
   *
   * @param pageTitle title displayed on the tab
   */
  MaterializeLayout(String pageTitle);

  ~MaterializeLayout();

  /**
   * @brief prepares module static files to be served
   *
   * @param moduleInfo
   * @return true on success
   * @return false on failure
   */
  bool injectModule(const MaterializeLayoutModule& moduleInfo);

  template <template <class> class C, typename... Args>
  MaterializeLayoutComponent<C> constexpr createComponent(Args... args);

  template <template <class> class C, typename... Args>
  MaterializeLayoutComponent<C> constexpr createAndAppendComponent(MaterializeLayoutComponent<HTMLElement> parent = nullptr, Args... args);

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
};

template <template <class> class C, typename... Args>
MaterializeLayoutComponent<C> constexpr MaterializeLayout::createComponent(Args... args)
{
  const bool isInherited = std::is_base_of_v<MaterializeLayoutComponent_t<HTMLElement>, MaterializeLayoutComponent_t<C>>;
  const bool isConstructible = std::is_constructible_v<MaterializeLayoutComponent_t<C>, HTMLElementArgs*, Args...>;

  static_assert(isInherited, "Component must be inherited from HTMLElement");
  static_assert(isConstructible, "Incompatible args");

  if constexpr (isInherited && isConstructible)
    return new MaterializeLayoutComponent_t<C>(this->getArgs(), std::forward<Args>(args)...);
  else
    return nullptr;
}

template <template <class> class C, typename... Args>
MaterializeLayoutComponent<C> constexpr MaterializeLayout::createAndAppendComponent(MaterializeLayoutComponent<HTMLElement> parent, Args... args)
{
  if (!parent)
    parent = this;

  auto el = this->createComponent<C>(std::forward<Args>(args)...);
  if (!el)
    return nullptr;

  parent->appendChild(el);
  return el;
}