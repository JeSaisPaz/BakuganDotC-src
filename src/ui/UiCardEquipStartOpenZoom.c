// bdc 0x0896b7dc UiCardEquipStartOpenZoom
#include "bdc.h"

/* Resets the sprite-layer zoom of `UiCardEquip` (`+0x29c4..0x29d0`) for opening
   (`closing` = 0, scale 0) or closing (scale 1) and applies scale 0 to the layer
   (`GfxSpriteLayerSetZoom`). */

void UiCardEquipStartOpenZoom(UiCardEquip *self, u8 closing)
{
  float scale;

  if (closing == 0) {
    scale = 0.0f;
    self->zoomTarget = scale;
    self->zoomScale = scale;
    self->zoomStep = scale;
    self->zoomDone = 0;
  } else {
    scale = 1.0f;
    self->zoomStep = 0.0f;
    self->zoomTarget = scale;
    self->zoomScale = scale;
    self->zoomDone = 0;
  }
  GfxSpriteLayerSetZoom(scale, 0.0f, self->base.spriteLayer, (float *)0);
}
