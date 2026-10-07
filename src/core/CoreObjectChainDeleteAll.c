// bdc 0x089d88a4 CoreObjectChainDeleteAll
#include "bdc.h"

/* Walks a `CoreObject` chain from `first` through `next` and deletes every object with its
   virtual deleting destructor (vtable entry 1, flags 3), reading `next` before each delete. */
void CoreObjectChainDeleteAll(CoreObject *first)
{
    CoreObject *next;

    while (first != NULL) {
        next = first->next;
        {
            /* virtual deleting destructor: vtable entry 1, flags 3 */
            const VtblEntry *dtor = &((const VtblEntry *)first->vtable)[1];
            ((void (*)(void *, s32))dtor->fn)((u8 *)first + dtor->delta, 3);
        }
        first = next;
    }
}
