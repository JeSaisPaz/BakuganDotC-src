// bdc 0x089f7920 GfxTextureListSwap
#include "bdc.h"

/* Replaces the `g_textureList` head with `list` and returns the previous head. Used by
   `UiTextPrinterCtor` to keep its font textures in a private list. */

void *GfxTextureListSwap(void *list)
{
  void *old = g_textureList;

  g_textureList = list;
  return old;
}
