// bdc 0x089f70cc GfxTextureGetClutColors
#include "bdc.h"

/* Returns the number of CLUT colours from the texture's TIM2 picture header. */

int GfxTextureGetClutColors(void *tex)
{
  return (short)((GfxTexture *)tex)->picture->clutColors;
}
