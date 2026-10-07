// bdc 0x08a30404 SndEmitterGroupNodeGetData
#include "bdc.h"

/* Returns the payload (`+8`) of a node of the emitter-group list: the emitter list
   (`CorePrioList`) that forms one exclusive group. */

CorePrioList *SndEmitterGroupNodeGetData(CorePrioNode *node)

{
  return node->data;
}

