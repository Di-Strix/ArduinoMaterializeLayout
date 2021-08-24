#ifndef _MATERIALIZE_LAYOUT_MATERIALIZE_CSS_MODULE_H_
#define _MATERIALIZE_LAYOUT_MATERIALIZE_CSS_MODULE_H_

#include "../../ComponentFactory/ComponentFactory.h"
#include "../../MaterializeLayoutTypes.h"
#include "MaterializeCssStatic.h"
#include "../../Button/Button.h"
#include "../../Field/Field.h"
#include "../../Layout/Layout.h"
#include "../../Tab/Tab.h"
#include "../../TabGroup/TabGroup.h"
#include "../../Text/Text.h"

#define MaterializeCssComponent (int)MaterializeCssComponentEnum

enum class MaterializeCssComponentEnum
{
  Button,
  Field,
  Layout,
  Tab,
  TabGroup,
  StaticText,
  DynamicText
};

MaterializeLayoutModule getMaterializeCssModule();

#endif //_MATERIALIZE_LAYOUT_MATERIALIZE_CSS_MODULE_H_