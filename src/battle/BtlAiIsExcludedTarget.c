// bdc 0x0888e0c4 BtlAiIsExcludedTarget
#include "bdc.h"

/* Calls unit virtual `slot` (a no-argument predicate) through the GCC 2.x vtable entry. */
static int UnitVirtual(BtlBakugan *unit, int slot)
{
    const VtblEntry *entry = &((const VtblEntry *)unit->base.base.vtable)[slot];

    return ((int (*)(void *))entry->fn)((u8 *)unit + entry->delta);
}

/* Returns true when `BtlAi` must not target `unit`. Always false unless the battle
   rule mode (script global variable 8) is 1. In that mode: when the unit's virtual slot 10
   (`+0x50`, the Bakugan class test) is non-zero, the result is `!unit->isPlayer`; otherwise all of
   slots 13, 19, 11 and 12 (`+0x68`, `+0x98`, `+0x58`, `+0x60`) are called and any non-zero result
   gives true; when all are zero, slots 15 and 16 (`+0x78`, `+0x80`) are still called but the
   result is false either way. `self` is unused. */
bool BtlAiIsExcludedTarget(BtlAi *self, BtlBakugan *unit)
{
    int any;

    (void)self;
    if (g_scriptGlobalVars[8] != 1) {
        return false;
    }
    if (UnitVirtual(unit, 10) != 0) {
        return unit->isPlayer == 0;
    }
    any = UnitVirtual(unit, 13);
    any |= UnitVirtual(unit, 19);
    any |= UnitVirtual(unit, 11);
    any |= UnitVirtual(unit, 12);
    if (any != 0) {
        return true;
    }
    any = UnitVirtual(unit, 15);
    any |= UnitVirtual(unit, 16);
    (void)any;
    return false;
}
