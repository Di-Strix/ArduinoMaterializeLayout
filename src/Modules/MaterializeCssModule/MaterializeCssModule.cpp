#include "MaterializeCssModule.h"

MaterializeLayoutModule getMaterializeCssModule()
{
  return {
    { "materialize",
        MATERIALIZE_CSS,
        MATERIALIZE_CSS_LENGTH },

    { "materialize",
        MATERIALIZE_JS,
        MATERIALIZE_JS_LENGTH },

    "",
    "",
    { { F("MaterializeCssHandler"),
        F(
            // clang-format off
            "const debounce = (callback, delay) => {"
              "let timeoutId = -1;"
              "return (...props) => {"
                "clearTimeout(timeoutId);"
                "timeoutId = setTimeout(callback.bind(null, ...props), delay);"
              "}"
            "};"

            "M.AutoInit();"

            "document"
              ".querySelectorAll('[data-MCSS-emitOnClick=true]')"
              ".forEach(el => {"
                "el.addEventListener('click', dynamicUpdateService.emitEvent)"
              "});"
            "document"
              ".querySelectorAll('[data-MCSS-emitOnChange=true]')"
              ".forEach(el => {"
                "el.addEventListener("
                  "'input',"
                  "debounce(dynamicUpdateService.emitEvent, 250)"
                ")"
              "})"
            // clang-format on
            ),

        F("el.textContent = value") } }
  };
}