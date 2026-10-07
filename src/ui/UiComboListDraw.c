// bdc 0x089b30a4 UiComboListDraw
#include "bdc.h"

/* Draw (vtable slot 4) of the combo list screen (task id 3002): submits the sprite layer scaled by
   `+0x7c/+0x80` (`GfxSpriteLayerSetZoom`). */

void UiComboListDraw(UiComboList *self)

{
  void *packet;
  GfxSpriteLayer *layer;
  
  packet = GfxNewRenderPacket(700.0f);
  layer = (self->base).spriteLayer;
  if (layer != (GfxSpriteLayer *)0x0) {
    GfxSpriteLayerSetZoom(self->zoom,self->zoomY,layer,(float *)0x0);
    GfxSpriteLayerDraw((self->base).spriteLayer,packet);
  }
  return;
}

