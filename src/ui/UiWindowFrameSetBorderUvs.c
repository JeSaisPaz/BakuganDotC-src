// bdc 0x089fed78 UiWindowFrameSetBorderUvs
#include "bdc.h"

/* Splits the border texture of a 9-slice window frame (`UiWindowFrame`, 0x130 bytes: a
   `GfxSpriteLayer` with vtable `0x08af5954` at `+0x74`, plus a `CoreObject`
   at `+0x80`) into 9-slice pieces: for the 8 border sprites (corners and edges) sets UV rectangles
   (`GfxSpriteSetUvRectXYWH`) from the texture's half width/height minus 1 texel, with 2-texel
   strips for the edges. Sprite order: top-left, top, top-right, left, right, bottom-left, bottom,
   bottom-right. */

void UiWindowFrameSetBorderUvs(UiWindowFrame *self)

{
  float rects[8][4];
  GfxSprite *sprite;
  float *rect;
  int i;
  float halfW;
  float rightX;
  float halfH;
  float bottomY;

  halfW = (float)GfxTextureGetWidth(self->borderTexture) * 0.5f - 1.0f;
  rightX = halfW + 2.0f;
  halfH = (float)GfxTextureGetHeight(self->borderTexture) * 0.5f - 1.0f;
  bottomY = halfH + 2.0f;

  rects[0][0] = 0.0f;   rects[0][1] = 0.0f;    rects[0][2] = halfW; rects[0][3] = halfH;
  rects[1][0] = halfW;  rects[1][1] = 0.0f;    rects[1][2] = 2.0f;  rects[1][3] = halfH;
  rects[2][0] = rightX; rects[2][1] = 0.0f;    rects[2][2] = halfW; rects[2][3] = halfH;
  rects[3][0] = 0.0f;   rects[3][1] = halfH;   rects[3][2] = halfW; rects[3][3] = 2.0f;
  rects[4][0] = rightX; rects[4][1] = halfH;   rects[4][2] = halfW; rects[4][3] = 2.0f;
  rects[5][0] = 0.0f;   rects[5][1] = bottomY; rects[5][2] = halfW; rects[5][3] = halfH;
  rects[6][0] = halfW;  rects[6][1] = bottomY; rects[6][2] = 2.0f;  rects[6][3] = halfH;
  rects[7][0] = rightX; rects[7][1] = bottomY; rects[7][2] = halfW; rects[7][3] = halfH;

  sprite = ((GfxSpriteLayer *)self)->head;
  rect = rects[0];
  i = 0;
  do {
    GfxSpriteSetUvRectXYWH(sprite, rect);
    i = i + 1;
    sprite = sprite->next;
    rect = rect + 4;
  } while (i < 8);
}
