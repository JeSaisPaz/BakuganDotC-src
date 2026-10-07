// bdc 0x0883a67c BtlHudCreateTextObject
#include "bdc.h"

/* Allocates a 0xb0-byte .fab object from the low end of the heap (restoring the previous
   allocation side), constructs it from the pack file `name` into the HUD's fab object list
   (GfxFabCtorFromPack), and stores it in `fabs[slot]`; stores NULL when the allocation failed. Returns the stored object. */

GfxFab *BtlHudCreateTextObject(BtlHud *self, char *name, int slot)
{
    bool fromLow;
    void *fab;
    GfxFab *obj = NULL;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    fab = MemAlloc(0xb0, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (fab != NULL) {
        GfxFabCtorFromPack(fab, name, &self->fabList);
        obj = fab;
    }
    self->fabs[slot] = obj;
    return obj;
}
