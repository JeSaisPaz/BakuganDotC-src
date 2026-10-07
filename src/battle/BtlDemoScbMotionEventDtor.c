// bdc 0x08909400 BtlDemoScbMotionEventDtor
#include "bdc.h"

/* Destructor (`g_btlDemoScbMotionEventVtbl` entry 1) of `BtlDemoScbMotionEvent`: reinstalls the
   vtable, runs the event base destructor `BtlDemoScbEventDtor` and frees the node when
   `flags & 1`. Does nothing for NULL. */
void BtlDemoScbMotionEventDtor(BtlDemoScbMotionEvent *ev, u32 flags)
{
    if (ev != NULL) {
        ev->base.base.vtable = g_btlDemoScbMotionEventVtbl;
        BtlDemoScbEventDtor(&ev->base, 0);
        if ((flags & 1) != 0) {
            MemLock();
            MemFree(ev, NULL, 0);
            MemUnlock();
        }
    }
}
