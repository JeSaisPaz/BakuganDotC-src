// bdc 0x08909678 BtlDemoScbKeyTrackEventDtor
#include "bdc.h"

/* Deleting destructor (`g_btlDemoScbKeyTrackEventVtbl` entry 1) of `BtlDemoScbKeyTrackEvent`:
   does nothing for NULL; otherwise re-installs that vtable, frees the key array `keys` when set
   (its `CxxVecBlock`, which starts at the 0x10-byte cookie) and clears it, runs
   `BtlDemoScbEventDtor` with flags 0 and frees the event when bit 0 of `flags` is set. */
void BtlDemoScbKeyTrackEventDtor(BtlDemoScbKeyTrackEvent *ev, u32 flags)
{
    void *keys;

    if (ev == NULL) {
        return;
    }
    keys = ev->keys;
    ev->base.base.vtable = g_btlDemoScbKeyTrackEventVtbl;
    if (keys != NULL) {
        MemLock();
        MemFree((CxxVecBlock *)((u8 *)keys - __builtin_offsetof(CxxVecBlock, elements)), NULL, 0);
        MemUnlock();
        ev->keys = NULL;
    }
    BtlDemoScbEventDtor(ev, 0);
    if (flags & 1) {
        MemLock();
        MemFree(ev, NULL, 0);
        MemUnlock();
    }
}
