#include "MaterializeLayout.h"

MaterializeLayout::MaterializeLayout(String pageTitle) : Page(pageTitle, [this]() -> PageSources
                                                              { return this->compileSrc(); })
{
  this->injectModule(F("normalize"), getNormalizeCssModule());
  this->injectModule(F("materialize"), getMaterializeCssModule());
  this->injectModule(F("mainApp"), getMainAppModule());
}

PageSources MaterializeLayout::compileSrc()
{
  PageSources src;

  for (auto [moduleName, moduleInfo] : this->modules)
  {
    if (moduleInfo.CSSFileName && moduleInfo.CSSFile)
    {
      src.stylesFileNames.push_front(moduleInfo.CSSFileName);
    }
    if (moduleInfo.JSFileName && moduleInfo.JSFile)
    {
      src.scriptFileNames.push_front(moduleInfo.JSFileName);
    }
  }

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