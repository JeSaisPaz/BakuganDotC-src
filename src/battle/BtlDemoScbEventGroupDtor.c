// bdc 0x08a2cf64 BtlDemoScbEventGroupDtor
#include "bdc.h"

/* Deleting destructor (`g_btlDemoScbEventGroupVtbl` entry 1) of a `.scb` event group node
   (`BtlDemoScbEventGroupCtor`): does nothing for NULL; otherwise re-installs that vtable, runs
   `CoreObjectDtor` with flags 0 and frees the object when bit 0 of `flags` is set. */
void BtlDemoScbEventGroupDtor(CoreObject *grp, u32 flags)
{
    if (grp == NULL) {
        return;
    }
    grp->vtable = g_btlDemoScbEventGroupVtbl;
    CoreObjectDtor(grp, 0);
    if (flags & 1) {
        MemLock();
        MemFree(grp, NULL, 0);
        MemUnlock();
    }
}
