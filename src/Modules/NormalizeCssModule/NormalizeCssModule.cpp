#include "NormalizeCssModule.h"

MaterializeLayoutModule getNormalizeCssModule()
{
  return {
      "normalize",
      NORMALIZE_CSS,
      NORMALIZE_CSS_LENGTH,
  };
}