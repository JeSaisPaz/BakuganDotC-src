// bdc 0x089f7c60 GfxTextureSetMipSlope
#include "bdc.h"

/* Sets the mipmap slope of a mipmapped texture: stores `slope` in the picture header (`mipSlope`) and
   rewrites the `TSLOPE` word (`0xd0`, float >> 8) at index `mipCmdIdx - 2` of the GE block. */

void GfxTextureSetMipSlope(float slope, void *tex)
{
  GfxTexture *t = (GfxTexture *)tex;

  if (t->picture->mipMapTextures > 1) {
    t->picture->mipSlope = slope;
    ((u32 *)t->blocks)[t->mipCmdIdx - 2] = *(u32 *)&t->picture->mipSlope >> 8 | 0xd0000000;
  }
}
