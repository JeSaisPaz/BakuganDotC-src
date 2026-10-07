// bdc 0x08909190 BtlDemoScbEventCtor
#include "bdc.h"

/* Base constructor of a `.scb` event node (`CoreObject`-based; built by
   `BtlDemoScbEventGroupReadEvents`): `CoreObjectInit`, installs `g_btlDemoScbEventVtbl`,
   clears the frame, stores the owning list `list` and appends the node to it
   (`CoreObjectListAppend`). Returns `ev`. */
BtlDemoScbEvent *BtlDemoScbEventCtor(BtlDemoScbEvent *ev, CoreObjectList *list)
{
    CoreObjectInit(&ev->base, NULL);
    ev->base.vtable = g_btlDemoScbEventVtbl;
    ev->frame = 0;
    ev->list = list;
    CoreObjectListAppend(&ev->base, list);
    return ev;
}
