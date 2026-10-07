// bdc 0x0885bcb4 BtlUnitAltDtor
#include "bdc.h"

/* Destructor of the non-playable unit class (a BtlCpuUnit subclass): does nothing for NULL;
   otherwise restores g_btlUnitAltVtbl, destroys and clears the AI object when set (BtlAiDtor,
   flags 3), runs the CPU-unit base destructor (flags 0) and frees the object when bit 0 of
   `flags` is set. */
void BtlUnitAltDtor(BtlCpuUnit *unit, u32 flags)
{
    if (unit == NULL) {
        return;
    }
    unit->base.base.base.vtable = g_btlUnitAltVtbl;
    if (unit->ai != NULL) {
        BtlAiDtor(unit->ai, 3);
        unit->ai = NULL;
    }
    BtlCpuUnitDtor(unit, 0);
    if (flags & 1) {
        MemLock();
        MemFree(unit, NULL, 0);
        MemUnlock();
    }
}
