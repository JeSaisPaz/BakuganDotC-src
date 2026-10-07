// bdc 0x089f6c74 GfxGetNullTexture
#include "bdc.h"

/* Returns the fallback `"NonTexture"` texture (`g_nullTexture`), creating it on first use via
   `GfxInitNullTexture`. */

void *GfxGetNullTexture(void)

{
  if (g_nullTexture == (void *)0x0) {
    GfxInitNullTexture();
  }
  return g_nullTexture;
}

