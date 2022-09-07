#include "MaterializeCssModule.h"

MaterializeLayoutModule getMaterializeCssModule()
{
  return {
    { MATERIALIZE_CSS_HASH, MATERIALIZE_CSS, MATERIALIZE_CSS_LENGTH },
    { MATERIALIZE_JS_HASH, MATERIALIZE_JS, MATERIALIZE_JS_LENGTH },
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

        F(
            // clang-format off
            "const data = JSON.parse(value);"
            "if (data.styles)"
                "Object.entries(data.styles).forEach(([key, value]) => el.style.setProperty(key, value));"
            "if (data.atributes)"
                "Object.entries(data.attributes).forEach(([key, value]) => el.setAttribute(key, value));"
            "if(Array.isArray(data.classListDiff))"
                "data.classListDiff.forEach(diff => {"
                    "switch(diff.changeType){"
                      "case 'add':"
                        "el.classList.add(diff.className);"
                        "break;"
                      "case 'remove':"
                        "el.classList.remove(diff.className);"
                        "break;"
                      "case 'replace':"
                        "el.classList.replace(diff.className, diff.newClassName);"
                        "break;"
                    "}"
                  "});"
            "if (data.textContent && el.textContent !== data.textContent) el.textContent = data.textContent;"
            // clang-format on
            ) } }
  };
}