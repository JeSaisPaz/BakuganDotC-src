// bdc 0x08888850 BtlCombatGetStatusRatio
#include "bdc.h"

/* Returns the remaining fraction of timed status `id` of a `BtlCombatState`: `remaining / total`
   for an active slot with `total != 0`, otherwise 0.0 (also 0.0 while `stats` is NULL). */
float BtlCombatGetStatusRatio(BtlCombatState *combat, int id)
{
    BtlCombatStatusSlot *slot;

    if (combat->stats == NULL) {
        return 0.0f;
    }
    slot = &combat->status[id];
    if (slot->active != 0 && slot->total != 0) {
        return (float)slot->remaining / (float)slot->total;
    }
    return 0.0f;
}
