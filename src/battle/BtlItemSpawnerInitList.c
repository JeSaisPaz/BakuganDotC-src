// bdc 0x088a8370 BtlItemSpawnerInitList
#include "bdc.h"

/* Lazily allocates the item-spawner list head `g_btlItemSpawnerList` (an empty
   `CoreObjectList`, 12 bytes) from the low end of the heap, under `MemLock` and restoring the
   previous placement policy, then resets the spawner index counter `g_btlItemSpawnerCount`.
   Called by `ActorStageObjRecordInitLists` and `ActorStageObjRecordSpawn`. */
void BtlItemSpawnerInitList(void)
{
    if (g_btlItemSpawnerList == NULL) {
        bool fromLow;
        CoreObjectList *list;

        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        list = MemAlloc(sizeof(CoreObjectList), NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        g_btlItemSpawnerList = list;
        list->tail = NULL;
        g_btlItemSpawnerList->head = NULL;
        g_btlItemSpawnerList->count = 0;
    }
    g_btlItemSpawnerCount = 0;
}
