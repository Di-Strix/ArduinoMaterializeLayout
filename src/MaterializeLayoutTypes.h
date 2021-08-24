#ifndef _MATERIALIZE_LAYOUT_TYPES_H_
#define _MATERIALIZE_LAYOUT_TYPES_H_

#include <Arduino.h>
#include <map>

#include "ComponentFactory/ComponentFactory.h"

enum class SharedStaticType
{
  MATERIALIZE_CSS,
  MATERIALIZE_JS,
  NORMALIZE_CSS,
  APPLICATION_JS,
};

#endif //_MATERIALIZE_LAYOUT_TYPES_H_