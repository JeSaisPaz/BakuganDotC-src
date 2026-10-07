// bdc 0x08a32150 NetErrorNodeGetData
#include "bdc.h"

/* Returns the payload pointer (`+0x8`) of a net-error list node. Counterpart of
   `CorePrioNodeGetData`. */

void *NetErrorNodeGetData(CorePrioNode *node)

{
  return node->data;
}

