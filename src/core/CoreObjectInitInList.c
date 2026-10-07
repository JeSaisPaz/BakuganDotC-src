// bdc 0x089d8734 CoreObjectInitInList
#include "bdc.h"

/* Constructor of the `CoreObject` base for objects kept in a head/tail/count list: sets the base
   vtable `g_coreObjectVtbl`, clears the links, appends the object to `list`
   (`CoreObjectListAppend`, which also sets the owner `+0x10`), clears `unk08` and assigns the
   next id (`++``g_coreObjectCounter`). Returns `obj`. */
CoreObject *CoreObjectInitInList(CoreObject *obj, void *list)
{
    obj->vtable = g_coreObjectVtbl;
    obj->prev = NULL;
    obj->next = NULL;
    CoreObjectListAppend(obj, list);
    obj->unk08 = 0;
    obj->id = ++g_coreObjectCounter;
    return obj;
}
