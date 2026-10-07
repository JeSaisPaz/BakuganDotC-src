// bdc 0x089d878c CoreObjectDtor
#include "bdc.h"

/* Destructor of the `CoreObject` base: resets the vtable to the base one
   (`g_coreObjectVtbl`), unlinks the object (`CoreObjectUnlink`) and frees it when bit 0 of
   `flags` is set. A NULL `obj` is ignored. Chained to by about 25 derived destructors. */
void CoreObjectDtor(CoreObject *obj, u32 flags)
{
    if (obj == NULL)
        return;
    obj->vtable = g_coreObjectVtbl;
    CoreObjectUnlink(obj);
    if (flags & 1) {
        MemLock();
        MemFree(obj, NULL, 0);
        MemUnlock();
    }
}
