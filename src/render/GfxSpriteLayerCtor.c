// bdc 0x089f4f24 GfxSpriteLayerCtor
#include "bdc.h"

/* Constructor of a sprite layer (the container drawn by `GfxSpriteLayerDraw2D` /
   `GfxSpriteLayerDraw3D`): sets the vtable `0x08af5854` at `+0x74`, stores `is3D` at `+0`, clears
   the sort flag (`+4`), the pool fields (`+8` array, `+0x10` last slot, `+0x14` used count), the
   draw list (`+0x1c` head, `+0x20` tail, `+0x24` count), sets the layer mask (`+0x28`) to 1, the
   view matrix (`+0x30`) to identity via `GfxSpriteLayerResetView`, alpha (`+0x70`) to 1.0, and
   the maximum sorted sprites (`+0x18`) to 1000 for 2D (`is3D == 0`) or 500 for 3D. */

GfxSpriteLayer *GfxSpriteLayerCtor(GfxSpriteLayer *self, s32 is3D)

{
  
  self->vtbl = (const VtblEntry *)&g_gfxSpriteLayerVtbl;
  self->is3D = is3D;
  self->sorted = '\0';
  self->pool = (GfxSprite *)0x0;
  self->poolLast = (GfxSprite *)0x0;
  self->poolUsed = 0;
  self->tail = (GfxSprite *)0x0;
  self->head = (GfxSprite *)0x0;
  self->count = 0;
  GfxSpriteLayerSetLayerMask(self,1);
  GfxSpriteLayerResetView(self);
  s32 maxSorted = 500;
  self->alpha = 1.0f;
  if (is3D == 0) {
    maxSorted = 1000;
  }
  self->maxSorted = maxSorted;
  return self;
}

