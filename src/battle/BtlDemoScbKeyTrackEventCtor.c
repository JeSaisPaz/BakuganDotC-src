// bdc 0x08909638 BtlDemoScbKeyTrackEventCtor
#include "bdc.h"

/* Constructor of the key-track event `BtlDemoScbKeyTrackEvent` (0x34 bytes, vtable
   `g_btlDemoScbKeyTrackEventVtbl`; built by `BtlDemoScbEventGroupReadEvents`): runs the base
   constructor `BtlDemoScbEventCtor`, installs its own vtable and clears the key array, the key
   count and the length (in that order). Returns `ev`. */
void *BtlDemoScbKeyTrackEventCtor(void *ev, void *list)
{
    BtlDemoScbKeyTrackEvent *self = ev;

    BtlDemoScbEventCtor(&self->base, list);
    self->base.base.vtable = g_btlDemoScbKeyTrackEventVtbl;
    self->keys = NULL;
    self->keyCount = 0;
    self->length = 0;
    return ev;
}
