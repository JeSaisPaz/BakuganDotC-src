// bdc 0x08a30424 SndEmitterGroupNodeMarkRemoved
#include "bdc.h"

/* Flags a node of the emitter-group list as removed by setting its `state` (`+0`) to 2; walkers
   skip it and `SndEmitterGroupListFlush` deletes it. */

void SndEmitterGroupNodeMarkRemoved(CorePrioNode *node)

{
  node->state = 2;
  return;
}

