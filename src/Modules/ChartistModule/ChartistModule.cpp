#include "ChartistModule.h"

const MaterializeLayoutModule getChartistModule()
{
  return {
    { CHARTIST_CSS_HASH, CHARTIST_CSS, CHARTIST_CSS_LENGTH },
    { CHARTIST_JS_HASH, CHARTIST_JS, CHARTIST_JS_LENGTH },
    [](ResponseWriter writer) { writer(F(".chart-overflow-visible > svg {overflow: visible;}")); },
    [](ResponseWriter writer) {},
    { { F("ChartistHandler"),
        [](ResponseWriter writer) {
          // clang-format off
            writer(F("this.charts = [];"));
            writer(F("document.querySelectorAll('[data-chart=true]').forEach(ch => {"));
            writer(  F("const chObj = {"));
            writer(          F("id: ch.dataset.id,"));
            writer(          F("chart: new Chartist.Line("));
            writer(              F("ch,"));
            writer(              F("{},"));
            writer(              F("{ low: 0, showArea: true, showPoint: true, fullWidth: true }"));
            writer(          F("),"));
            writer(      F("};"));
            writer(  F("this.charts.push(chObj);"));
            writer(F("});"));
          // clang-format on
        },
        [](ResponseWriter writer) {
          writer(F("const chObj = this.charts.filter(c => c.id === el.dataset.id)[0];"));
          writer(F("chObj.chart.update(JSON.parse(value));"));
        } } }
  };
}