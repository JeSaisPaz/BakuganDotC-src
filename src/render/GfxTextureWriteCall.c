// bdc 0x089f70e0 GfxTextureWriteCall
#include "bdc.h"

/* Emits a GE call of the texture's state block for `slot` (`tex->blocks + slot * 0x60`, slot forced to
   0 when `singleSlot` marks the single shared block): `BASE` (0x10, high address bits) and `CALL` (0x0a)
   words. Returns `dl + 2`. */

u32 *GfxTextureWriteCall(void *tex, u32 *dl, int slot)
{
  GfxTexture *t = (GfxTexture *)tex;
  u32 addr;

  if (t->singleSlot != 0) {
    slot = 0;
  }
  addr = (u32)(uintptr_t)(t->blocks + slot * 0x60);
  dl[0] = (addr >> 0x18 & 0xf) << 0x10 | 0x10000000;
  dl[1] = addr & 0xffffff | 0xa000000;
  return dl + 2;
}
