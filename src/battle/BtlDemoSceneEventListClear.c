// bdc 0x0890869c BtlDemoSceneEventListClear
#include "bdc.h"

/* Deletes every event table of a demo scene's event list (`CoreObjectList`) through its virtual
   deleting destructor (vtable entry 1, flags 3), reading the next pointer before each delete,
   then empties the list: tail, head and count set to 0. */
void BtlDemoSceneEventListClear(void *list)
{
    CoreObjectList *events = (CoreObjectList *)list;
    CoreObject *obj = events->head;

    while (obj != NULL) {
        CoreObject *next = obj->next;
        const VtblEntry *dtor = &((const VtblEntry *)obj->vtable)[1];

        ((void (*)(void *, s32))dtor->fn)((u8 *)obj + dtor->delta, 3);
        obj = next;
    }
    events->tail = NULL;
    events->head = NULL;
    events->count = 0;
}
