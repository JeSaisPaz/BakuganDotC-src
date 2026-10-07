// bdc 0x089d93e8 GmoMotionTrackKind
#include "bdc.h"

/* Classifies a `0xb3` track by its channel id: ids `0x48..0x4d` and `0xe1` give `0x100` with bit 0
   set when `(flags & 0xf) != 3` and cleared when any of `flags & 0xff00` is set; ids `0x42..0x47`
   and `0x4f` give `0x100`; ids `0x82..0x88` and `0x98` give `0x200`; anything else gives 0. The
   result is OR-ed into the track's `u16` kind flags at `+2`. */

u32 GmoMotionTrackKind(s32 id, u32 flags)

{
  u32 bit0;
  
  if (((0x47 < id) && (id < 0x4e)) || (id == 0xe1)) {
    bit0 = (u32)((flags & 0xf) != 3);
    if ((flags & 0xff00) != 0) {
      bit0 = 0;
    }
    return bit0 | 0x100;
  }
  if (((id < 0x42) || (0x47 < id)) && (id != 0x4f)) {
    if (((id < 0x82) || (0x88 < id)) && (id != 0x98)) {
      return 0;
    }
    return 0x200;
  }
  return 0x100;
}

