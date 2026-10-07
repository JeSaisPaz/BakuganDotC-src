// bdc 0x089d88f8 CoreObjectListDeleteAll
#include "bdc.h"

/* Deletes every object of a `CoreObject` list holder (`CoreObjectList`) through the virtual
   deleting destructor (vtable `+0xc`, flags 3). An inlined copy of `CoreObjectListClear`. */
void CoreObjectListDeleteAll(CoreObjectList *list)
{
    CoreObject *obj = list->head;
    CoreObject *next;

    while (obj != NULL) {
        next = obj->next;
        {
            /* virtual deleting destructor: vtable entry 1, flags 3 */
            const VtblEntry *dtor = &((const VtblEntry *)obj->vtable)[1];
            ((void (*)(void *, s32))dtor->fn)((u8 *)obj + dtor->delta, 3);
        }
        obj = next;
    }
}
