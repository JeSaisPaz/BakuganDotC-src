// bdc 0x088383e8 BtlHudAdvicePickMessage
#include "bdc.h"

/* Chooses the advisor message id for HUD advice slot `slot` (0..21): a per-slot base id plus the
   advisor face (BtlHudAdviceGetFace) times 1, 2, 3 or 5, with a random line (`vrndi.s`, PlatformRandU32) for
   slots 0..4 and an alternate line set for slots 5 and 6 when `useAltLine` is set. Returns -1 when
   no message applies: always while the battle rule mode (script global variable 8) is 2; for slots
   5/6 (normal line) and 10/11 on stage 1 (script global variable 1); for slots 0..2 with face 5;
   for slot 5's alternate line with face 1; for slots 14/15 with a face outside 0..5 (unsigned
   test, so -1 included); for slot 20 with face 2; and for any `slot` above 21 (unsigned test, so
   negative slots too). */

int BtlHudAdvicePickMessage(BtlHud *self, int slot, bool useAltLine)
{
    s32 face;
    s32 msg;
    bool isStage1;

    (void)self;
    if (g_scriptGlobalVars[8] == 2) {
        return -1;
    }
    msg = -1;
    face = BtlHudAdviceGetFace();
    isStage1 = g_scriptGlobalVars[1] == 1;

    switch (slot) {
    case 0:
    case 1:
    case 2:
        /* Face 5 has no line here; otherwise one of 3 lines per face. */
        if (face != 5) {
            msg = face * 3 + 0x1a9 + (s32)((((PlatformRandU32() >> 16) * 3) >> 16));
            if (msg == 0x1a9) {
                /* Line 0x1a9 is replaced by 0x1aa or 0x1ab (top random bit). */
                msg = (s32)(((PlatformRandU32() >> 16) * 2) >> 16) + 0x1aa;
            }
        }
        break;
    case 3:
    case 4:
        /* One of 5 lines per face; two ids are remapped to late additions. */
        msg = face * 5 + 0x1da + (s32)(((PlatformRandU32() >> 16) * 5) >> 16);
        if (msg == 0x1dc) {
            msg = 0x387;
        } else if (msg == 0x1df) {
            msg = 0x388;
        }
        break;
    case 5:
        if (!useAltLine) {
            if (!isStage1) {
                msg = face * 2 + 0x251;
            }
        } else if (face == 1) {
            /* no alternate line for face 1 */
        } else if (face == 2 || face == 4 || face == 5) {
            msg = face * 2 + 0x217;
        } else {
            msg = face * 2 + 0x218;
        }
        break;
    case 6:
        if (!useAltLine) {
            if (!isStage1) {
                msg = face * 2 + 0x252;
            }
        } else if (face == 2 || face == 4 || face == 5) {
            msg = face * 2 + 0x218;
        } else {
            msg = face * 2 + 0x217;
        }
        break;
    case 7:
        msg = face * 3 + 0x230;
        break;
    case 8:
        msg = face * 3 + 0x231;
        break;
    case 9:
        msg = face + 0x24b;
        break;
    case 10:
        if (!isStage1) {
            msg = face * 2 + 0x25d;
        }
        break;
    case 11:
        if (!isStage1) {
            msg = face * 2 + 0x25e;
        }
        break;
    case 12:
        msg = face * 2 + 0x269;
        break;
    case 13:
        msg = face * 2 + 0x26a;
        break;
    case 14:
    case 15:
        /* Unsigned compare: face -1 (no advisor) gets no line. */
        if ((u32)face < 6) {
            switch (face) {
            case 1:
                msg = 0x27c;
                break;
            case 2:
                msg = 0x27d;
                break;
            case 3:
                msg = 0x27e;
                break;
            case 4:
                msg = 0x389;
                break;
            case 5:
                msg = 0x27f;
                break;
            default:
                msg = 0x27b;
                break;
            }
        }
        break;
    case 16:
    case 17:
    case 18:
        msg = face + 0x275;
        break;
    case 19:
        msg = face * 2 + 0x280;
        break;
    case 20:
        if (face != 2) {
            msg = face * 2 + 0x281;
        }
        break;
    case 21:
        msg = face + 0x32c;
        break;
    default:
        break;
    }
    return msg;
}
