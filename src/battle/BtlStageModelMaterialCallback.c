// bdc 0x0889cb6c BtlStageModelMaterialCallback
#include "bdc.h"

/* Material callback of `BtlStageSetupModelMaterials` for the arena models: when bits 2..3 of
   the render flags are 0 it sets them to 2 (value 8), leaving materials with an explicit mode
   alone. */
void BtlStageModelMaterialCallback(void *matState, void *arg)
{
    GfxMaterialState *state = matState;

    (void)arg;
    if ((state->renderFlags & 0xc) == 0) {
        state->renderFlags = (state->renderFlags & 0xf3) | 8;
    }
}
