// bdc 0x08908394 BtlDemoScbEventListFree
#include "bdc.h"

/* Deletes every event object of a `.scb` event list (a `CoreObjectList`), head to tail, through
   its virtual deleting destructor (vtable entry 1, flags 3), then frees the 12-byte list head itself
   under `MemLock`. Does nothing for a NULL `list`; `tbl` (the owning event table) is unused. */
void BtlDemoScbEventListFree(void *tbl, void *list)
{
  CoreObjectList *events = (CoreObjectList *)list;
  CoreObject *node;
  CoreObject *next;
  const VtblEntry *dtor;

  (void)tbl;
  if (events == NULL) {
    return;
  }
  node = events->head;
  while (node != NULL) {
    next = node->next;
    dtor = &((const VtblEntry *)node->vtable)[1];
    ((void (*)(void *, s32))dtor->fn)((u8 *)node + dtor->delta, 3);
    node = next;
  }
  MemLock();
  MemFree(events, NULL, 0);
  MemUnlock();
}
