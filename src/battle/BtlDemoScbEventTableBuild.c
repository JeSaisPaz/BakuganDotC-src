// bdc 0x08908118 BtlDemoScbEventTableBuild
#include "bdc.h"

/* Builds the event table of a `.scb` scene from its event data: stores the frame count `data[1]`
   in frameCount, then for each of the `data[0]` groups (u32 offsets at byte 4, each relative to
   byte 4) allocates a 0x44-byte event-group node from the low end of the heap, constructs it from
   the group header (`BtlDemoScbEventGroupCtor`) and appends it to lists[0]
   (`CoreObjectListAppend`, NULL when the allocation failed). If lists[2] is NULL or empty
   afterwards it appends one more group built from a default header (record count 0, type 0xffff,
   word 0). */

static BtlDemoScbEventGroup *BtlDemoScbEventTableNewGroup(BtlDemoScbEventTable *table, u16 *hdr)
{
    bool fromLow;
    BtlDemoScbEventGroup *grp;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    grp = MemAlloc(sizeof(BtlDemoScbEventGroup), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (grp != NULL) {
        BtlDemoScbEventGroupCtor(grp, hdr, table);
    }
    return grp;
}

void BtlDemoScbEventTableBuild(BtlDemoScbEventTable *table, u16 *data)
{
    u32 *offsets;
    s32 i;
    bool empty;
    u16 defHdr[6];

    table->frameCount = data[1];
    offsets = (u32 *)&data[2];
    for (i = 0; i < data[0]; i++) {
        /* the binary has two identical arms for header[1] == 0xffff and != 0xffff */
        CoreObjectListAppend((CoreObject *)BtlDemoScbEventTableNewGroup(
                                 table, (u16 *)((u8 *)offsets + offsets[i])),
                             table->lists[0]);
    }
    empty = true;
    if (table->lists[2] != NULL) {
        empty = table->lists[2]->head == NULL;
    }
    if (empty) {
        /* only the first 8 bytes are set; the constructor also copies defHdr[4..5], which the
           binary leaves uninitialised on the stack */
        defHdr[1] = 0xffff;
        defHdr[0] = 0;
        defHdr[2] = 0;
        defHdr[3] = 0;
        CoreObjectListAppend((CoreObject *)BtlDemoScbEventTableNewGroup(table, defHdr),
                             table->lists[0]);
    }
}
