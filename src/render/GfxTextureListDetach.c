// bdc 0x089f7910 GfxTextureListDetach
#include "bdc.h"

/* Returns the current `g_textureList` head and empties the list. Used by `GfxInitNullTexture`
   so the fallback texture is not part of the searchable list. */

void *GfxTextureListDetach(void)
{
  void *head = g_textureList;

  g_textureList = NULL;
  return head;
}
