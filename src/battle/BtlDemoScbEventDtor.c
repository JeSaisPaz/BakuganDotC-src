// bdc 0x089091e8 BtlDemoScbEventDtor
#include "bdc.h"

/* Base destructor of a `.scb` demo event node (`BtlDemoScbEvent`, built by
   `BtlDemoScbEventCtor`): reinstalls the base vtable `g_btlDemoScbEventVtbl`, runs the
   `CoreObject` destructor `CoreObjectDtor` (unlink, no free) and frees the node when
   `flags & 1`. Does nothing for NULL. */
void BtlDemoScbEventDtor(void *ev, u32 flags)
{
    BtlDemoScbEvent *self = (BtlDemoScbEvent *)ev;

    if (self != NULL) {
        self->base.vtable = g_btlDemoScbEventVtbl;
        CoreObjectDtor(&self->base, 0);
        if ((flags & 1) != 0) {
            MemLock();
            MemFree(self, NULL, 0);
            MemUnlock();
        }
    }
}
