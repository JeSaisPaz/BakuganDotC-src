// bdc 0x089f70a0 GfxTextureGetHeight
#include "bdc.h"

/* Returns the texture's pixel height from its TIM2 picture header. */

int GfxTextureGetHeight(void *tex)
{
  return (short)((GfxTexture *)tex)->picture->height;
}
