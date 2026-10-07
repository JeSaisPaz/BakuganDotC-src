// bdc 0x089ee42c UiSpriteMngTaskDraw
#include "bdc.h"

/* Draw method of the sprite manager task: when visible (`+0x18`), non-empty (`+0x1c`) and it has a
   sprite layer (`+0x10`), draws it (`GfxSpriteLayerDraw`) into a render packet at depth `+0x20`. */

void UiSpriteMngTaskDraw(UiSpriteMng *self)

{
  void *packet;
  
  if (((self->visible != '\0') && (0 < self->count)) && (self->layer != (void *)0x0)) {
    packet = GfxNewRenderPacket(self->depth);
    GfxSpriteLayerDraw(self->layer,packet);
  }
  return;
}

