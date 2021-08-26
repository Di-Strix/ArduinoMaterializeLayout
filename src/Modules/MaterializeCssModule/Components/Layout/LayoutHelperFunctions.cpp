#include "LayoutHelperFunctions.h"

String getLayoutTypeClass(LayoutType lt)
{
  switch (lt) {
  case LayoutType::NONE:
  default:
    return String();
  case LayoutType::COLUMN:
    return F("col");
  case LayoutType::ROW:
    return F("row");
  }
}