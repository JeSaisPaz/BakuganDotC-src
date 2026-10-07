// bdc 0x088a896c BtlItemSpawnerDestroyAll
#include "bdc.h"

/* Destroys every item spawner of `g_btlItemSpawnerList` (virtual deleting destructor, vtable
   entry 1, flags 3; each object's `next` is read before it is destroyed), frees the list head
   (`MemFree` under `MemLock`) and clears it, then resets `g_btlItemSpawnerCount` (also when
   there was no list). */
void BtlItemSpawnerDestroyAll(void)
{
    CoreObject *obj;
    CoreObject *next;
    const VtblEntry *dtor;

    if (g_btlItemSpawnerList != NULL) {
        obj = g_btlItemSpawnerList->head;
        if (obj != NULL) {
            next = obj->next;
            for (;;) {
                if (obj != NULL) {
                    dtor = &((const VtblEntry *)obj->vtable)[1];
                    ((void (*)(void *, s32))dtor->fn)((u8 *)obj + dtor->delta, 3);
                }
                if (next == NULL) {
                    break;
                }
                obj = next;
                next = next->next;
            }
        }
        if (g_btlItemSpawnerList != NULL) {
            MemLock();
            MemFree(g_btlItemSpawnerList, NULL, 0);
            MemUnlock();
            g_btlItemSpawnerList = NULL;
        }
    }
    g_btlItemSpawnerCount = 0;
}
