// bdc 0x08a2e724 SndEmitterNodeGetData
#include "bdc.h"

/* Returns the data pointer (`+8`) of an emitter-list node, normally a `SndEmitter` (or, for the
   group scratch lists, also a `SndEmitter`). */

SndEmitter *SndEmitterNodeGetData(CorePrioNode *node)

{
  return node->data;
}

