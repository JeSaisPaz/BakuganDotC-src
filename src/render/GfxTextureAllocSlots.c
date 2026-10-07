// bdc 0x089f7af4 GfxTextureAllocSlots
#include "bdc.h"

/* Gives a texture private GE state slots: clears `singleSlot` (+0xad) and allocates 0x300 bytes (8
   slots of 0x60) into `blocks` (+0xb0), from the low heap end when `fromLow`. */

void GfxTextureAllocSlots(void *tex, bool fromLow)
{
  GfxTexture *t = (GfxTexture *)tex;
  bool prevLow;
  void *p;

  t->singleSlot = 0;
  MemLock();
  prevLow = MemIsAllocFromLow();
  MemSetAllocFromLow(fromLow);
  p = MemAlloc(8 * sizeof(t->inlineBlock), (char *)0, 0);
  MemSetAllocFromLow(prevLow);
  MemUnlock();
  t->blocks = (unsigned char *)p;
}
