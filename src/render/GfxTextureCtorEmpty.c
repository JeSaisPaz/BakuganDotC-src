// bdc 0x08a294b4 GfxTextureCtorEmpty
#include "bdc.h"

/* Constructs an empty texture object: `CoreObjectInit``(tex, NULL)` (not linked into a chain),
   texture vtable `0x08af5864` at `+0x14`, `+0x11c` cleared. Returns `tex`. */

void *GfxTextureCtorEmpty(void *tex)
{
  GfxTexture *t = (GfxTexture *)tex;

  CoreObjectInit((CoreObject *)tex, (CoreObject *)0x0);
  t->vtbl = g_gfxTextureVtbl;
  t->unk11c = 0;
  return tex;
}
