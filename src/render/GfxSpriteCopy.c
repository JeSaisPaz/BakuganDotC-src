// bdc 0x089f3da4 GfxSpriteCopy
#include "bdc.h"

/* Copies sprite `src` into `dst` (both `GfxSprite`): saves `dst`'s list node words
   (`+0x00..0x13`) and `slotFlags` (`+0x124`), runs `GfxSpriteCopyFields`, relinks `dst` into its
   own list (`CoreObjectCopyLinks`/`CoreObjectClearLinks` on a temporary node with the base vtable `0x08af5394`),
   restores `slotFlags`, and re-points `vertices` at `dst`'s own `vertexBuf` for quad modes 2-4
   (re-selecting the template with `GfxSpriteSetQuadMode` for modes 0/1). */

void GfxSpriteCopy(const GfxSprite *src, GfxSprite *dst)

{
  uint mode;
  u32 savedFlags;
  CoreObject tmp;
  
  savedFlags = dst->slotFlags;
  tmp.prev = (CoreObject *)dst->prev;
  tmp.vtable = g_coreObjectVtbl;
  tmp.next = (CoreObject *)dst->next;
  tmp.unk08 = dst->poolIndex;
  tmp.id = dst->serial;
  tmp.list = (CoreObjectList *)dst->list;
  GfxSpriteCopyFields(dst,src);
  CoreObjectCopyLinks(&tmp,(CoreObject *)dst);
  CoreObjectClearLinks(&tmp);
  dst->slotFlags = savedFlags;
  mode = src->quadMode;
  if (mode < 5) {
    if (((mode == 2) || (mode == 3)) || (mode == 4)) {
      dst->vertices = dst->vertexBuf;
    }
    else {
      GfxSpriteSetQuadMode(dst,mode);
    }
  }
  CoreObjectDtor(&tmp,2);
  return;
}

