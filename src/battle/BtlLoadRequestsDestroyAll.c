// bdc 0x0885b568 BtlLoadRequestsDestroyAll
#include "bdc.h"

/* Deletes every pending load request in `g_btlLoadRequests` through its deleting destructor
   (vtable entry 1, flag 3; the next link is read before each delete), then frees the list holder
   and clears `g_btlLoadRequests`. Called by `BtlMainTaskDtor`; does nothing when the holder
   does not exist. */
void BtlLoadRequestsDestroyAll(void)
{
    CoreObject *obj;
    CoreObject *next;

    if (g_btlLoadRequests == NULL) {
        return;
    }
    obj = g_btlLoadRequests->head;
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
    if (g_btlLoadRequests != NULL) {
        MemLock();
        MemFree(g_btlLoadRequests, NULL, 0);
        MemUnlock();
        g_btlLoadRequests = NULL;
    }
}
