// bdc 0x08a2df6c SndEmitterListInsert
#include "bdc.h"

/* Adds `data` with sort key `priority` to the emitter list: takes a node from the list's
   `MemPool` (or the low heap), initialises it (`SndEmitterNodeInit`, `SndEmitterNodeSetData`,
   `SndEmitterNodeSetPriority`) and links it into the pending chain with
   `SndEmitterListLinkSorted`; it becomes visible to iteration after the next
   `SndEmitterListFlush`. Returns the node. If allocation fails the node is NULL and the
   setters and the link are still called with NULL (no check in the original). */

CorePrioNode *SndEmitterListInsert(CorePrioList *list, SndEmitter *data, s32 priority)
{
  bool fromLow;
  CorePrioNode *heapNode;
  CorePrioNode *node;

  node = (CorePrioNode *)0x0;
  if (list->pool != (MemPool *)0x0) {
    node = MemPoolAlloc(list->pool);
  }
  if (node == (CorePrioNode *)0x0) {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    heapNode = MemAlloc(sizeof(CorePrioNode), (const char *)0x0, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    node = (CorePrioNode *)0x0;
    if (heapNode != (CorePrioNode *)0x0) {
      SndEmitterNodeInit(heapNode);
      node = heapNode;
    }
  }
  else {
    SndEmitterNodeInit(node);
  }
  SndEmitterNodeSetData(node, data);
  SndEmitterNodeSetPriority(node, priority);
  SndEmitterListLinkSorted(list, list->pending, node);
  return node;
}
