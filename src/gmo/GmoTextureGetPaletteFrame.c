// bdc 0x08a10864 GmoTextureGetPaletteFrame
#include "bdc.h"

/* Returns frame pointer `level` (`frame` modulo the frame count) of the palette list (`+0x10`) of a
   texture record (0x40 bytes: `+0x0` refcount, `+0x4` current image, `+0x8` current palette, `+0xc`
   image list, `+0x10` palette list, `+0x14` animation tracks (0x30 bytes, count `+0x18` + 1),
   `+0x1d..+0x1f` frame selectors), or 0 when out of range. */

u32 GmoTextureGetPaletteFrame(void *tex, int level, int frame)

{
  GmoImage *list;
  void **slot;
  int idx;

  if (tex != NULL) {
    list = ((GmoTexture *)tex)->palettes;
    if (list != NULL && level >= 0 && level < (int)list->levelCount) {
      slot = list->levels + level;
      if (frame != 0) {
        idx = frame % (int)list->frameCount;
        if (idx < 0) {
          idx += list->frameCount;
        }
        slot += idx * list->levelCount;
      }
      return (u32)(uintptr_t)*slot;
    }
  }
  return 0;
}
