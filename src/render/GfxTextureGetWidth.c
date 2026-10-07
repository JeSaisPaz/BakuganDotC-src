// bdc 0x089f7094 GfxTextureGetWidth
#include "bdc.h"

/* Returns the texture's pixel width from its TIM2 picture header. */

int GfxTextureGetWidth(void *tex)
{
  return (short)((GfxTexture *)tex)->picture->width;
}
