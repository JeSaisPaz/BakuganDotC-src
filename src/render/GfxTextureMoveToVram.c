// bdc 0x089f7484 GfxTextureMoveToVram
#include "bdc.h"

/* Finds texture `name` (`GfxFindTexture`) and rebuilds its GE state (`GfxTextureRebuild`) with
   VRAM placement forced on (flag `0x08ac6164` = 1 for the duration), so its pixels are copied into
   VRAM. */

void GfxTextureMoveToVram(char *name)

{
  u8 prev;
  void *tex;
  
  prev = g_gfxTexVramEnabled;
  g_gfxTexVramEnabled = 1;
  tex = GfxFindTexture(name);
  if (tex != (void *)0x0) {
    GfxTextureRebuild(tex);
  }
  g_gfxTexVramEnabled = prev;
  return;
}

