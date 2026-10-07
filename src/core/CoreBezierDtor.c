// bdc 0x0889f920 CoreBezierDtor
#include "bdc.h"

/* Destructor of the `CoreBezierCtor` helper: reinstalls its vtable `g_coreBezierVtbl` and
   frees the object under `MemLock` when bit 0 of `flags` is set (GCC 2.x deleting-destructor
   convention). NULL `self` does nothing. */
void CoreBezierDtor(CoreBezier *self, u32 flags)
{
    if (self == NULL)
        return;
    self->vtbl = g_coreBezierVtbl;
    if ((flags & 1) != 0) {
        MemLock();
        MemFree(self, NULL, 0);
        MemUnlock();
    }
}
