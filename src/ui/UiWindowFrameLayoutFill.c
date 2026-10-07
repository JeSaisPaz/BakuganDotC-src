// bdc 0x089ff4c4 UiWindowFrameLayoutFill
#include "bdc.h"

/* Positions and sizes the fill sprite (`fill`, `+0xac`) of a 9-slice window frame (`UiWindowFrame`,
   0x130 bytes: a `GfxSpriteLayer` with vtable `0x08af5954` at `+0x74`, plus a
   `CoreObject` at `+0x80`): clamps the frame size `rect[2..3]` to 0..480 x 0..272 (NaN clamps to
   the maximum) and rounds the integer size up to even like `UiWindowFrameLayout`, then places
   the sprite at the frame position minus (half size - `fillInset`) scaled by the open `progress`,
   depth + 10, sizes it to (size + inset[2..3]) * progress (`UiSpriteSetSize`) and sets its UVs
   by `fillMode`: 0 = a 2x2 texel cell at `fillParams[0..1]` * 4 + 1, 1 = origin `fillParams[0..1]`
   and the sprite size scaled by `fillParams[2..3]`, other modes leave the UVs. */

void UiWindowFrameLayoutFill(UiWindowFrame *self)

{
  GfxSprite *sprite;
  float uvTile[4] __attribute__((aligned(16)));
  float uvScaled[4] __attribute__((aligned(16)));
  float w;
  float h;
  float u;
  float v;
  float uw;
  int iw;
  int ih;
  int mode;

  w = self->rect[2];
  if (w < 0.0f) {
    w = 0.0f;
  } else if (!(w <= 480.0f)) {
    w = 480.0f;
  }
  iw = (int)w;
  self->rect[2] = w;
  h = self->rect[3];
  if (h < 0.0f) {
    h = 0.0f;
  } else if (!(h <= 272.0f)) {
    h = 272.0f;
  }
  ih = (int)h;
  self->rect[3] = h;
  if (iw % 2 != 0) {
    iw = iw + 1;
  }
  if (ih % 2 != 0) {
    ih = ih + 1;
  }
  sprite = self->fill;
  sprite->posX = self->rect[0] - ((float)iw * 0.5f - self->fillInset[0]) * self->progress;
  sprite->posW = 0.0f;
  sprite->posY = self->rect[1] - ((float)ih * 0.5f - self->fillInset[1]) * self->progress;
  sprite->posZ = self->depth + 10.0f;
  UiSpriteSetSize(((float)iw + self->fillInset[2]) * self->progress,
                  ((float)ih + self->fillInset[3]) * self->progress, self->fill);
  mode = self->fillMode;
  if (mode == 0) {
    uvTile[0] = self->fillParams[0] * 4.0f + 1.0f;
    uvTile[1] = self->fillParams[1] * 4.0f + 1.0f;
    uvTile[2] = 2.0f;
    uvTile[3] = 2.0f;
    GfxSpriteSetUvRectXYWH(self->fill, uvTile);
  } else if (mode == 1) {
    sprite = self->fill;
    u = self->fillParams[0];
    v = self->fillParams[1];
    uw = GfxSpriteGetWidth(sprite) * self->fillParams[2];
    uvScaled[3] = GfxSpriteGetHeight(self->fill) * self->fillParams[3];
    uvScaled[0] = u;
    uvScaled[1] = v;
    uvScaled[2] = uw;
    GfxSpriteSetUvRectXYWH(sprite, uvScaled);
  }
}
