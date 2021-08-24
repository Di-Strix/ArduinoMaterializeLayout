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

struct MaterializeLayoutModule
{
  String CSSFileName;
  const uint8_t *CSSFile;
  size_t CSSFileLength;

  String JSFileName;
  const uint8_t *JSFile;
  size_t JSFileLength;

  std::map<int, AbstractComponentCreator *> declarations;
};

#endif //_MATERIALIZE_LAYOUT_TYPES_H_