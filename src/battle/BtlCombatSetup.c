// bdc 0x08886e44 BtlCombatSetup
#include "bdc.h"

/* Binds a freshly constructed `BtlCombatState` to its unit and species: stores `owner`; when
   `stats` is NULL allocates the 0x110-byte stat table from the low end of the heap (under
   `MemLock`, restoring the previous placement policy); copies the species' record from
   `g_btlKindStatTables``[species]` into it (species 1..0x20; any other value uses entry 1). Then
   sets the four stat levels (`level`, `attackLevel`, `defenseLevel`, `level4`) to 4, clears `dead`
   and `artReadyFrames`, sets energy to 1000, applies the loadout and handicap
   (`BtlCombatLoadLoadoutAndHandicap`) and finally `maxHp = hp =`
   `BtlCombatComputeMaxHp``(combat)`. The allocation result is not checked. */

void BtlCombatSetup(BtlCombatState *combat, void *owner, int species)
{
    BtlUnitStatTable *stats = combat->stats;
    bool fromLow;
    float maxHp;

    combat->owner = owner;
    if (stats == NULL) {
        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        stats = MemAlloc(sizeof(BtlUnitStatTable), NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        combat->stats = stats;
    }
    if (species > 0 && species <= 0x20) {
        *stats = *(const BtlUnitStatTable *)g_btlKindStatTables[species];
    } else {
        *stats = *(const BtlUnitStatTable *)g_btlKindStatTables[1];
    }
    combat->level = 4;
    combat->attackLevel = 4;
    combat->defenseLevel = 4;
    combat->level4 = 4;
    combat->dead = 0;
    combat->artReadyFrames = 0;
    combat->energy = 1000.0f;
    BtlCombatLoadLoadoutAndHandicap(combat);
    maxHp = BtlCombatComputeMaxHp(combat);
    combat->maxHp = maxHp;
    combat->hp = maxHp;
}
