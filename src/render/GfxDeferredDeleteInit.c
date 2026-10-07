// bdc 0x089f8a70 GfxDeferredDeleteInit
#include "bdc.h"

/* Clears the deferred-delete object list (`0x08b02c70` head, `0x08b02c74` tail, `0x08b02c78`
   count). Called by `GfxRenderInit`. */

void GfxDeferredDeleteInit(void)

{
  g_gfxDeferredDeleteTail = (CoreObject *)0x0;
  g_gfxDeferredDeleteHead = (CoreObject *)0x0;
  g_gfxDeferredDeleteCount = 0;
  return;
}

