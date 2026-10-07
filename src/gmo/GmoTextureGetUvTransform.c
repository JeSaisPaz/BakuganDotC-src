// bdc 0x08a10838 GmoTextureGetUvTransform
#include "bdc.h"

/* Returns the optional UV transform block of a GMO texture record (`tex + 0x20`, `{uOffset,
   vOffset, uScale, vScale}`) when bit 0x20 of the flags half-word `+2` is set, else NULL. */

float *GmoTextureGetUvTransform(GmoTexture *tex)
{
  if ((tex != NULL) && ((tex->flags & 0x20) != 0)) {
    return tex->uvTransform;
  }
  return NULL;
}
