// bdc 0x089f74d4 GfxTextureRebuild
#include "bdc.h"

/* Rebuilds a texture's GE command block (`GfxTextureBuildDl`) from its power-of-two rounded width
   and height. */

void GfxTextureRebuild(void *tex)

{
  u32 w;
  u32 h;
  
  w = GfxTextureGetWidth(tex);
  w = GfxTextureRoundPow2(tex,w);
  h = GfxTextureGetHeight(tex);
  h = GfxTextureRoundPow2(tex,h);
  GfxTextureBuildDl(tex,w,h);
  return;
}

