// bdc 0x089f7138 GfxTextureSelectSlot
#include "bdc.h"

/* Makes GE state block `slot` the texture's current one: stores `blocks + slot * 0x60` in `curBlock`
   and `gmo.image` (slot 0 when `singleSlot` is set). */

void GfxTextureSelectSlot(void *tex, int slot)
{
  GfxTexture *t = (GfxTexture *)tex;
  u8 *block;

  if (t->singleSlot != 0) {
    slot = 0;
  }
  block = t->blocks + slot * 0x60;
  t->curBlock = block;
  t->gmo.image = block;
}
