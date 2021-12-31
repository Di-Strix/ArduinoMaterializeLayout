#ifndef _MATERIALIZE_LAYOUT_CHARTIST_MODULE_H_
#define _MATERIALIZE_LAYOUT_CHARTIST_MODULE_H_

#include "ChartistModuleStatic.h"
#include <ComponentFactory/ComponentFactory.h>
#include <MaterializeLayoutTypes.h>

#include "Components/Chart.h"
#include "Components/ChartLine/ChartLine.h"

#define ChartistComponent (int)ChartistComponentEnum

enum class ChartistComponentEnum {
  Chart
};

const MaterializeLayoutModule getChartistModule();

#endif //_MATERIALIZE_LAYOUT_CHARTIST_MODULE_H_