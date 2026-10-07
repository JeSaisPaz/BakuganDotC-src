// bdc 0x08a2cef0 BtlDemoScbObjectDtor
#include "bdc.h"

/* Destructor (`g_btlDemoScbObjectVtbl` entry 1) of a `.scb` scene object entry
   (`BtlDemoScbObjectCtor`): reinstalls that vtable, runs `CoreObjectDtor` (unlink, no free)
   and frees the object when `flags & 1`. Does nothing for NULL. */
void BtlDemoScbObjectDtor(CoreObject *obj, u32 flags)
{
    if (obj != NULL) {
        obj->vtable = g_btlDemoScbObjectVtbl;
        CoreObjectDtor(obj, 0);
        if ((flags & 1) != 0) {
            MemLock();
            MemFree(obj, NULL, 0);
            MemUnlock();
        }
    }
}
