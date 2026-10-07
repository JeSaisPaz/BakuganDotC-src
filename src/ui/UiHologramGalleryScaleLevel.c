// bdc 0x0891d518 UiHologramGalleryScaleLevel
#include "bdc.h"

/* Returns `level * 10 / 7` (integer). */

u32 UiHologramGalleryScaleLevel(UiHologramGallery *self, u32 level)

{
  return (u32)(((s32)(level & 0xff) * 10) / 7);
}

