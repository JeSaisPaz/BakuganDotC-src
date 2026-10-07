// bdc 0x08a2ff0c SndEmitterGroupListRewind
#include "bdc.h"

/* Rewinds the iteration cursor of the emitter-group list: `list->cursor = first real node`
   (`GetNext` of the live-chain sentinel at `list+0`, stored at `list+8`). Called at the end of
   `SndEmitterGroupListFlush`. */
void SndEmitterGroupListRewind(CorePrioList *list)
{
    list->cursor = SndEmitterGroupNodeGetNext(list->active);
}
