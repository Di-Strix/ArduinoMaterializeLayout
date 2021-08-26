#ifndef _MATERIALIZE_LAYOUT_TYPES_H_
#define _MATERIALIZE_LAYOUT_TYPES_H_

#include <Arduino.h>
#include <map>

#include "ComponentFactory/ComponentFactory.h"

enum class SharedStaticType
{
  CSS,
  JS
};

struct SrcFile
{
  String fileName;
  const uint8_t *file;
  size_t fileLength;
};

struct MaterializeLayoutModule
{
  SrcFile CSS;

  SrcFile JS;

  std::map<int, AbstractComponentCreator *> declarations;
};

#endif //_MATERIALIZE_LAYOUT_TYPES_H_