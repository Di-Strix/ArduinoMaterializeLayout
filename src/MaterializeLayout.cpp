#include "MaterializeLayout.h"

bool MaterializeLayout::injectModule(String moduleName, MaterializeLayoutModule moduleInfo)
{
  auto it = this->modules.find(moduleName);
  if (it != this->modules.end())
    return false;

  this->modules[moduleName] = moduleInfo;

  return true;
}