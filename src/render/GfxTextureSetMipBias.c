// bdc 0x089f7bd4 GfxTextureSetMipBias
#include "bdc.h"

/* Sets the mipmap level bias of a mipmapped texture (TIM2 mip count > 1): stores `bias` in the
   picture header (`mipBias`), and rewrites the `TEXLEVEL` word (`0xc8`, bias * 16 clamped to s8,
   mode from header byte `+0x25`) at index `mipCmdIdx - 1` of the GE block. */

void GfxTextureSetMipBias(float bias, void *tex)
{
  GfxTexture *t = (GfxTexture *)tex;
  int v;
  int c;

  if (t->picture->mipMapTextures > 1) {
    t->picture->mipBias = bias;
    v = (int)(t->picture->mipBias * 16.0f);
    c = 0x7f;
    if (v < 0x80) {
      c = v;
      if (v < -0x80) {
        c = -0x80;
      }
    }
    ((u32 *)t->blocks)[t->mipCmdIdx - 1] =
        ((u32)c & 0xff) << 0x10 | 0xc8000000 | t->picture->gsTex1[5];
  }
}
