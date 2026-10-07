// bdc 0x08a109ec GmoTextureRemoveImage
#include "bdc.h"

/* Unlinks an image from the image list of a texture record. The binary tail-calls
   GmoTextureListRemove (`j 0x08a1099c` with a0 = &tex->images); Ghidra inlined its body. */
void GmoTextureRemoveImage(GmoTexture *tex, void *img)
{
    GmoTextureListRemove(&tex->images, img);
}
