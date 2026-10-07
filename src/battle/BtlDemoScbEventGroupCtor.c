// bdc 0x08907d68 BtlDemoScbEventGroupCtor
#include "bdc.h"

/* Constructor of a `.scb` event group node (`BtlDemoScbEventGroup`, vtable
   `g_btlDemoScbEventGroupVtbl`): initialises the `CoreObject` base unchained, copies the
   12-byte group header `hdr` into `header`, copies the table's event lists `tbl->lists[1..8]` into
   `lists[0..7]`, then parses the group's records (`BtlDemoScbEventGroupParse`). Returns `grp`. */
BtlDemoScbEventGroup *BtlDemoScbEventGroupCtor(BtlDemoScbEventGroup *grp, u16 *hdr,
                                               BtlDemoScbEventTable *tbl)
{
    s32 i;

    CoreObjectInit(&grp->base, NULL);
    grp->base.vtable = g_btlDemoScbEventGroupVtbl;
    /* the binary moves the header as three 32-bit words */
    for (i = 0; i < 6; i++) {
        grp->header[i] = hdr[i];
    }
    for (i = 0; i < 8; i++) {
        grp->lists[i] = tbl->lists[i + 1];
    }
    BtlDemoScbEventGroupParse(grp, hdr);
    return grp;
}
