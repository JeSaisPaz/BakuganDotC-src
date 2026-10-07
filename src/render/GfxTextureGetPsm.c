// bdc 0x089f70ac GfxTextureGetPsm
#include "bdc.h"

/* Returns the GE pixel storage mode of a texture: the TIM2 image type (picture header `imageType`)
   mapped through `g_gfxPsmTable`. */

u32 GfxTextureGetPsm(void *tex)
{
  return g_gfxPsmTable[(signed char)((GfxTexture *)tex)->picture->imageType];
}
