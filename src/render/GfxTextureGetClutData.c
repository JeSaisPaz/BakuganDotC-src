// bdc 0x089f70d8 GfxTextureGetClutData
#include "bdc.h"

/* Returns the texture's CLUT (palette) data pointer; used by `GfxSetEffectTint` to recolour
   palettes. */
void *GfxTextureGetClutData(GfxTexture *tex)
{
    return tex->clutData;
}
