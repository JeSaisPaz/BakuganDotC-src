// bdc 0x0889954c BtlAiPadPress02
#include "bdc.h"

/* Sets action bit 2 (the button mapped from pad 0x4000; used as the dodge after a sidestep) on the
   virtual pad of `BtlAi` (`ai+0x1b0`: two 0x70-byte input controllers as read by
   `BtlInputReadActions`, the current one at `+0` and the previous frame's at `+0x70`, owner unit
   `+0xe0`, stick axes `+0xe4`/`+0xe8`); returns 1. */

s32 BtlAiPadPress02(BtlAiPad *self)

{
  self->cur.aiActions |= 2;
  return 1;
}

