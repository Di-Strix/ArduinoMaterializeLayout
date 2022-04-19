#include "NormalizeCssModule.h"

MaterializeLayoutModule getNormalizeCssModule()
{
  return {
    {
        NORMALIZE_CSS_HASH,
        NORMALIZE_CSS,
        NORMALIZE_CSS_LENGTH,
    }
  };
}