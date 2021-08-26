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

    {
        { MaterializeCssComponent::Button, new ComponentCreator<MaterializeLayoutComponent_t<Button>>() },
        { MaterializeCssComponent::Field, new ComponentCreator<MaterializeLayoutComponent_t<Field>>() },
        { MaterializeCssComponent::Layout, new ComponentCreator<MaterializeLayoutComponent_t<Layout>>() },
        { MaterializeCssComponent::Tab, new ComponentCreator<MaterializeLayoutComponent_t<Tab>>() },
        { MaterializeCssComponent::TabGroup, new ComponentCreator<MaterializeLayoutComponent_t<TabGroup>>() },
        { MaterializeCssComponent::StaticText, new ComponentCreator<MaterializeLayoutComponent_t<StaticText>>() },
        { MaterializeCssComponent::DynamicText, new ComponentCreator<MaterializeLayoutComponent_t<DynamicText>>() },
    },

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