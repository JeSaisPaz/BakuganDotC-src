// bdc 0x089f7b68 GfxTextureFreeSlots
#include "bdc.h"

/* Frees a texture's private GE state slots (`blocks`) and returns it to the inline single block at
   `+0xb4` (`singleSlot` = 1). No-op when already shared. */

void GfxTextureFreeSlots(void *tex)
{
  GfxTexture *t = (GfxTexture *)tex;
  u8 *ptr;

  if (t->singleSlot == 0) {
    ptr = t->blocks;
    t->singleSlot = 1;
    if (ptr != (u8 *)0) {
      MemLock();
      MemFree(ptr, (const char *)0, 0);
      MemUnlock();
      t->blocks = (u8 *)0;
    }
    t->blocks = t->inlineBlock;
  }
}
