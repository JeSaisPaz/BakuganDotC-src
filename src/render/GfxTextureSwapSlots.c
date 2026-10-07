// bdc 0x089f71e8 GfxTextureSwapSlots
#include "bdc.h"

/* Swaps the 0x60-byte GE state blocks `a` and `b` of a texture with private slots (no-op when
   `singleSlot` (+0xad) marks the shared single block). */

void GfxTextureSwapSlots(void *tex, int a, int b)
{
  GfxTexture *t = (GfxTexture *)tex;
  unsigned char tmp[0x60];

  if (t->singleSlot == 0) {
    memcpy(tmp, t->blocks + a * 0x60, 0x60);
    memcpy(t->blocks + a * 0x60, t->blocks + b * 0x60, 0x60);
    memcpy(t->blocks + b * 0x60, tmp, 0x60);
  }
}
