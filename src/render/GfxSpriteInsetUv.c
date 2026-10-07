// bdc 0x089f43a4 GfxSpriteInsetUv
#include "bdc.h"

/* Shrinks the sprite's UV rectangle by `inset` texels on every side (u0 += inset/w, u1 -= inset/w,
   same for v), keeping the duplicated corner values in sync. No-op for the shared-template modes
   0/1. */

void GfxSpriteInsetUv(float inset, GfxSprite *sprite)

{
  if (!(sprite->quadMode >= 0 && sprite->quadMode < 2)) {
    float *tex = (float *)sprite->texture;
    GfxSpriteVertex (*q)[4] = (GfxSpriteVertex (*)[4])sprite->vertices;

    (*q)[2].u = (*q)[2].u + inset * tex[0xa4 / 4];
    (*q)[0].u = (*q)[2].u;
    (*q)[3].u = (*q)[3].u - inset * tex[0xa4 / 4];
    (*q)[1].u = (*q)[3].u;
    (*q)[1].v = (*q)[1].v + inset * tex[0xa8 / 4];
    (*q)[0].v = (*q)[1].v;
    (*q)[3].v = (*q)[3].v - inset * tex[0xa8 / 4];
    (*q)[2].v = (*q)[3].v;
  }
}
