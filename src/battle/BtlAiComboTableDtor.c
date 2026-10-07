// bdc 0x08899610 BtlAiComboTableDtor
#include "bdc.h"

/* Destructor of the combo-input table of `BtlAi`: restores `g_btlAiComboTableVtbl`
   and frees the object when bit 0 of `flags` is set. Called by `BtlAiDtor` with flags 2
   (embedded). */
void BtlAiComboTableDtor(BtlAiComboTable *table, u32 flags)
{
    if (table == NULL) {
        return;
    }
    table->vtbl = g_btlAiComboTableVtbl;
    if (flags & 1) {
        MemLock();
        MemFree(table, NULL, 0);
        MemUnlock();
    }
}
