// bdc 0x08a24cd0 GmoImageArrayMeasureCopy
#include "bdc.h"

/* Measure pass for copying `count` pixel images (0x30-byte records): when `flags & 0x11` reserves
   the records (`GmoImagePlanReservePalettes`) and measures each (`GmoImageMeasureCopyOne`).
   Returns 1 (0 for NULL arguments). */

s32 GmoImageArrayMeasureCopy(void *textures, s32 count, u32 flags, void *arena)
{
  GmoImage *img = (GmoImage *)textures;
  s32 i;

  if (textures == NULL || arena == NULL) {
    return 0;
  }
  if ((flags & 0x11) != 0) {
    GmoImagePlanReservePalettes(count, arena);
    for (i = 0; i < count; i++) {
      GmoImageMeasureCopyOne(NULL, &img[i], flags, arena);
    }
  }
  return 1;
}
