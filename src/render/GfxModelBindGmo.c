// bdc 0x089dfabc GfxModelBindGmo
#include "bdc.h"

/* Attaches a loaded GMO file to a model object: stores `gmo` and `arg` in `gmo`/`gmoSize`, indexes
   the chunks (`GfxModelIndexChunks`), strips texture directories (`GmoStripTexturePaths`),
   loads the GMO model data `data` (`GmoModelTryLoad`, result ignored), binds the materials
   (`GfxModelBindMaterials`), clears the `g_gfxModelBindClearMask` bits of `flags30`
   (`GfxModelSetFlags30`), prepares the mesh instances (`GmoModelPrepareInstances`), sets the
   flags (`GmoModelSetFlags2E`, `GmoModelSetFlags28`), selects motion 0
   (`GmoModelSelectMotion`), evaluates it once (`GmoMotionUpdateAll`) and, when the model has
   nodes, computes the bind-pose world matrices (`GmoModelComputeWorldMatricesBind`); when the
   file has motion chunks (`motionCount > 0`) it enables motion playback (`GfxModelEnableMotion`). */

void GfxModelBindGmo(GfxModel *self, void *gmo, u32 arg)
{
    self->gmo = gmo;
    self->gmoSize = arg;
    GfxModelIndexChunks(self);
    GmoStripTexturePaths(gmo);
    GmoGetFirstModel(self->gmo);
    GmoModelTryLoad(self->data, gmo, arg, 0);
    GfxModelBindMaterials(self);
    GfxModelSetFlags30(self, g_gfxModelBindClearMask, false);
    GmoModelPrepareInstances(self->data, 0xffff);
    GmoModelSetFlags2E(self->data, 0xffff, 0xfcff);
    GmoModelSetFlags28(self->data, 4, 4);
    GmoModelSetFlags28(self->data, 1, 0);
    GmoModelSelectMotion(0.0f, self->data, 0);
    GmoMotionUpdateAll(0.0f, self->data);
    if (self->data->nodeCount != 0) {
        GmoModelComputeWorldMatricesBind(self->data);
    }
    if (self->motionCount > 0) {
        GfxModelEnableMotion(self);
    }
}
