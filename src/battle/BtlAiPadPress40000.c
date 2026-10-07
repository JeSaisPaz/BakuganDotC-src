// bdc 0x08899560 BtlAiPadPress40000
#include "bdc.h"

/* Sets action bit 0x40000 (counter input of the guard layer) on the virtual pad of
   `BtlAi` (`ai+0x1b0`: two 0x70-byte input controllers as read by
   `BtlInputReadActions`, the current one at `+0` and the previous frame's at `+0x70`, owner unit
   `+0xe0`, stick axes `+0xe4`/`+0xe8`); returns 1. */

s32 BtlAiPadPress40000(BtlAiPad *self)

{
  self->cur.aiActions |= 0x40000;
  return 1;
}

