// bdc 0x089f7534 GfxTextureMoveToMainRam
#include "bdc.h"

/* Finds texture `name` and moves it back out of VRAM: with VRAM placement off (`0x08ac6164` = 0)
   runs `GfxTextureReleaseVram`. Counterpart of `GfxTextureMoveToVram`, used by the battle and
   main-menu destructors. */

void GfxTextureMoveToMainRam(char *name)

{
  u8 prev;
  void *tex;
  
  prev = g_gfxTexVramEnabled;
  g_gfxTexVramEnabled = 0;
  tex = GfxFindTexture(name);
  if (tex != (void *)0x0) {
    GfxTextureReleaseVram(tex);
  }
  g_gfxTexVramEnabled = prev;
  return;
}

