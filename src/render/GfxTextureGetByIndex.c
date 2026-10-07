// bdc 0x089f7890 GfxTextureGetByIndex
#include "bdc.h"

/* Returns the `index`-th texture of `g_textureList`, or the `"NonTexture"` fallback
   (`GfxGetNullTexture`) when the list is shorter. */

void *GfxTextureGetByIndex(int index)
{
  GfxTexture *t = (GfxTexture *)g_textureList;
  int i = 0;

  while (t != (GfxTexture *)0) {
    if (i == index) {
      return t;
    }
    t = t->next;
    i++;
  }
  return GfxGetNullTexture();
}
