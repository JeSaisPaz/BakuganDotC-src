// bdc 0x08909588 BtlDemoScbSoundEventParse
#include "bdc.h"

/* Body parser (`g_btlDemoScbSoundEventVtbl` entry 2) of `BtlDemoScbSoundEvent`. The body starts
   with a `u16` sub-type and its data at byte 4. Sub-type 0: `paramA`/`paramB` are the high/low
   halves of data word 0, `paramC`/`paramD` data halfwords 3/2, `value` data word 2; returns 0xc.
   Sub-type 1: `value` = data word 0; returns 4. Other sub-types: returns 0. */
int BtlDemoScbSoundEventParse(BtlDemoScbSoundEvent *ev, const u16 *body)
{
    const u16 *half = body + 2;
    const u32 *words = (const u32 *)half;
    u16 subType = body[0];

    if (subType == 0) {
        ev->paramA = (u16)(words[0] >> 16);
        ev->paramB = (u16)words[0];
        ev->paramC = half[3];
        ev->paramD = half[2];
        ev->value = words[2];
        return 0xc;
    }
    if (subType < 2) {
        ev->value = words[0];
        return 4;
    }
    return 0;
}
