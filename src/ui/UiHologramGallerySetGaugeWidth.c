// bdc 0x0891d53c UiHologramGallerySetGaugeWidth
#include "bdc.h"

/* Sizes a gauge sprite to `18 + 9 * n` x 16 pixels (UV rectangle and size). */

void UiHologramGallerySetGaugeWidth(UiHologramGallery *self, GfxSprite *sprite, u32 n)
{
  float w = (float)((n & 0xff) * 9 + 0x12);
  float rect[4];

  rect[0] = 0.0f;
  rect[1] = 0.0f;
  rect[2] = w;
  rect[3] = 16.0f;
  GfxSpriteSetUvRectXYWH(sprite, rect);
  UiSpriteSetSize(w, 16.0f, sprite);
}
