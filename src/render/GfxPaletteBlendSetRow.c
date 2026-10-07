// bdc 0x089ed188 GfxPaletteBlendSetRow
#include "bdc.h"

/* Copies CLUT row `row` of the texture unchanged into the blender output (`GfxPaletteBlend` with
   t = 0). */

void GfxPaletteBlendSetRow(void *pb, s32 row)

{
  GfxPaletteBlend(0.0f, pb, row, 0);
  return;
}

