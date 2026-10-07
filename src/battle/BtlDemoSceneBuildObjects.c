// bdc 0x08904ce0 BtlDemoSceneBuildObjects
#include "bdc.h"

/* Builds the scene object list `list` from the table `tbl` (`u32 count`, then `count` byte offsets
   relative to `tbl + 1`, each to a `BtlDemoScbRecord`): per record allocates 0x2c bytes from the
   low end of the heap, constructs a `BtlDemoScbObject` with `BtlDemoScbObjectCtor` when the
   allocation succeeded, and appends the result (NULL on failure) with `CoreObjectListAppend`.
   Then walks the list switching on each record's `subType`; every case (1..4 and default) is
   empty, so the walk has no effect. */
void BtlDemoSceneBuildObjects(void *list, u32 *tbl)
{
    CoreObjectList *objects = (CoreObjectList *)list;
    u8 *records = (u8 *)(tbl + 1);
    CoreObject *node;
    u32 i;

    for (i = 0; i < tbl[0]; i++) {
        u32 offset = tbl[1 + i];
        bool fromLow;
        CoreObject *obj;
        CoreObject *built;

        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        obj = MemAlloc(sizeof(BtlDemoScbObject), NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        built = NULL;
        if (obj != NULL) {
            BtlDemoScbObjectCtor(obj, records + offset);
            built = obj;
        }
        CoreObjectListAppend(built, objects);
    }

    for (node = objects->head; node != NULL; node = node->next) {
        switch (((BtlDemoScbObject *)node)->rec.subType) {
        case 1:
        case 2:
        case 3:
        case 4:
        default:
            break;
        }
    }
}
