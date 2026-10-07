// bdc 0x08a2e74c SndEmitterNodeMarkRemoved
#include "bdc.h"

/* Flags an emitter-list node as removed (`state = 2`). The node stays linked until
   `SndEmitterListFlush` / `SndEmitterListClear` unlinks and frees it, and iteration skips it.
    */

void SndEmitterNodeMarkRemoved(CorePrioNode *node)

{
  node->state = 2;
  return;
}

