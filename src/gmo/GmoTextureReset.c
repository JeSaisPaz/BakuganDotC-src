// bdc 0x08a12818 GmoTextureReset
#include "bdc.h"

/* Empties a texture record (0x40 bytes: `+0x0` refcount, `+0x4` current image, `+0x8` current
   palette, `+0xc` image list, `+0x10` palette list, `+0x14` animation tracks (0x30 bytes, count
   `+0x18` + 1), `+0x1d..+0x1f` frame selectors) (`GmoTextureDestroyContents`) and re-initialises
   it (`GmoTextureInit`), keeping its reference count. */

void GmoTextureReset(GmoTexture *tex)
{
  u16 refCount;

  if (tex != NULL) {
    refCount = tex->refCount;
    GmoTextureDestroyContents(tex);
    GmoTextureInit(tex);
    tex->refCount = refCount;
  }
}
