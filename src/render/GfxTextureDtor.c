// bdc 0x089f6d1c GfxTextureDtor
#include "bdc.h"

/* Destructor of a texture object (`g_gfxTextureVtbl` slot 1): unlinks it when it is the head of
   `g_textureList`, releases its two VRAM blocks (`vramBlock0`/`vramBlock1`) to the display
   allocator, frees the owned TIM2 data (`tim2`, when `ownsTim2` is set) and its private GE state
   slots (`blocks`, unless `singleSlot` marks the inline block), runs the `CoreObject` base dtor
   and frees the object when `flags & 1`. */

void GfxTextureDtor(void *tex, u32 flags)
{
  GfxTexture *t = (GfxTexture *)tex;
  void *p;

  if (t == NULL)
    return;
  t->vtbl = g_gfxTextureVtbl;
  if (g_textureList == t)
    g_textureList = t->next;
  GfxDisplayVramFree(g_gfxDisplay, t->vramBlock0);
  GfxDisplayVramFree(g_gfxDisplay, t->vramBlock1);
  if (t->ownsTim2 != 0 && t->tim2 != NULL) {
    p = t->tim2;
    MemLock();
    MemFree(p, (char *)0, 0);
    MemUnlock();
    t->tim2 = NULL;
  }
  if (t->singleSlot == 0 && t->blocks != NULL) {
    p = t->blocks;
    MemLock();
    MemFree(p, (char *)0, 0);
    MemUnlock();
    t->blocks = NULL;
  }
  CoreObjectDtor((CoreObject *)t, 0);
  if ((flags & 1) != 0) {
    MemLock();
    MemFree(t, (char *)0, 0);
    MemUnlock();
  }
}
