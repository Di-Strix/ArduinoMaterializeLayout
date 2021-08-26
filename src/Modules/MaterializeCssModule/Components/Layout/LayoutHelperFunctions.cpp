#include "LayoutHelperFunctions.h"

String getLayoutTypeClass(LayoutTypes lt)
{
  switch (lt)
  {
  case LayoutTypes::NONE:
  default:
    return String();
  case LayoutTypes::COLUMN:
    return F("col");
  case LayoutTypes::ROW:
    return F("row");
  }
}