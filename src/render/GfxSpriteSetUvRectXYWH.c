// bdc 0x089f4030 GfxSpriteSetUvRectXYWH
#include "bdc.h"

/* Sets the sprite's UV rectangle from `rect = {x, y, w, h}` in texels: like `UiSpriteSetUvRect`
   but with width/height instead of the second corner. Scales by the texture's `1/width`
   (`tex+0xa4`) and `1/height` (`tex+0xa8`) and writes u0/u1/v0/v1 into the four vertices; moves
   modes 0/1 into their own buffer (mode + 2) first, does nothing for modes outside 2..4. */

void GfxSpriteSetUvRectXYWH(GfxSprite *sprite, const float *rect)
{
  int mode = sprite->quadMode;
  GfxTexture *tex;
  float u;

  if (mode < 2) {
    GfxSpriteSetQuadMode(sprite, mode + 2);
    mode = sprite->quadMode;
  }
  if ((1 < mode) && (mode < 5)) {
    tex = (GfxTexture *)sprite->texture;
    u = rect[0] * tex->invWidth;
    (*(GfxSpriteVertex(*)[4])sprite->vertices)[2].u = u;
    (*(GfxSpriteVertex(*)[4])sprite->vertices)[0].u = u;
    tex = (GfxTexture *)sprite->texture;
    u = (rect[0] + rect[2]) * tex->invWidth;
    (*(GfxSpriteVertex(*)[4])sprite->vertices)[3].u = u;
    (*(GfxSpriteVertex(*)[4])sprite->vertices)[1].u = u;
    tex = (GfxTexture *)sprite->texture;
    u = rect[1] * tex->invHeight;
    (*(GfxSpriteVertex(*)[4])sprite->vertices)[1].v = u;
    (*(GfxSpriteVertex(*)[4])sprite->vertices)[0].v = u;
    tex = (GfxTexture *)sprite->texture;
    u = (rect[1] + rect[3]) * tex->invHeight;
    (*(GfxSpriteVertex(*)[4])sprite->vertices)[3].v = u;
    (*(GfxSpriteVertex(*)[4])sprite->vertices)[2].v = u;
  }
}
