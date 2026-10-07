// bdc 0x089f4200 UiSpriteAddUvRect
#include "bdc.h"

/* Offsets (scrolls) the sprite's UV rectangle: adds `delta[0..3]` (x0, y0, x1, y1 in texels) scaled
   by the texture's per-texel factors (`tex+0xa4` for x, `tex+0xa8` for y) to the four UV pairs of
   the quad data at `sprite+0x120` (indices 0/0x18, 4/0x10, 0xc/0x24, 0x1c/0x28). Switches the
   sprite to quad mode first (`GfxSpriteSetQuadMode`) when its mode `+0xe4` < 2. Counterpart of
   `UiSpriteSetUvRect`; used by `UiSpriteMngMoveAndOffsetUv` and `UiSpriteMngOffsetUv`. */

void UiSpriteAddUvRect(void *sprite, const float *delta)

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
    f = (spr->vertices + 2)->u + delta[0] * ((GfxTexture *)spr->texture)->invWidth;
    (spr->vertices + 2)->u = f;
    (spr->vertices + 0)->u = f;
    f = (spr->vertices + 3)->u + delta[2] * ((GfxTexture *)spr->texture)->invWidth;
    (spr->vertices + 3)->u = f;
    (spr->vertices + 1)->u = f;
    f = (spr->vertices + 1)->v + delta[1] * ((GfxTexture *)spr->texture)->invHeight;
    (spr->vertices + 1)->v = f;
    (spr->vertices + 0)->v = f;
    f = (spr->vertices + 3)->v + delta[3] * ((GfxTexture *)spr->texture)->invHeight;
    (spr->vertices + 3)->v = f;
    (spr->vertices + 2)->v = f;
  }
}
