// bdc 0x089d9ea4 GmoMotionGetDataByIndex
#include "bdc.h"

/* Returns the data block (`GmoMotionInfo`) of the registry motion with the given index: the entry
   is found with the callback stored at `mgr+0x10` when one is installed, otherwise with
   `GmoMotionAt`; the data comes from the entry's virtual "get data" method (vtable entry 2). NULL
   when there is no such entry. */

void *GmoMotionGetDataByIndex(void *mgr, s32 index)
{
  GmoMotionMgr *m = (GmoMotionMgr *)mgr;
  CoreNode *node;
  const VtblEntry *getData;

  if (m->lookupCb != NULL) {
    node = m->lookupCb(index, mgr);
  } else {
    node = GmoMotionAt(mgr, index);
  }
  if (node == NULL) {
    return NULL;
  }
  getData = &((const VtblEntry *)node->vtable)[2];
  return ((void *(*)(void *))getData->fn)((u8 *)node + getData->delta);
}
