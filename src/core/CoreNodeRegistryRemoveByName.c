// bdc 0x089d8ec8 CoreNodeRegistryRemoveByName
#include "bdc.h"

/* Destroys every manager registered in `g_coreNodeRoot` whose name key
   (`CoreNodeRegistryEntry` `name`, compared by pointer first, then with `strcmp`) equals `name`,
   using the node's virtual destructor (vtable entry 1) with `flags = 3`; when no registered manager
   remains it destroys the root as well and clears `g_coreNodeRoot`. */
void CoreNodeRegistryRemoveByName(const char *name)
{
    CoreNodeRegistryEntry *entry;
    CoreNode *next;
    const VtblEntry *dtor;
    bool match;

    if (g_coreNodeRoot == NULL) {
        return;
    }
    for (entry = (CoreNodeRegistryEntry *)g_coreNodeRoot->next; entry != NULL;
         entry = (CoreNodeRegistryEntry *)next) {
        match = false;
        if (entry->name == name) {
            match = true;
        } else if (strcmp(entry->name, name) == 0) {
            match = true;
        }
        next = entry->base.next;
        if (match) {
            dtor = &((const VtblEntry *)entry->base.vtable)[1];
            ((void (*)(void *, s32))dtor->fn)((u8 *)entry + dtor->delta, 3);
        }
    }
    if (g_coreNodeRoot->next == NULL && g_coreNodeRoot != NULL) {
        dtor = &((const VtblEntry *)g_coreNodeRoot->vtable)[1];
        ((void (*)(void *, s32))dtor->fn)((u8 *)g_coreNodeRoot + dtor->delta, 3);
        g_coreNodeRoot = NULL;
    }
}
