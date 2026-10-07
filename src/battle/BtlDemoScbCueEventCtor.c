// bdc 0x0890929c BtlDemoScbCueEventCtor
#include "bdc.h"

/* Constructor of the bodiless cue event (0x28 bytes, a plain `BtlDemoScbEvent` with vtable
   `g_btlDemoScbCueEventVtbl`; built by `BtlDemoScbEventGroupReadEvents`): runs the base
   constructor `BtlDemoScbEventCtor` and installs its own vtable. Its body parser is
   `BtlDemoScbCueEventParse`. Returns `ev`. */
void *BtlDemoScbCueEventCtor(void *ev, void *list)
{
    BtlDemoScbEvent *self = ev;

    BtlDemoScbEventCtor(self, list);
    self->base.vtable = g_btlDemoScbCueEventVtbl;
    return ev;
}
