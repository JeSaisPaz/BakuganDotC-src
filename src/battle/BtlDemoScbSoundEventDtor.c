// bdc 0x08a2d0c8 BtlDemoScbSoundEventDtor
#include "bdc.h"

/* Destructor (`g_btlDemoScbSoundEventVtbl` entry 1) of `BtlDemoScbSoundEvent`: reinstalls the
   vtable, runs the event base destructor `BtlDemoScbEventDtor` and frees the object when
   `flags & 1`. Does nothing for NULL. */
void BtlDemoScbSoundEventDtor(BtlDemoScbSoundEvent *ev, u32 flags)
{
    if (ev != NULL) {
        ev->base.base.vtable = g_btlDemoScbSoundEventVtbl;
        BtlDemoScbEventDtor(&ev->base, 0);
        if ((flags & 1) != 0) {
            MemLock();
            MemFree(ev, NULL, 0);
            MemUnlock();
        }
    }
}
