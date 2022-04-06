#pragma once

#include "../../ComponentFactory/ComponentFactory.h"
#include "../../MaterializeLayoutTypes.h"
#include "ChartistModuleStatic.h"

#include "Components/Chart.h"
#include "Components/ChartLine/ChartLine.h"

#define ChartistComponent (int)ChartistComponentEnum

enum class ChartistComponentEnum {
  Chart
};

const MaterializeLayoutModule getChartistModule();
