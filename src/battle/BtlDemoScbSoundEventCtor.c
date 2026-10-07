// bdc 0x08909540 BtlDemoScbSoundEventCtor
#include "bdc.h"

/* Constructor of `BtlDemoScbSoundEvent` (0x34 bytes): runs the base `BtlDemoScbEventCtor`
   (init, append to `list`), installs `g_btlDemoScbSoundEventVtbl` and clears the four halfword
   parameters and the value word. Returns `ev`. */
BtlDemoScbSoundEvent *BtlDemoScbSoundEventCtor(BtlDemoScbSoundEvent *ev, CoreObjectList *list)
{
    BtlDemoScbEventCtor(&ev->base, list);
    ev->base.base.vtable = g_btlDemoScbSoundEventVtbl;
    ev->paramA = 0;
    ev->paramB = 0;
    ev->paramC = 0;
    ev->paramD = 0;
    ev->value = 0;
    return ev;
}
