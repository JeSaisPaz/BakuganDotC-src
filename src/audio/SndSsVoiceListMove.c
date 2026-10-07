// bdc 0x08a235b0 SndSsVoiceListMove
#include "bdc.h"

/* Moves voice `voice` from one voice list to another: finds its record in `src` (by index byte),
   closes the gap, decrements `*srcCount`, appends it to `dst` and increments `*dstCount`. Returns
   the record. */

u8 *SndSsVoiceListMove(u32 *srcCount, u8 **src, s32 *dstCount, u8 **dst, u32 voice)
{
  u32 count;
  u32 i;
  u32 last;
  u8 *found;
  s32 n;

  count = *srcCount;
  i = 0;
  found = NULL;
  for (; i < count; i++) {
    if (*src[i] == voice) {
      found = src[i];
      break;
    }
  }
  last = count - 1;
  for (; i < last; i++) {
    src[i] = src[i + 1];
  }
  *srcCount = count - 1;
  n = *dstCount;
  *dstCount = n + 1;
  dst[n] = found;
  return found;
}
