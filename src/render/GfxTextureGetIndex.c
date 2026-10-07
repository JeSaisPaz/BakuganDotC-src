// bdc 0x089f78dc GfxTextureGetIndex
#include "bdc.h"

/* Returns the position of `tex` in `g_textureList`, or -1 when it is not in the list. Inverse of
   `GfxTextureGetByIndex`. */

int GfxTextureGetIndex(void *tex)
{
  int index = 0;
  GfxTexture *node = (GfxTexture *)g_textureList;

  while (node != NULL) {
    if (node == (GfxTexture *)tex) {
      return index;
    }
    node = node->next;
    index++;
  }
  return -1;
}
