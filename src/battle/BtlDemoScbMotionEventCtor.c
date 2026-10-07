// bdc 0x0890939c BtlDemoScbMotionEventCtor
#include "bdc.h"

/* Constructor of `BtlDemoScbMotionEvent` (0x3c bytes): runs the base `BtlDemoScbEventCtor`
   (init, append to `list`), installs `g_btlDemoScbMotionEventVtbl`, sets the blend time to 0.1 and
   clears the motion id, parameters and flags. Returns `ev`. */
BtlDemoScbMotionEvent *BtlDemoScbMotionEventCtor(BtlDemoScbMotionEvent *ev, CoreObjectList *list)
{
    BtlDemoScbEventCtor(&ev->base, list);
    ev->base.base.vtable = g_btlDemoScbMotionEventVtbl;
    ev->motionId = 0;
    ev->motionParam = 0;
    ev->motionFlag = 0;
    ev->flag0 = 0;
    ev->blendTime = 0.1f;
    ev->range2B = 0;
    ev->range2A = 0;
    ev->range4B = 0;
    ev->range4A = 0;
    return ev;
}
