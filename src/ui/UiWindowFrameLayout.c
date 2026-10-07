// bdc 0x089ff1e4 UiWindowFrameLayout
#include "bdc.h"

/* Layout (vtable `+0x3c`) of a 9-slice window frame (`UiWindowFrame`, 0x130 bytes: a
   `GfxSpriteLayer` with vtable `0x08af5954` at `+0x74`, plus a `CoreObject`
   at `+0x80`): clamps the size (`+0xb8`, `+0xbc`) to the 480x272 screen (NaN clamps to the maximum) and rounds it up to even
   pixels, lays out the fill sprite (`UiWindowFrameLayoutFill`) and places and sizes the 8 border
   sprites around the rectangle (corner offsets of `(size + 31) / 2`), scaled by the open progress, then calls vtable slot 6 (`+0x30`).
    */

void UiWindowFrameLayout(UiWindowFrame *self)

{
  GfxSprite *sprite;
  const VtblEntry *vt;
  float offset[16];
  float fw;
  float fh;
  float halfW;
  float halfH;
  float w;
  float h;
  int iw;
  int ih;
  int i;

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
  UiWindowFrameLayoutFill(self);
  sprite = ((GfxSpriteLayer *)self)->head;
  fw = (float)iw;
  fh = (float)ih;
  halfW = (fw + 31.0f) * 0.5f;
  halfH = (fh + 31.0f) * 0.5f;
  for (i = 0; i < 8; i++) {
    /* x offsets of the 8 border sprites (corners and edges), then their y offsets */
    offset[0] = -halfW;
    offset[1] = 0.0f;
    offset[2] = halfW;
    offset[3] = -halfW;
    offset[4] = halfW;
    offset[5] = -halfW;
    offset[6] = 0.0f;
    offset[7] = halfW;
    offset[8] = -halfH;
    offset[9] = -halfH;
    offset[10] = -halfH;
    offset[11] = 0.0f;
    offset[12] = 0.0f;
    offset[13] = halfH;
    offset[14] = halfH;
    offset[15] = halfH;
    w = 31.0f;
    h = 31.0f;
    if (i == 1 || i == 6) {
      w = fw;
    }
    if (i == 3 || i == 4) {
      h = fh;
    }
    sprite->posX = self->rect[0] + offset[i] * self->progress;
    sprite->posY = self->rect[1] + offset[i + 8] * self->progress;
    sprite->posZ = self->depth;
    sprite->posW = 0.0f;
    GfxSpriteSetSize(sprite, w * self->progress, h * self->progress);
    sprite = sprite->next;
  }
  vt = &((GfxSpriteLayer *)self)->vtbl[6];
  ((void (*)(void *))vt->fn)((u8 *)self + vt->delta);
}
