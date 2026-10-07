// bdc 0x089f50f4 UiSpriteLayerRelease
#include "bdc.h"

/* Releases a sprite from its layer: if bit 0 of its `slotFlags` is set, bit 1 is cleared, the
   sprite is unlinked (`CoreObjectUnlink`) and the layer's `poolUsed` count decremented;
   otherwise the sprite's virtual destructor (vtable slot 1, arg 3) runs. NULL sprite is a no-op.
   Used by `UiSpriteMngRemove` / `UiSpriteMngAdd`. */

void UiSpriteLayerRelease(void *layer, void *sprite)

{
  GfxSprite *spr = (GfxSprite *)sprite;

  if (spr != (GfxSprite *)0x0) {
    if ((spr->slotFlags & 1) == 0) {
      const VtblEntry *dtor = &((const VtblEntry *)spr->vtable)[1];
      ((void (*)(void *, int))dtor->fn)((u8 *)spr + dtor->delta, 3);
    }
    else {
      spr->slotFlags = spr->slotFlags & ~2u;
      CoreObjectUnlink((CoreObject *)spr);
      ((GfxSpriteLayer *)layer)->poolUsed = ((GfxSpriteLayer *)layer)->poolUsed - 1;
    }
  }
}
