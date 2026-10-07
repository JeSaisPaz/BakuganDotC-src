// bdc 0x089f7930 GfxTextureSetSlotClut
#include "bdc.h"

/* Points GE state slot `slot` of a texture at the CLUT `clut` (CBP `0xb0` / CBW `0xb1` words at
   index `mipCmdIdx + 2`/`+ 3` of the slot's 0x18-word block). If the texture still uses its shared
   inline block (`singleSlot`), first gives it 8 private slots (`GfxTextureAllocSlots`), copies
   the inline block into each and points each slot at consecutive palettes from `clutData` (stride
   0x40 bytes, 0x400 for T8). Returns whether the split happened. */

bool GfxTextureSetSlotClut(void *tex, int slot, u32 clut, bool fromLow)
{
  GfxTexture *t = (GfxTexture *)tex;
  bool split = false;
  int stride;
  int i;
  u32 addr;

  if (t->singleSlot != 0) {
    split = true;
    GfxTextureAllocSlots(t, fromLow);
    stride = 0x40;
    if (GfxTextureGetPsm(t) == 5)
      stride = 0x400;
    for (i = 0; i < 8; i++) {
      memcpy(t->blocks + i * 0x60, t->inlineBlock, 0x60);
      addr = (u32)(uintptr_t)t->clutData + i * stride;
      ((u32 *)t->blocks)[i * 0x18 + t->mipCmdIdx + 2] = ((addr >> 24) & 0xf) << 16 | 0xb1000000;
      addr = (u32)(uintptr_t)t->clutData + i * stride;
      ((u32 *)t->blocks)[i * 0x18 + t->mipCmdIdx + 3] = (addr & 0xffffff) | 0xb0000000;
    }
  }
  ((u32 *)t->blocks)[slot * 0x18 + t->mipCmdIdx + 2] = ((clut >> 24) & 0xf) << 16 | 0xb1000000;
  ((u32 *)t->blocks)[slot * 0x18 + t->mipCmdIdx + 3] = (clut & 0xffffff) | 0xb0000000;
  return split;
}
