// bdc 0x08a25e14 GmoImageGetLevelData
#include "bdc.h"

/* Returns the data pointer of mip level `level` and animation frame `frame` (wrapped modulo the
   frame count `+0x26`) of an image: `levels[frame * levelCount + level]` from the table at `+0x20`.
   NULL for a bad level or NULL texture. */

void *GmoImageGetLevelData(const GmoImage *self, s32 level, s32 frame)

{
  s32 idx;
  s32 count;

  if (self == (GmoImage *)0x0 || level < 0 || level >= (s32)self->levelCount) {
    return (void *)0x0;
  }
  if (frame == 0) {
    return self->levels[level];
  }
  count = (s32)self->frameCount;
  idx = frame % count;
  if (idx < 0) {
    idx += count;
  }
  return self->levels[level + idx * (s32)self->levelCount];
}
