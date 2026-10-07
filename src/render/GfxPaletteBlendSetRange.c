// bdc 0x089ed158 GfxPaletteBlendSetRange
#include "bdc.h"

/* Sets the blended CLUT entry range of a palette blender (`GfxPaletteBlender`): when `start` is
   below the entry count (`count`) it stores `start` and `rangeCount`, clipping the length so
   the range ends at the entry count; out-of-range `start` leaves the blender unchanged. */

void GfxPaletteBlendSetRange(GfxPaletteBlender *pb, s32 start, s32 count)

{
  s32 total;

  total = pb->count;
  if (start < total) {
    if (total <= start + count) {
      count = count - ((start + count) - total);
    }
    pb->start = start;
    pb->rangeCount = count;
  }
  return;
}
