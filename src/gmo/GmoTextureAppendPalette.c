// bdc 0x08a109f4 GmoTextureAppendPalette
#include "bdc.h"

/* Appends a palette to the palette list of a texture record (tail call `j` into `GmoListAppend`,
   which links through the `next` pointer at item `+4` and ignores a NULL item). */
void GmoTextureAppendPalette(GmoTexture *tex, void *pal)
{
    GmoListAppend(&tex->palettes, pal);
}
