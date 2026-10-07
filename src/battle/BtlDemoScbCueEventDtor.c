// bdc 0x08a2cfe0 BtlDemoScbCueEventDtor
#include "bdc.h"

/* Deleting destructor of the cue `.scb` event (`BtlDemoScbEvent` with no own fields, built by
   `BtlDemoScbCueEventCtor`; entry 1 of `g_btlDemoScbCueEventVtbl`): reinstalls the cue vtable
   `g_btlDemoScbCueEventVtbl`, runs the event base destructor `BtlDemoScbEventDtor` without
   freeing, and frees the object when `flags & 1`. Does nothing for NULL. */
void BtlDemoScbCueEventDtor(void *ev, u32 flags)
{
    BtlDemoScbEvent *self = (BtlDemoScbEvent *)ev;

    if (self != NULL) {
        self->base.vtable = g_btlDemoScbCueEventVtbl;
        BtlDemoScbEventDtor(self, 0);
        if ((flags & 1) != 0) {
            MemLock();
            MemFree(self, NULL, 0);
            MemUnlock();
        }
    }
}
