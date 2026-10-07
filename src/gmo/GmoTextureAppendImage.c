// bdc 0x08a10994 GmoTextureAppendImage
#include "bdc.h"

/* Appends an image to the image list of a texture record. The binary tail-calls
   GmoListAppend (`j 0x08a10958` with a0 = &tex->images); Ghidra inlined its body. */

void GmoTextureAppendImage(GmoTexture *tex, void *img)
{
    GmoListAppend(&tex->images, img);
}
