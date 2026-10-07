// bdc 0x08a10a5c GmoTextureSetImage
#include "bdc.h"

/* Makes `img` the current image (`image`, `+0x4`) of a `GmoTexture`, moving the pool-1 image-
   heap reference from the old image to the new one (`GmoImageHeapRelease`, then
   `GmoImageHeapAddRef`). Does nothing for a NULL texture or when `img` is already current. */
void GmoTextureSetImage(GmoTexture *tex, void *img)
{
    if (tex == NULL) {
        return;
    }
    if (tex->image == img) {
        return;
    }
    GmoImageHeapRelease(1, tex->image);
    GmoImageHeapAddRef(1, img);
    tex->image = img;
}
