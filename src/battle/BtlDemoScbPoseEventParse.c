// bdc 0x08909318 BtlDemoScbPoseEventParse
#include "bdc.h"

/* Body parser (`g_btlDemoScbPoseEventVtbl` entry 2) of `BtlDemoScbPoseEvent`. The body starts
   with a `u16` sub-type and its data at byte 4. Sub-type 0: `valueA` = low half of data word 0,
   `valueB` = data halfword 3; returns 8. Sub-types 2 and 3 return 0x34, 4 returns 0x38, 5 returns
   0xc; sub-type 1 and anything above 5 return 0. */
int BtlDemoScbPoseEventParse(BtlDemoScbPoseEvent *ev, const u16 *body)
{
    const u16 *half = body + 2;
    const u32 *words = (const u32 *)half;
    u16 subType = body[0];

    if (subType > 5) {
        return 0;
    }
    switch (subType) {
    case 1:
        return 0;
    case 2:
        return 0x34;
    case 3:
        return 0x34;
    case 4:
        return 0x38;
    case 5:
        return 0xc;
    default:
        ev->valueA = (s16)words[0];
        ev->valueB = (s16)half[3];
        return 8;
    }
}
