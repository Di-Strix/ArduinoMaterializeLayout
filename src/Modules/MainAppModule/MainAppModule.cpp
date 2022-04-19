#include "MainAppModule.h"

MaterializeLayoutModule getMainAppModule()
{
  return {
    {},
    { MAIN_APP_JS_HASH, MAIN_APP_JS, MAIN_APP_JS_LENGTH }
  };
}