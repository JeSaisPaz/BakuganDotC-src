// bdc 0x0890925c BtlDemoScbEventRead
#include "bdc.h"

/* Reads one record into a `.scb` event node (`BtlDemoScbEvent`, built by
   `BtlDemoScbEventGroupReadEvents`): frame = rec[1], flags = rec[0], then calls the class's body
   parser (vtable entry 2) with the same record; returns the record size (parser result + 4). */

int BtlDemoScbEventRead(BtlDemoScbEvent *ev, u16 *rec)
{
  const VtblEntry *parse = &((const VtblEntry *)ev->base.vtable)[2];

  ev->frame = rec[1];
  ev->flags = rec[0];
  return ((int (*)(void *, u16 *))parse->fn)((u8 *)ev + parse->delta, rec) + 4;
}
