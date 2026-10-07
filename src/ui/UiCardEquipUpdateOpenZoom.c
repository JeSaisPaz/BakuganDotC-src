// bdc 0x0896b838 UiCardEquipUpdateOpenZoom
#include "bdc.h"

/* Advances the sprite-layer zoom of `UiCardEquip` by 1/8 per frame: opening
   overshoots to 1.2 then settles to 1.0; closing shrinks linearly to 0. Returns 1 when finished. */

u8 UiCardEquipUpdateOpenZoom(UiCardEquip *self, u8 closing)
{
  float base = self->zoomTarget;
  float t = self->zoomStep + 0.125f;
  u8 finished = 0;
  GfxSpriteLayer *layer = self->base.spriteLayer;
  float scale;

  if (!closing) {
    if (self->zoomDone == 0) {
      /* stage 0: ease out from the base scale up to base + 1.2 */
      self->zoomStep = t;
      scale = base + (1.0f - (t - 1.0f) * (t - 1.0f)) * 1.2f;
      self->zoomScale = scale;
      GfxSpriteLayerSetZoom(scale, 0.0f, layer, NULL);
      if (!(self->zoomStep < 1.0f)) {
        GfxSpriteLayerSetZoom(1.2f, 0.0f, self->base.spriteLayer, NULL);
        self->zoomTarget = self->zoomScale;
        self->zoomStep = 0.0f;
        self->zoomDone++;
      }
    } else {
      /* stage 1: settle back by 0.2 */
      self->zoomStep = t;
      scale = base - t * t * 0.20000005f;
      self->zoomScale = scale;
      GfxSpriteLayerSetZoom(scale, 0.0f, layer, NULL);
      if (!(self->zoomStep < 1.0f)) {
        GfxSpriteLayerSetZoom(1.0f, 0.0f, self->base.spriteLayer, NULL);
        finished = 1;
      }
    }
  } else {
    self->zoomStep = t;
    scale = base - t;
    self->zoomScale = scale;
    GfxSpriteLayerSetZoom(scale, 0.0f, layer, NULL);
    if (!(self->zoomStep < 1.0f)) {
      GfxSpriteLayerSetZoom(0.0f, 0.0f, self->base.spriteLayer, NULL);
      finished = 1;
    }
  }
  return finished != 0;
}
