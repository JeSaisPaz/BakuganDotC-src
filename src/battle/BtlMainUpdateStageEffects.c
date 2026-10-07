// bdc 0x0884c320 BtlMainUpdateStageEffects
#include "bdc.h"

/* Runs the stage's per-frame effect updates: `BtlStageUpdateAmbientEffects`, then
   `GfxEffectMgrUpdateOwner` on the stage effect manager `stageEffects` when it exists, then
   `GfxMeshObjUpdateAll`. */
void BtlMainUpdateStageEffects(BtlMain *self)
{
    BtlStageUpdateAmbientEffects();
    if (self->stageEffects != NULL) {
        GfxEffectMgrUpdateOwner(self->stageEffects);
    }
    GfxMeshObjUpdateAll();
}
