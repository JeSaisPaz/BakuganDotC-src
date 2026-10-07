// bdc 0x08a2d054 BtlDemoScbPoseEventDtor
#include "bdc.h"

/* Deleting destructor of the Pose `.scb` event (`BtlDemoScbPoseEvent`, built by
   `BtlDemoScbPoseEventCtor`; entry 1 of `g_btlDemoScbPoseEventVtbl`): reinstalls the Pose
   vtable `g_btlDemoScbPoseEventVtbl`, runs the event base destructor `BtlDemoScbEventDtor`
   without freeing, and frees the object when `flags & 1`. Does nothing for NULL. */
void BtlDemoScbPoseEventDtor(void *ev, u32 flags)
{
    BtlDemoScbPoseEvent *self = (BtlDemoScbPoseEvent *)ev;

    if (self != NULL) {
        self->base.base.vtable = g_btlDemoScbPoseEventVtbl;
        BtlDemoScbEventDtor(self, 0);
        if ((flags & 1) != 0) {
            MemLock();
            MemFree(self, NULL, 0);
            MemUnlock();
        }
    }
}
