// bdc 0x08a2d13c BtlDemoScbCueEventBDtor
#include "bdc.h"

/* Deleting destructor of the second bodiless cue `.scb` event (`BtlDemoScbEvent` with no own
   fields, built by `BtlDemoScbCueEventBCtor`; entry 1 of `g_btlDemoScbCueEventBVtbl`):
   reinstalls that vtable, runs the event base destructor `BtlDemoScbEventDtor` without freeing,
   and frees the object when `flags & 1`. Does nothing for NULL. */
void BtlDemoScbCueEventBDtor(void *ev, u32 flags)
{
    BtlDemoScbEvent *self = (BtlDemoScbEvent *)ev;

    if (self != NULL) {
        self->base.vtable = g_btlDemoScbCueEventBVtbl;
        BtlDemoScbEventDtor(self, 0);
        if ((flags & 1) != 0) {
            MemLock();
            MemFree(self, NULL, 0);
            MemUnlock();
        }
    }
}
