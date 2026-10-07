// bdc 0x08a319d0 NetErrorListInsertSorted
#include "bdc.h"

/* Links `node` into the chain starting at sentinel `head`, before the first node with a higher
   priority. Counterpart of `CorePrioListInsertSorted` in the net-error list instance. */

void NetErrorListInsertSorted(CorePrioList *list, CorePrioNode *head, CorePrioNode *node)

{
  CorePrioNode *prev;
  s32 nodePrio;
  s32 headPrio;
  
  do {
    while( true ) {
      prev = head;
      if (prev == (CorePrioNode *)0x0) {
        return;
      }
      head = NetErrorNodeGetNext(prev);
      if (head != (CorePrioNode *)0x0) break;
      NetErrorNodeSetNext(prev,node);
      NetErrorNodeSetNext(node,(CorePrioNode *)0x0);
    }
    nodePrio = NetErrorNodeGetPriority(node);
    headPrio = NetErrorNodeGetPriority(head);
  } while (headPrio <= nodePrio);
  NetErrorNodeSetNext(prev,node);
  NetErrorNodeSetNext(node,head);
  return;
}

