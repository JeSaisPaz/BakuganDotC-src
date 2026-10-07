// bdc 0x089ed040 GfxPaletteBlendInit
#include "bdc.h"

/* Initialises a palette blender (`GfxPaletteBlender`) for `count` CLUT entries of `texture`:
   allocates the 64-byte-aligned output buffer (`MemAllocAligned`, low heap) and clears the
   range start and installed flag. */

GfxPaletteBlender *GfxPaletteBlendInit(GfxPaletteBlender *pb, s32 count, void *texture)
{
  pb->slot = 0;
  pb->count = count;
  pb->texture = texture;
  pb->output = MemAllocAligned(count * sizeof(u32), true);
  pb->start = 0;
  pb->rangeCount = count;
  pb->installed = 0;
  return pb;
}
