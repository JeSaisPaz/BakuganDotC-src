// bdc 0x088b3da4 StopWallDestroyAll
#include "bdc.h"

/* Deletes every stop wall in `g_stopWallList` through its deleting destructor (vtable
   entry 1, flag 3; the next link is read before each delete), then frees the list holder and
   clears `g_stopWallList`. Called by `ActorStageObjRecordDestroyAll`; does nothing when the
   holder does not exist. */
void StopWallDestroyAll(void)
{
    CoreObject *obj;
    CoreObject *next;

    if (g_stopWallList == NULL) {
        return;
    }
    obj = g_stopWallList->head;
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
    if (g_stopWallList != NULL) {
        MemLock();
        MemFree(g_stopWallList, NULL, 0);
        MemUnlock();
        g_stopWallList = NULL;
    }
}
