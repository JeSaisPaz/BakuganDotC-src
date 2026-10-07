// bdc 0x089ed098 GfxPaletteBlendDtor
#include "bdc.h"

/* Destructor of a palette blender: restores the texture's own CLUT when the blender was installed
   (`GfxTextureFreeSlots`), frees the output buffer (`MemFreeAligned`) and the object when `flags & 1`.
   Called from `BtlHudDtor` and `UiScreenDtor`. */

void GfxPaletteBlendDtor(GfxPaletteBlender *pb, u32 flags)
{
  if (pb != NULL) {
    if (pb->installed != 0) {
      GfxTextureFreeSlots(pb->texture);
    }
    MemFreeAligned(pb->output);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(pb, NULL, 0);
      MemUnlock();
    }
  }
}
