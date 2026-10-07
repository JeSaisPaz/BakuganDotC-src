// bdc 0x08a32168 NetErrorNodeSetNext
#include "bdc.h"

/* Stores the next link (`+0xc`) of a net-error list node. Counterpart of `CorePrioNodeSetNext`.
    */

void NetErrorNodeSetNext(CorePrioNode *node, CorePrioNode *next)

{
  node->next = next;
  return;
}

