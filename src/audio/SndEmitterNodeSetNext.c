// bdc 0x08a2e73c SndEmitterNodeSetNext
#include "bdc.h"

/* Sets the `next` pointer (`+0xc`) of an emitter-list node. */

void SndEmitterNodeSetNext(CorePrioNode *node, CorePrioNode *next)

{
  node->next = next;
  return;
}

