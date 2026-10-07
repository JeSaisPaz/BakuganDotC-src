// bdc 0x08a32170 NetErrorNodeGetNext
#include "bdc.h"

/* Returns the next link (`+0xc`) of a net-error list node. Counterpart of `CorePrioNodeGetNext`.
    */

CorePrioNode *NetErrorNodeGetNext(CorePrioNode *node)

{
  return node->next;
}

