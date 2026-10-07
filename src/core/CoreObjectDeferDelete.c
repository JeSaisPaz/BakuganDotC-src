// bdc 0x089de910 CoreObjectDeferDelete
#include "bdc.h"

/* Queues a `CoreObject` for deferred deletion: unless it is already in
   `g_coreObjectDeferDeleteList`, unlinks it (`CoreObjectUnlink`) and appends it there
   (`CoreObjectListAppend`), storing `delay` in its `id` field (non-zero = survive one more
   flush). Deleted by `CoreObjectDeferDeleteFlush`. */
void CoreObjectDeferDelete(CoreObject *obj, u32 delay)
{
    if (CoreObjectListContains(&g_coreObjectDeferDeleteList.head, obj) == 0) {
        CoreObjectUnlink(obj);
        CoreObjectListAppend(obj, &g_coreObjectDeferDeleteList);
        obj->id = delay;
    }
}
