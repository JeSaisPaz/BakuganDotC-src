// bdc 0x08908430 BtlDemoScbEventTableClear
#include "bdc.h"

/* Frees the nine event lists `lists[0..8]` of a `.scb` event table, in order, through
   `BtlDemoScbEventListFree` (which deletes each list's nodes and its list head). Called by
   `BtlDemoScbEventTableDtor`. */
void BtlDemoScbEventTableClear(BtlDemoScbEventTable *tbl)
{
    s32 i;

    for (i = 0; i < 9; i++) {
        BtlDemoScbEventListFree(tbl, tbl->lists[i]);
    }
}
