// bdc 0x089f6cac GfxTextureCtor
#include "bdc.h"

/* Constructor of the 0x140-byte texture object: `CoreObjectInit`, texture vtable `g_gfxTextureVtbl` at
   `+0x14`, then `GfxTextureInitFromTim2``(tex, name, tim2, flag)`. Returns `tex`. */

CoreObject *GfxTextureCtor(CoreObject *tex, char *name, void *tim2, u8 flag)
{
  CoreObjectInit(tex, (CoreObject *)0x0);
  tex->vtable = g_gfxTextureVtbl;
  GfxTextureInitFromTim2(tex, name, tim2, flag);
  return tex;
}
