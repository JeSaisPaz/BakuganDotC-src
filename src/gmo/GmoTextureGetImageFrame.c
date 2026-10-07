// bdc 0x08a10b88 GmoTextureGetImageFrame
#include "bdc.h"

/* Returns frame pointer `level` (`frame` modulo the frame count) of the image list head (`+0xc`) of
   a texture record (0x40 bytes: `+0x0` refcount, `+0x4` current image, `+0x8` current palette,
   `+0xc` image list, `+0x10` palette list, `+0x14` animation tracks (0x30 bytes, count `+0x18` +
   1), `+0x1d..+0x1f` frame selectors), or 0. */

u32 GmoTextureGetImageFrame(void *tex, int level, int frame)

{
  GmoImage *list;
  void **slot;
  int idx;

  if (tex != NULL) {
    list = ((GmoTexture *)tex)->images;
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
