// bdc 0x08909474 BtlDemoScbMotionEventParse
#include "bdc.h"

/* Body parser (`g_btlDemoScbMotionEventVtbl` entry 2) of `BtlDemoScbMotionEvent`. The body is a
   `u16` sub-type followed (at byte 4) by little-endian data `d`; returns the data size consumed:
   0: `flag0` = d byte 3 (4); 1: nothing (4); 2: `range2B`/`range2A` = d halfwords 1/0 (4);
   3: `motionId` = d word 0, `motionParam` = d halfword 3, `motionFlag` = d byte 5, `blendTime` =
   0.1 when d byte 4 is non-zero else 0 (8); 4: `range4B`/`range4A` = d halfwords 1/0 (4);
   5: skips 0x34; 6: skips 0x18; other sub-types return 0. Bytes are taken from the halfwords
   (low byte first), matching the PSP's byte loads. */
int BtlDemoScbMotionEventParse(BtlDemoScbMotionEvent *ev, const u16 *body)
{
    const u16 *d = body + 2;

    switch (body[0]) {
    case 0:
        ev->flag0 = (u8)(d[1] >> 8);
        return 4;
    case 1:
        return 4;
    case 2:
        ev->range2B = d[1];
        ev->range2A = d[0];
        return 4;
    case 3:
        ev->motionId = (u32)d[0] | ((u32)d[1] << 16);
        ev->motionParam = d[3];
        ev->motionFlag = (u8)(d[2] >> 8);
        ev->blendTime = ((u8)d[2] != 0) ? 0.1f : 0.0f;
        return 8;
    case 4:
        ev->range4B = d[1];
        ev->range4A = d[0];
        return 4;
    case 5:
        return 0x34;
    case 6:
        return 0x18;
    default:
        return 0;
    }
}
