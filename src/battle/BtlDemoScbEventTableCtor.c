// bdc 0x0890833c BtlDemoScbEventTableCtor
#include "bdc.h"

/* Constructor of a 0x40-byte `.scb` event table (`BtlDemoScbEventTable`): `CoreObjectInit`
   with no parent, installs the vtable `g_btlDemoScbEventTableVtbl`, then
   `BtlDemoScbEventTableInit` and `BtlDemoScbEventTableBuild` from `data`. Returns `tbl`. */
CoreObject *BtlDemoScbEventTableCtor(CoreObject *tbl, u16 *data)
{
    CoreObjectInit(tbl, NULL);
    tbl->vtable = g_btlDemoScbEventTableVtbl;
    BtlDemoScbEventTableInit((BtlDemoScbEventTable *)tbl);
    BtlDemoScbEventTableBuild((BtlDemoScbEventTable *)tbl, data);
    return tbl;
}
