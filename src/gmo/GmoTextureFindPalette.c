// bdc 0x08a10940 GmoTextureFindPalette
#include "bdc.h"

/* Finds the `n`-th palette with id `id` in the palette list (`palettes`, `+0x10`) of a
   `GmoTexture` (`GmoImageListFind`); NULL for a NULL texture. */

GmoImage *GmoTextureFindPalette(GmoTexture *tex, u32 id, int n)

{
  
  if (tex != (GmoTexture *)0x0) {
    return GmoImageListFind(tex->palettes,id,n);
  }
  return (GmoImage *)0x0;
}

