// bdc 0x08a26d14 GmoTim2GetPicture
#include "bdc.h"

typedef struct Tim2FileHead {
  u8 magic[4];
  u8 version;
  s8 align;
  s16 pictureCount;
  u8 pad[8];
  u8 firstPicture16[0x70];
  u8 firstPicture128[1];
} Tim2FileHead;

/* Returns picture header `index` of a TIM2 file (picture count at `+6`; pictures start at `+0x10`,
   or `+0x80` when the header's alignment byte `+5` is set; each picture header starts with its
   total size), or NULL when out of range. */

void *GmoTim2GetPicture(const void *data, u32 size, s32 index)
{
  const Tim2FileHead *head = (const Tim2FileHead *)data;
  const u8 *pic;
  s32 i;

  if (index < 0 || index >= head->pictureCount) {
    return NULL;
  }
  pic = head->align != 0 ? head->firstPicture128 : head->firstPicture16;
  for (i = 0; i < index; i++) {
    pic += *(const s32 *)pic;
  }
  return (void *)pic;
}
