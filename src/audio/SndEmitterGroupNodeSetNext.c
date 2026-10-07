// bdc 0x08a30414 SndEmitterGroupNodeSetNext
#include "bdc.h"

/* Stores `next` into the link field (`+0xc`) of a node of the emitter-group list. */

void SndEmitterGroupNodeSetNext(CorePrioNode *node, CorePrioNode *next)

{
  node->next = next;
  return;
}

