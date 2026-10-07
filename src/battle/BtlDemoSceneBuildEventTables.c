// bdc 0x089085ac BtlDemoSceneBuildEventTables
#include "bdc.h"

/* Builds the event tables of a `.scb` scene from `tbl` (`u32 count` then `count` offsets relative
   to `tbl + 1`): allocates one 0x40-byte `BtlDemoScbEventTable` per entry from the low end of the heap, constructs
   it with `BtlDemoScbEventTableCtor` and appends it to `list` (`CoreObjectListAppend`); a
   failed allocation appends NULL. */

void BtlDemoSceneBuildEventTables(void *list, u32 *tbl)
{
    CoreObjectList *tables = (CoreObjectList *)list;
    u8 *records = (u8 *)(tbl + 1);
    u32 i;

    for (i = 0; i < tbl[0]; i++) {
        u32 offset = tbl[1 + i];
        bool fromLow;
        CoreObject *obj;
        CoreObject *built;

        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        obj = MemAlloc(sizeof(BtlDemoScbEventTable), NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        built = NULL;
        if (obj != NULL) {
            BtlDemoScbEventTableCtor(obj, (u16 *)(records + offset));
            built = obj;
        }
        CoreObjectListAppend(built, tables);
    }
}
