// bdc 0x088866f8 BtlCombatDtor
#include "bdc.h"

/* Deleting destructor of a `BtlCombatState` (entry 1 of `g_btlCombatStateVtbl`): re-installs
   that vtable at `vtable` (+0x11c), frees the stat table `stats` under `MemLock` and clears the
   pointer when it is set, and when `flags & 1` also frees the object itself. Does nothing for
   NULL. */
void BtlCombatDtor(BtlCombatState *combat, u32 flags)
{
    BtlUnitStatTable *stats;

    if (combat == NULL) {
        return;
    }
    stats = combat->stats;
    combat->vtable = g_btlCombatStateVtbl;
    if (stats != NULL) {
        MemLock();
        MemFree(stats, NULL, 0);
        MemUnlock();
        combat->stats = NULL;
    }
    if ((flags & 1) != 0) {
        MemLock();
        MemFree(combat, NULL, 0);
        MemUnlock();
    }
}
