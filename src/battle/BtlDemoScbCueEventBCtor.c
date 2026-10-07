// bdc 0x089095fc BtlDemoScbCueEventBCtor
#include "bdc.h"

/* Constructor of the second bodiless `.scb` cue event class (0x28 bytes): runs the base
   `BtlDemoScbEventCtor` (init, append to `list`) and installs `g_btlDemoScbCueEventBVtbl`;
   body parser `BtlDemoScbCueEventBParse`. Returns `ev`. */
BtlDemoScbEvent *BtlDemoScbCueEventBCtor(BtlDemoScbEvent *ev, CoreObjectList *list)
{
    BtlDemoScbEventCtor(ev, list);
    ev->base.vtable = g_btlDemoScbCueEventBVtbl;
    return ev;
}
