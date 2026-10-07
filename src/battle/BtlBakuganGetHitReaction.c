// bdc 0x0886423c BtlBakuganGetHitReaction
#include "bdc.h"

/* Maps a hit id to the hit-reaction code used by `BtlBakuganOnHit`: 0..2 → 1, 3..5 → 0x11,
   6..0xb → 2, 0xc..0xe → 4, 0xf..0x11 → 0x14, 0x12 → 5, 0x13 → 0x15, 0x14 → 6, 0x15 → 0x16.
   Any other id (negatives included) is tested against a list of special ids in order; hit id 0x1a
   also sets the flinch gauge to 100. Unlisted ids → 1. */
int BtlBakuganGetHitReaction(BtlBakugan *self, int hitId)
{
    u32 i;

    if (hitId >= 0 && hitId < 6) {
        return hitId < 3 ? 1 : 0x11;
    }
    if (hitId >= 6 && hitId < 0xc) {
        return 2;
    }
    if (hitId >= 0xc && hitId < 0x12) {
        return hitId < 0xf ? 4 : 0x14;
    }
    if (hitId >= 0x12 && hitId < 0x14) {
        return hitId == 0x13 ? 0x15 : 5;
    }
    if (hitId >= 0x14 && hitId < 0x16) {
        return hitId == 0x15 ? 0x16 : 6;
    }

    switch (hitId) {
    case 0xa7: return 3;
    case 0x1f: return 10;
    case 0x86: return 5;
    case 0x87: return 3;
    case 0x48: return 1;
    case 0xba: return 3;
    case 0x20: return 1;
    case 0x61:
    case 0x1d:
    case 0x1e:
    case 0x8b: return 4;
    case 0x54: return 2;
    default: break;
    }
    if (hitId > 0x1a && hitId < 0x1f) {
        return 3; /* 0x1b/0x1c: 0x1d/0x1e matched above */
    }
    if (hitId == 0x1a) {
        self->flinchGauge = 100;
        return 1;
    }
    if (hitId > 0x2b && hitId < 0x2e) {
        return 3;
    }
    if (hitId == 0xb3) {
        return 1;
    }
    if (hitId > 0xb4 && hitId < 0xb8) {
        return 8;
    }
    if (hitId == 0x3f || hitId == 0x44) {
        return 10;
    }
    if (hitId == 0xb8) {
        return 9;
    }
    for (i = 0; i < 7; i++) {
        if (g_btlHitReactionKind2Ids[i] == (u16)hitId) {
            return 2;
        }
    }
    return 1;
}
