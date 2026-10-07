// bdc 0x0885b72c BtlLoadRequestDtor
#include "bdc.h"

/* Destructor (`g_btlLoadRequestVtbl` entry 1) of a `BtlLoadRequest`: reinstalls the vtable,
   deletes each non-NULL package slot in order through its `CorePack` vtable (entry 1, the
   deleting destructor, flags 3) and clears the slot, runs `CoreObjectDtor``(req, 0)` and frees
   the object when `flags & 1`. Does nothing for NULL. */
void BtlLoadRequestDtor(BtlLoadRequest *req, u32 flags)
{
    int i;

    if (req == NULL) {
        return;
    }
    req->base.vtable = g_btlLoadRequestVtbl;
    for (i = 0; i < 4; i++) {
        CorePack *pkg = (CorePack *)req->packages[i];

        if (pkg != NULL) {
            const VtblEntry *dtor = &((const VtblEntry *)pkg->base.vtable)[1];

            ((void (*)(void *, s32))dtor->fn)((u8 *)pkg + dtor->delta, 3);
            req->packages[i] = NULL;
        }
    }
    CoreObjectDtor(&req->base, 0);
    if ((flags & 1) != 0) {
        MemLock();
        MemFree(req, NULL, 0);
        MemUnlock();
    }
}
