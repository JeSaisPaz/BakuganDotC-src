// bdc 0x089e1078 GfxSetTextureSlotB
#include "bdc.h"

/* Stores `tex` into slot `slot` of the global pointer table at `g_gmoStateTextures` +7. Used by
   `ScriptOpTextureCmd` cmd 1. Sibling: `GfxSetTextureSlotA`. */

void GfxSetTextureSlotB(void *tex, int slot)

{
  g_gmoStateTextures[slot + 7] = tex;
  return;
}

