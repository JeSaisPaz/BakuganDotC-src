// bdc 0x089f4120 UiSpriteSetUvRect
#include "bdc.h"

/* Sets the sprite's UV rectangle from `rect[0..3]` (x0, y0, x1, y1 in texels), scaled by the
   texture's factors (`invWidth` / `invHeight`) and stored into the quad vertices (`vertices`).
   Switches the sprite to quad mode first (`GfxSpriteSetQuadMode(sprite, mode+2)`) if its mode
   (`quadMode`) is < 2. Does nothing for modes outside 2..4. */

void UiSpriteSetUvRect(void *sprite, const float *rect)

{
  GfxSprite *spr = (GfxSprite *)sprite;
  int mode;
  float f;

  mode = spr->quadMode;
  if (mode < 2) {
    GfxSpriteSetQuadMode(spr,mode + 2);
    mode = spr->quadMode;
  }
  if ((1 < mode) && (mode < 5)) {
    f = rect[0] * ((GfxTexture *)spr->texture)->invWidth;
    (spr->vertices + 2)->u = f;
    (spr->vertices + 0)->u = f;
    f = rect[2] * ((GfxTexture *)spr->texture)->invWidth;
    (spr->vertices + 3)->u = f;
    (spr->vertices + 1)->u = f;
    f = rect[1] * ((GfxTexture *)spr->texture)->invHeight;
    (spr->vertices + 1)->v = f;
    (spr->vertices + 0)->v = f;
    f = rect[3] * ((GfxTexture *)spr->texture)->invHeight;
    (spr->vertices + 3)->v = f;
    (spr->vertices + 2)->v = f;
  }
}
