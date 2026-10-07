// bdc 0x088a5a1c BtlEnemySpawnerDestroyAll
#include "bdc.h"

/* Deletes every enemy spawner in `g_btlEnemySpawnerList` through its deleting destructor
   (vtable entry 1, flag 3; the next link is read before each delete), then frees the list holder
   and clears `g_btlEnemySpawnerList`. Does nothing when the holder does not exist. Called by
   the stage teardown `ActorStageObjRecordDestroyAll`. */
void BtlEnemySpawnerDestroyAll(void)
{
    CoreObject *obj;
    CoreObject *next;

    if (g_btlEnemySpawnerList == NULL) {
        return;
    }
    obj = g_btlEnemySpawnerList->head;
    if (obj != NULL) {
        next = obj->next;
        for (;;) {
            if (obj != NULL) {
                const VtblEntry *dtor = &((const VtblEntry *)obj->vtable)[1];

                ((void (*)(void *, s32))dtor->fn)((u8 *)obj + dtor->delta, 3);
            }
            if (next == NULL) {
                break;
            }
            obj = next;
            next = next->next;
        }
    }
    if (g_btlEnemySpawnerList != NULL) {
        MemLock();
        MemFree(g_btlEnemySpawnerList, NULL, 0);
        MemUnlock();
        g_btlEnemySpawnerList = NULL;
    }
}
