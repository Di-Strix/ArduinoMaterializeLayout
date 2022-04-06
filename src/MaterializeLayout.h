#pragma once

#include <ArduinoJson.h>
#include <map>
#include <ESPAsyncWebServer.h>

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
  DynamicJsonDocument doc(ESP.getMaxFreeBlockSize() - 512);
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
