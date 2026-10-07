// bdc 0x08a10a04 GmoTextureSetPalette
#include "bdc.h"

/* Makes `pal` the current palette (`palette`, `+8`) of a `GmoTexture`, moving the pool-1
   image-heap reference from the old one (`GmoImageHeapRelease`, then `GmoImageHeapAddRef`).
   Does nothing for a NULL texture or when `pal` is already current. */
void GmoTextureSetPalette(void *tex_, void *pal)
{
    GmoTexture *tex = tex_;

    if (tex != NULL && tex->palette != pal) {
        GmoImageHeapRelease(1, tex->palette);
        GmoImageHeapAddRef(1, pal);
        tex->palette = pal;
    }
}
