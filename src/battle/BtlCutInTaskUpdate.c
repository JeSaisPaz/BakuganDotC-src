// bdc 0x0885486c BtlCutInTaskUpdate
#include "bdc.h"

/* Update slot (2) of the cut-in task (`BtlCutInTask`, id 0x1e1, `BtlCutInTaskCtor`):
   increments the frame counter `frame` and updates its effect manager `effects`
   (`GfxEffectMgrUpdate`) when that holds any effect (`base.count != 0`). */
void BtlCutInTaskUpdate(void *task)
{
    BtlCutInTask *self = task;

    self->frame++;
    if (self->effects->base.count != 0) {
        GfxEffectMgrUpdate(self->effects);
    }
}
