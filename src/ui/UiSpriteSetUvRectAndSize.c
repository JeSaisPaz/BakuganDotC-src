// bdc 0x0890bbf8 UiSpriteSetUvRectAndSize
#include "bdc.h"

/* Sets a sprite's UV rectangle to (`u0`, `v0`, `u1`, `v1`) (`UiSpriteSetUvRect`) and its size to
   the rectangle's extent (`UiSpriteSetSize`). No-op for NULL. */

void UiSpriteSetUvRectAndSize(float u0, float v0, float u1, float v1, void *unused, GfxSprite *sprite)

{
  float rect[4];

  if (sprite != (GfxSprite *)0x0) {
    rect[0] = u0;
    rect[1] = v0;
    rect[2] = u1;
    rect[3] = v1;
    UiSpriteSetUvRect(sprite,rect);
    UiSpriteSetSize(u1 - u0,v1 - v0,sprite);
  }
}
