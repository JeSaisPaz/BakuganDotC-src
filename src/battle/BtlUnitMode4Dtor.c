// bdc 0x0885cfb0 BtlUnitMode4Dtor
#include "bdc.h"

/* Destructor of the mode-4 battle unit (a BtlCpuUnit subclass): does nothing for NULL; otherwise
   restores g_btlUnitMode4Vtbl, destroys and clears the AI object when set (BtlAiDtor, flags 3),
   runs the CPU-unit base destructor (flags 0) and frees the object when bit 0 of `flags` is
   set. */
void BtlUnitMode4Dtor(BtlCpuUnit *unit, u32 flags)
{
    if (unit == NULL) {
        return;
    }
    unit->base.base.base.vtable = g_btlUnitMode4Vtbl;
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
