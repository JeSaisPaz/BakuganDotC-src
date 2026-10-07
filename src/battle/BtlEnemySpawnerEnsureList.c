// bdc 0x088a5468 BtlEnemySpawnerEnsureList
#include "bdc.h"

/* Allocates the 12-byte list holder `g_btlEnemySpawnerList` (head, tail, count) of the enemy
   spawners from the low end of the game heap when it does not exist yet, restoring the previous
   allocation direction afterwards, and zeroes it. The allocation result is not checked for NULL. */
void BtlEnemySpawnerEnsureList(void)
{
    bool fromLow;
    CoreObjectList *list;

    if (g_btlEnemySpawnerList != NULL) {
        return;
    }
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    list = MemAlloc(sizeof(CoreObjectList), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    g_btlEnemySpawnerList = list;
    list->tail = NULL;
    g_btlEnemySpawnerList->head = NULL;
    g_btlEnemySpawnerList->count = 0;
}
