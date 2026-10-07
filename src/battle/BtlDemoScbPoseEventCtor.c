// bdc 0x089092d8 BtlDemoScbPoseEventCtor
#include "bdc.h"

/* Constructor of the two-value event `BtlDemoScbPoseEvent` (0x2c bytes, vtable
   `g_btlDemoScbPoseEventVtbl`; built by `BtlDemoScbEventGroupReadEvents`): runs the base
   constructor `BtlDemoScbEventCtor`, installs its own vtable and sets both values to -1.
   Returns `ev`. */
void *BtlDemoScbPoseEventCtor(void *ev, void *list)
{
    BtlDemoScbPoseEvent *self = ev;

    BtlDemoScbEventCtor(&self->base, list);
    self->base.base.vtable = g_btlDemoScbPoseEventVtbl;
    self->valueA = -1;
    self->valueB = -1;
    return ev;
}
