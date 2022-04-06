#include "ChartistModule.h"

const MaterializeLayoutModule getChartistModule()
{
  return {
    { "chartist",
        CHARTIST_CSS,
        CHARTIST_CSS_LENGTH },

    { "chartist",
        CHARTIST_JS,
        CHARTIST_JS_LENGTH },
    F(".chart-overflow-visible > svg {overflow: visible;}"),
    "",
    { { F("ChartistHandler"),
        F(
            // clang-format off
              "this.charts = [];"
              "document.querySelectorAll('[data-chart=true]').forEach(ch => {"
                "const chObj = {"
                        "id: ch.dataset.id,"
                        "chart: new Chartist.Line("
                            "ch,"
                            "{},"
                            "{ low: 0, showArea: true, showPoint: true, fullWidth: true }"
                        "),"
                    "};"
                "this.charts.push(chObj);"
              "});"
            // clang-format on
            ),
        F(
            // clang-formating off
            "const chObj = this.charts.filter(c => c.id === el.dataset.id)[0];"
            "chObj.chart.update(JSON.parse(value));"
            // clang-formating on
            ) } }
  };
}