// bdc 0x089ee580 UiSpriteMngRemove
#include "bdc.h"

/* Releases the sprite in `slot` (`UiSpriteLayerRelease`), clears the slot and decrements the count. Always
   returns 1. `ScriptOpSprite` cmd 0. */

int UiSpriteMngRemove(UiSpriteMng *self, int slot)

{
  if (self->layer != (void *)0x0) {
    if (self->sprites[slot] != (GfxSprite *)0x0) {
      UiSpriteLayerRelease(self->layer,self->sprites[slot]);
      self->sprites[slot] = (GfxSprite *)0x0;
      self->count = self->count + -1;
    }
  }
  return 1;
}

