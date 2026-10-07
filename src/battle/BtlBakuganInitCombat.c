// bdc 0x0885e128 BtlBakuganInitCombat
#include "bdc.h"

/* Thin wrapper used by `BtlBakuganCtor`: binds the unit's embedded `BtlCombatState` (`unit->combat`,
   `+0x434`) to the unit and species with `BtlCombatSetup``(&unit->combat, unit, kind)`. */
void BtlBakuganInitCombat(BtlBakugan *unit, s32 kind)
{
    BtlCombatSetup(&unit->combat, unit, kind);
}
