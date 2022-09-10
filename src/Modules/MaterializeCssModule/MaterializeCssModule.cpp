#include "MaterializeCssModule.h"

MaterializeLayoutModule getMaterializeCssModule()
{
  return {
    { MATERIALIZE_CSS_HASH, MATERIALIZE_CSS, MATERIALIZE_CSS_LENGTH },
    { MATERIALIZE_JS_HASH, MATERIALIZE_JS, MATERIALIZE_JS_LENGTH },
    [](ResponseWriter writer) {},
    [](ResponseWriter writer) {},
    { { F("MaterializeCssHandler"),
        [](ResponseWriter writer) {
          // clang-format off
          writer(F("const debounce = (callback, delay) => {"));
          writer(    F("let timeoutId = -1;"));
          writer(    F("return (...props) => {"));
          writer(      F("clearTimeout(timeoutId);"));
          writer(      F("timeoutId = setTimeout(callback.bind(null, ...props), delay);"));
          writer(    F("}"));
          writer(  F("};"));
          
          writer(F("M.AutoInit();"));
          
          writer(F("document"));
          writer(  F(".querySelectorAll('[data-MCSS-emitOnClick=true]')"));
          writer(  F(".forEach(el => {"));
          writer(    F("el.addEventListener('click', dynamicUpdateService.emitEvent)"));
          writer(  F("});"));
          writer(F("document"));
          writer(  F(".querySelectorAll('[data-MCSS-emitOnChange=true]')"));
          writer(  F(".forEach(el => {"));
          writer(    F("el.addEventListener("));
          writer(      F("'input',"));
          writer(      F("debounce(dynamicUpdateService.emitEvent, 250)"));
          writer(    F(")"));
          writer(  F("});"));
          
          writer( F("const el = document.querySelector('[data-MCSS-virtual-router]');"));
          writer( F("if(el) {"));
          writer(   F("const instance = M.Tabs.getInstance(el);"));
          writer(   F("const headers = document"));
          writer(       F(".querySelectorAll('[data-mcss-tabHeader]');"));
          
          writer(   F("if(headers.length) {"));
          writer(     F("const updateMCSSVirtualRouter = (router, e) => {"));
          writer(       F("window.location.href = e.target.hash;"));
          writer(       F("router.select(e.target.hash.substr(1));"));
          writer(       F("const heading = e.target.dataset.mcssVirtualRouterTabName || '';"));
          writer(       F("headers.forEach(el => el.textContent = heading);"));
          writer(       F("e.target.parentNode.parentNode.querySelectorAll('.active')"));
          writer(           F(".forEach(el => el.classList.remove('active'));"));
          writer(       F("e.target.parentNode.classList.add('active');"));
          writer(     F("};"));
          writer(     F("document.querySelectorAll('[href][data-mcss-virtual-router-tab-name]')"));
          writer(         F(".forEach(el => el.addEventListener('click', updateMCSSVirtualRouter.bind(null, instance)));"));
          writer(     F("const selected = document.querySelector(`[href=\"${window.location.hash}\"][data-mcss-virtual-router-tab-name]`)"));
          writer(         F("|| document.querySelector('[data-mcss-virtual-router-tab-name]');"));
          writer(     F("selected.dispatchEvent(new Event('click'));"));
          writer(   F("}"));
          writer(F("}"));
          // clang-format on
        },
        [](ResponseWriter writer) {
          // clang-format off
        writer(F("const data = JSON.parse(value);"));
        writer(    F("if (data.styles)"));
        writer(        F("Object.entries(data.styles).forEach(([key, value]) => el.style.setProperty(key, value));"));
        writer(    F("if (data.atributes)"));
        writer(        F("Object.entries(data.attributes).forEach(([key, value]) => el.setAttribute(key, value));"));
        writer(    F("if(Array.isArray(data.classListDiff))"));
        writer(        F("data.classListDiff.forEach(diff => {"));
        writer(            F("switch(diff.changeType){"));
        writer(              F("case 'add':"));
        writer(                F("el.classList.add(diff.className);"));
        writer(                F("break;"));
        writer(              F("case 'remove':"));
        writer(                F("el.classList.remove(diff.className);"));
        writer(                F("break;"));
        writer(              F("case 'replace':"));
        writer(                F("el.classList.replace(diff.className, diff.newClassName);"));
        writer(                F("break;"));
        writer(            F("}"));
        writer(          F("});"));
        writer(    F("if (data.textContent && el.textContent !== data.textContent) el.textContent = data.textContent;"));
          // clang-format on
        } } }
  };
}