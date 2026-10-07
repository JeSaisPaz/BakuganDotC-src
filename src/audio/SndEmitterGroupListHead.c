// bdc 0x08a2ff38 SndEmitterGroupListHead
#include "bdc.h"

/* Returns the sentinel node of the live chain (`list+0`) of the emitter-group list; the first real
   group is its `next` (`SndEmitterGroupNodeGetNext`). Used by `SndEmitterGroupSelectNearest`
   and `SndReleaseAll` to start their walks. */

CorePrioNode *SndEmitterGroupListHead(CorePrioList *list)

{
  return list->active;
}

