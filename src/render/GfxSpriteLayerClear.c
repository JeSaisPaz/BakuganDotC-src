// bdc 0x089f5188 GfxSpriteLayerClear
#include "bdc.h"

/* Releases every sprite in the layer's draw list: pool sprites (`slotFlags & 1`) just get their
   in-use bit 1 cleared, heap sprites are deleted through their virtual destructor (slot 1, flags
   3). Then clears the list head/tail/count (`+0x1c/+0x20/+0x24`) and the used-slot count (`+0x14`).
    */

void GfxSpriteLayerClear(GfxSpriteLayer *self)

{
  GfxSprite *cur = self->head;

  if (cur != (GfxSprite *)0x0) {
    GfxSprite *next = cur->next;
    unsigned int flags = cur->slotFlags;

    while (1) {
      if ((flags & 1) == 0) {
        if (cur != (GfxSprite *)0x0) {
          const VtblEntry *dtor = &((const VtblEntry *)cur->vtable)[1];
          ((void (*)(void *, int))dtor->fn)((u8 *)cur + dtor->delta, 3);
        }
      } else {
        cur->slotFlags = flags & ~2u;
      }
      if (next == (GfxSprite *)0x0) {
        break;
      }
      cur = next;
      flags = cur->slotFlags;
      next = cur->next;
    }
  }
  self->tail = (GfxSprite *)0x0;
  self->head = (GfxSprite *)0x0;
  self->count = 0;
  self->poolUsed = 0;
}
