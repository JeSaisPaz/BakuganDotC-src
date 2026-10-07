// bdc 0x088661e0 BtlFindPlayerBakuganBySlot
#include "bdc.h"

/* Returns the first player-controlled unit (`isPlayer`, `+0x158`, non-zero) of `g_btlBakuganList` whose slot
   `playerSlot` (`+0x150`) equals `slot`, or NULL. */

void *BtlFindPlayerBakuganBySlot(int slot)
{
    BtlBakugan *it = NULL;

    if (g_btlBakuganList != NULL) {
        it = *(BtlBakugan **)g_btlBakuganList;
    }
    for (; it != NULL; it = (BtlBakugan *)it->base.base.next) {
        if (it->isPlayer != 0 && it->playerSlot == slot) {
            return it;
        }
    }
    return NULL;
}
