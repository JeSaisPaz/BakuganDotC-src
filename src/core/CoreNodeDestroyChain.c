// bdc 0x089d8d78 CoreNodeDestroyChain
#include "bdc.h"

/* Destroys every node of a sibling chain starting at `first`: for each node it saves `next`, then
   calls the node's virtual destructor (vtable entry 1, `this` adjusted by the entry's delta) with
   `flags = 3`, i.e. destroy and free. */
void CoreNodeDestroyChain(CoreNode *first)
{
    while (first != NULL) {
        CoreNode *next = first->next;
        const VtblEntry *dtor = &((const VtblEntry *)first->vtable)[1];

        ((void (*)(void *, s32))dtor->fn)((u8 *)first + dtor->delta, 3);
        first = next;
    }
}
