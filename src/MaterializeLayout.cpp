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
    if (moduleInfo.CSS.fileName && moduleInfo.CSS.file)
    {
      src.styles.push_front({moduleInfo.CSS.fileName});
    }
    if (moduleInfo.JS.fileName && moduleInfo.JS.file)
    {
      src.scripts.push_front({moduleInfo.JS.fileName});
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