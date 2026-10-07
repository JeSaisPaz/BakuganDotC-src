// bdc 0x08a10928 GmoTextureFindImage
#include "bdc.h"

/* Finds the `n`-th image with id `id` in the image list (`images`, `+0xc`) of a `GmoTexture`
   (`GmoImageListFind`); NULL for a NULL texture. */

GmoImage *GmoTextureFindImage(GmoTexture *tex, u32 id, int n)

{
  
  if (tex != (GmoTexture *)0x0) {
    return GmoImageListFind(tex->images,id,n);
  }
  return (GmoImage *)0x0;
}

