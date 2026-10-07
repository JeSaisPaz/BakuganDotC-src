// bdc 0x088994f4 BtlAiPadPress04
#include "bdc.h"

/* Sets action bit 4 (held button: charge, latched heading in `BtlInputReadActions`) on the
   virtual pad of `BtlAi` (`ai+0x1b0`: two 0x70-byte input controllers as read by
   `BtlInputReadActions`, the current one at `+0` and the previous frame's at `+0x70`, owner unit
   `+0xe0`, stick axes `+0xe4`/`+0xe8`); returns 1. */

s32 BtlAiPadPress04(BtlAiPad *self)
{
    self->cur.aiActions |= 4;
    return 1;
}
