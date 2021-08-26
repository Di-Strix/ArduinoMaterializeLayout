#include "MaterializeLayout.h"

MaterializeLayout::MaterializeLayout(String pageTitle) : Page(pageTitle, [this]() -> PageSources
                                                              { return this->compileSrc(); })
{
  this->injectModule(F("normalize"), getNormalizeCssModule());
  this->injectModule(MATERIALIZE_CSS_MODULE, getMaterializeCssModule());
  this->injectModule(F("mainApp"), getMainAppModule());
}

PageSources MaterializeLayout::compileSrc()
{
  PageSources src;
  String moduleHandlers;
  size_t counter = 0;

  for (auto [moduleName, moduleInfo] : this->modules)
  {
    if ((moduleInfo.CSS.fileName && moduleInfo.CSS.file) || moduleInfo.inlineCSS)
    {
      src.styles.push_front({moduleInfo.CSS.fileName, moduleInfo.inlineCSS});
    }
    if ((moduleInfo.JS.fileName && moduleInfo.JS.file) || moduleInfo.inlineJS)
    {
      src.scripts.push_front({moduleInfo.JS.fileName, moduleInfo.inlineJS});
    }

    for (auto handler : moduleInfo.handlers)
    {
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

  src.scripts.push_back({"", moduleHandlers});

  return src;
}

bool MaterializeLayout::injectModule(String moduleName, MaterializeLayoutModule moduleInfo)
{
  auto it = this->modules.find(moduleName);
  if (it != this->modules.end())
    return false;

  this->modules[moduleName] = moduleInfo;

  return true;
}