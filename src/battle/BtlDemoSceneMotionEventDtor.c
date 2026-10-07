// bdc 0x0890786c BtlDemoSceneMotionEventDtor
#include "bdc.h"

/* Destructor of a demo scene motion event node (`BtlDemoSceneMotionEvent`, built by
   `BtlDemoSceneMotionEventCtor`): reinstalls `g_btlDemoSceneMotionEventVtbl`, runs the
   `CoreObject` destructor `CoreObjectDtor` (unlink, no free) and frees the node when
   `flags & 1`. Does nothing for NULL. */
void BtlDemoSceneMotionEventDtor(void *node, u32 flags)
{
    BtlDemoSceneMotionEvent *self = (BtlDemoSceneMotionEvent *)node;

    if (self != NULL) {
        self->base.vtable = g_btlDemoSceneMotionEventVtbl;
        CoreObjectDtor(&self->base, 0);
        if ((flags & 1) != 0) {
            MemLock();
            MemFree(self, NULL, 0);
            MemUnlock();
        }
    }
}
