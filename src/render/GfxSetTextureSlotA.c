// bdc 0x089e1060 GfxSetTextureSlotA
#include "bdc.h"

/* Stores `tex` into slot `slot` of the first table in `g_gmoStateTextures`. Used by
   `ScriptOpTextureCmd` cmd 0 (texture looked up by name). Sibling table: `GfxSetTextureSlotB`. */

void GfxSetTextureSlotA(void *tex, int slot)
{
  g_gmoStateTextures[slot] = tex;
}
