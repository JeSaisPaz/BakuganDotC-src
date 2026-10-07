// bdc 0x089ed110 GfxPaletteBlendInstall
#include "bdc.h"

/* Sets the first blended entry (`slot`) and optionally a new texture, then installs the blender's
   output buffer as the texture's CLUT (`GfxTextureSetSlotClut(tex, start, buffer, 1)`), remembering the
   result in `installed`. */

void GfxPaletteBlendInstall(GfxPaletteBlender *pb, s32 start, void *texture)
{
  pb->slot = start;
  if (texture != NULL) {
    pb->texture = texture;
  }
  pb->installed = GfxTextureSetSlotClut(pb->texture, start, pb->output, true);
}
