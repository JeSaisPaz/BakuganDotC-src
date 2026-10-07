// bdc 0x089f7580 GfxTextureReleaseVram
#include "bdc.h"

/* Returns the texture's VRAM blocks (`+0x114`, `+0x118`) to the display allocator (`GfxDisplayVramFree`),
   clears them and rebuilds the GE block from main memory (`GfxTextureBuildDl`). */

void GfxTextureReleaseVram(void *tex)
{
  GfxTexture *t = (GfxTexture *)tex;
  u32 w;
  u32 h;

  GfxDisplayVramFree(g_gfxDisplay, t->vramBlock0);
  GfxDisplayVramFree(g_gfxDisplay, t->vramBlock1);
  t->vramBlock1 = 0;
  t->vramBlock0 = 0;
  w = GfxTextureRoundPow2(tex, GfxTextureGetWidth(tex));
  h = GfxTextureRoundPow2(tex, GfxTextureGetHeight(tex));
  GfxTextureBuildDl(tex, w, h);
}
