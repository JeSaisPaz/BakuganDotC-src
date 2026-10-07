// bdc 0x08a32178 NetErrorNodeMarkRemoved
#include "bdc.h"

/* Flags a net-error list node as removed (state `+0x0` = 2); `NetErrorListMerge` frees it later.
   Counterpart of `CorePrioNodeMarkRemoved`. */

void NetErrorNodeMarkRemoved(CorePrioNode *node)

{
  node->state = 2;
  return;
}

