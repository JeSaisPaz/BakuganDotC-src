// bdc 0x08a109fc GmoTextureRemovePalette
#include "bdc.h"

/* Unlinks a palette from the palette list of a texture record (tail call to
   `GmoTextureListRemove`). */
void GmoTextureRemovePalette(GmoTexture *tex, void *pal)
{
    GmoTextureListRemove(&tex->palettes, pal);
}
