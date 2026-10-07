// bdc 0x089084b8 BtlDemoScbEventTableDtor
#include "bdc.h"

/* Destructor of a `.scb` event table: re-installs `g_btlDemoScbEventTableVtbl`, frees the
   nine event lists (`BtlDemoScbEventTableClear`), runs the `CoreObjectDtor` base destructor
   without freeing, then frees the object under the heap lock when bit 0 of `flags` is set.
   A NULL `tbl` does nothing. */
void BtlDemoScbEventTableDtor(BtlDemoScbEventTable *tbl, u32 flags)
{
    if (tbl == NULL) {
        return;
    }
    tbl->base.vtable = g_btlDemoScbEventTableVtbl;
    BtlDemoScbEventTableClear(tbl);
    CoreObjectDtor(&tbl->base, 0);
    if ((flags & 1) != 0) {
        MemLock();
        MemFree(tbl, NULL, 0);
        MemUnlock();
    }
}
