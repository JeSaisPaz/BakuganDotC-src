// bdc 0x08877a14 BtlHitIdUsesLinkedTarget
#include "bdc.h"

/* For an attack hit id in 0x23..0xb2 (type = id - 0x23, clamped to 0..0x8d) returns 1 when that
   type has the linked-target flag (signed high nibble of byte 2 of its info word in
   `g_btlAttackTypeInfo`) set, else 0; ids outside the range return 0. Used by
   `ActorCrystalOnHit`. */
int BtlHitIdUsesLinkedTarget(int hitId)
{
    int type;

    if (hitId < 0x23 || hitId >= 0xb3) {
        return 0;
    }
    type = hitId - 0x23;
    if (type < 0) {
        type = 0;
    } else if (type > 0x8d) {
        type = 0x8d;
    }
    return ((g_btlAttackTypeInfo[type] >> 20) & 0xf) != 0;
}
