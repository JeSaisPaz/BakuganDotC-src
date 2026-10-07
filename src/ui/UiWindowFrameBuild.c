// bdc 0x089feed4 UiWindowFrameBuild
#include "bdc.h"

/* Creates the sprites of a 9-slice window frame (`UiWindowFrame`, 0x130 bytes: a
   `GfxSpriteLayer` with vtable `0x08af5954` at `+0x74`, plus a `CoreObject`
   at `+0x80`) from style record `style` (`UiWindowFrameStyle`): border texture by name
   (`borderName`), border colour and fill colour; 8 centred, linearly filtered border sprites
   (`GfxSpriteLayerCreateSprite`, `GfxSpriteCenterPivot`) tinted with the border colour and
   using `borderSlot`, their 9-slice UVs (`UiWindowFrameSetBorderUvs`), then the fill texture
   (`fillName`), fill params, fill sprite (tinted, `fillSlot`), fill mode and inset; finally marks
   the layer sorted and sets frame flag bit 0. The sprite tint copies are 4-float quads: rgb into
   `tint` and lane 3 into the sprite's `alpha`. */

void UiWindowFrameBuild(UiWindowFrame *self, char *style)

{
  const UiWindowFrameStyle *st = (const UiWindowFrameStyle *)style;
  float fillPos[4];
  float borderPos[4];
  GfxSprite *sprite;
  GfxSprite *fill;
  int i;
  int k;

  self->borderTexture = GfxFindTexture(st->borderName);
  for (k = 0; k < 4; k++) {
    self->borderColor[k] = st->borderColor[k];
  }
  for (k = 0; k < 4; k++) {
    self->fillColor[k] = st->fillColor[k];
  }
  for (i = 0; i < 8; i++) {
    borderPos[2] = 0.0f;
    borderPos[1] = 0.0f;
    borderPos[0] = 0.0f;
    borderPos[3] = 0.0f;
    sprite = GfxSpriteLayerCreateSprite((GfxSpriteLayer *)self, self->borderTexture, borderPos,
                                        false);
    sprite->tint[0] = self->borderColor[0];
    sprite->tint[1] = self->borderColor[1];
    sprite->tint[2] = self->borderColor[2];
    sprite->alpha = self->borderColor[3];
    GfxSpriteCenterPivot(sprite);
    sprite->flags |= 0x20;
    sprite->textureSlot = st->borderSlot;
  }
  UiWindowFrameSetBorderUvs(self);
  self->fillTexture = GfxFindTexture(st->fillName);
  for (k = 0; k < 4; k++) {
    self->fillParams[k] = st->fillParams[k];
  }
  fillPos[2] = 0.0f;
  fillPos[1] = 0.0f;
  fillPos[0] = 0.0f;
  fillPos[3] = 0.0f;
  fill = GfxSpriteLayerCreateSprite((GfxSpriteLayer *)self, self->fillTexture, fillPos, false);
  self->fill = fill;
  fill->tint[0] = self->fillColor[0];
  fill->tint[1] = self->fillColor[1];
  fill->tint[2] = self->fillColor[2];
  fill->alpha = self->fillColor[3];
  self->fill->flags |= 0x20;
  self->fill->textureSlot = st->fillSlot;
  self->fillMode = st->fillMode;
  for (k = 0; k < 4; k++) {
    self->fillInset[k] = st->fillInset[k];
  }
  ((GfxSpriteLayer *)self)->sorted = 1;
  self->flags |= 1;
}
