// bdc 0x089f4300 GfxSpriteSetUvFull
#include "bdc.h"

/* Sets the sprite's UVs to the whole texture: u from 0 to `texture width * (1/width)` and v from 0
   to `height * (1/height)`, using the texture's pixel size getters `GfxTextureGetWidth`/`GfxTextureGetHeight`
   and its reciprocal factors at `tex+0xa4`/`tex+0xa8`. */

void GfxSpriteSetUvFull(GfxSprite *sprite)

{
  GfxSpriteVertex (*v)[4] = (GfxSpriteVertex (*)[4])sprite->vertices;
  float f;

  (*v)[2].u = 0.0f;
  (*v)[0].u = 0.0f;
  f = ((GfxTexture *)sprite->texture)->invWidth;
  f = (float)GfxTextureGetWidth(sprite->texture) * f;
  v = (GfxSpriteVertex (*)[4])sprite->vertices;
  (*v)[3].u = f;
  (*v)[1].u = f;
  v = (GfxSpriteVertex (*)[4])sprite->vertices;
  (*v)[1].v = 0.0f;
  (*v)[0].v = 0.0f;
  f = ((GfxTexture *)sprite->texture)->invHeight;
  f = (float)GfxTextureGetHeight(sprite->texture) * f;
  v = (GfxSpriteVertex (*)[4])sprite->vertices;
  (*v)[3].v = f;
  (*v)[2].v = f;
}
