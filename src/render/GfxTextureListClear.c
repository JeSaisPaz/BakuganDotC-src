// bdc 0x089f7608 GfxTextureListClear
#include "bdc.h"

/* Releases the texture list (`g_textureList`, `CoreObjectChainDeleteAll`) and clears the head pointer. */

void GfxTextureListClear(void)

{
  CoreObjectChainDeleteAll(g_textureList);
  g_textureList = (void *)0x0;
  return;
}

