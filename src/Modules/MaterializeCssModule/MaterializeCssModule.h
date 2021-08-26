#ifndef _MATERIALIZE_LAYOUT_MATERIALIZE_CSS_MODULE_H_
#define _MATERIALIZE_LAYOUT_MATERIALIZE_CSS_MODULE_H_

#include "../../ComponentFactory/ComponentFactory.h"
#include "../../MaterializeLayoutTypes.h"
#include "Components/Button/Button.h"
#include "Components/Field/Field.h"
#include "Components/Layout/Layout.h"
#include "Components/Tab/Tab.h"
#include "Components/TabGroup/TabGroup.h"
#include "Components/Text/Text.h"
#include "MaterializeCssStatic.h"

#define MaterializeCssComponent (int)MaterializeCssComponentEnum
#define MATERIALIZE_CSS_MODULE F("materialize")

enum class MaterializeCssComponentEnum {
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