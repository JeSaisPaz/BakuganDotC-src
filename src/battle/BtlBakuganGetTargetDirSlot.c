// bdc 0x08862c68 BtlBakuganGetTargetDirSlot
#include "bdc.h"

/* Maps `BtlBakuganGetTargetDirection` through `g_btlTargetDirSlots` = {3, 0, 2, 1} (behind →
   3, ahead → 0, sides 2/3 → 2/1) to the order the directional motion sets use. */
int BtlBakuganGetTargetDirSlot(BtlBakugan *bakugan)
{
    return g_btlTargetDirSlots[BtlBakuganGetTargetDirection(bakugan)];
}
