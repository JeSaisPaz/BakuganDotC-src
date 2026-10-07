// bdc 0x08a01aac GfxFabObjectDtor
#include "bdc.h"

/* Destructor of a placed `.fab` object (vtable `g_gfxFabObjectVtbl` slot 1): restores the vtable, runs
   `CoreObjectDtor` and frees the object when `flags & 1`. Counterpart of `GfxFabObjectCtor`. */

void GfxFabObjectDtor(CoreObject *obj, u32 flags)
{
    if (obj != NULL) {
        obj->vtable = g_gfxFabObjectVtbl;
        CoreObjectDtor(obj, 0);
        if ((flags & 1) != 0) {
            MemLock();
            MemFree(obj, NULL, 0);
            MemUnlock();
        }
    }
}
