// bdc 0x08899a08 BtlCpuUnitDtor
#include "bdc.h"

/* Destructor of the CPU-controlled battle Bakugan class: does nothing for NULL; otherwise
   restores g_btlCpuUnitVtbl, destroys and clears the AI object when set (BtlAiDtor, flags 3),
   runs the Bakugan base destructor (flags 0) and frees the object when bit 0 of `flags` is set. */
void BtlCpuUnitDtor(BtlCpuUnit *unit, u32 flags)
{
    if (unit == NULL) {
        return;
    }
    unit->base.base.base.vtable = g_btlCpuUnitVtbl;
    if (unit->ai != NULL) {
        BtlAiDtor(unit->ai, 3);
        unit->ai = NULL;
    }
    BtlBakuganDtor(&unit->base, 0);
    if (flags & 1) {
        MemLock();
        MemFree(unit, NULL, 0);
        MemUnlock();
    }
}
