// bdc 0x088a1884 ActorStageObjLandmarkSetupMaterials
#include "bdc.h"

/* Sets up the animated materials of the landmark `f6_landmark01` (`ActorStageObjLandmarkCtor`):
   UV scroll callbacks on the materials `f6_landmark01` (V speed 1/180, entry `uvScrolls+0x00`,
   `ActorStageObjMaterialWriteScrollUV`), `f6_landmark01_kara_` (V speed 1/180, entry `+0x10`,
   `ActorStageObjMaterialWriteScrollV`) and `f6_landmark01_tama` (V speed 0.005, entry `+0x20`,
   `ActorStageObjMaterialWriteScrollUV`) via `GfxModelSetMaterialAnimCallback`; then
   `ActorStageObjLandmarkMaterialReset` on every material, and on the `tama` (orb) material
   (`GfxModelFindMaterialStateBySubstr`) state list B slot bits 5..7 = 4 and billboard blend mode 2. */

void ActorStageObjLandmarkSetupMaterials(ActorStageObjLandmark *self)
{
  float *scroll = (float *)self->base.uvScrolls;
  GfxMaterialState *state;

  scroll[0] = 0.0f;
  scroll[1] = 0.0055555557f;
  scroll[2] = 0.0f;
  scroll[3] = 0.0f;
  GfxModelSetMaterialAnimCallback(&self->base.base, "f6_landmark01",
                                  ActorStageObjMaterialWriteScrollUV, &scroll[0]);
  scroll[4] = 0.0f;
  scroll[5] = 0.0055555557f;
  scroll[6] = 0.0f;
  scroll[7] = 0.0f;
  GfxModelSetMaterialAnimCallback(&self->base.base, "f6_landmark01_kara_",
                                  ActorStageObjMaterialWriteScrollV, &scroll[4]);
  scroll[8] = 0.0f;
  scroll[9] = 0.005f;
  scroll[10] = 0.0f;
  scroll[11] = 0.0f;
  GfxModelSetMaterialAnimCallback(&self->base.base, "f6_landmark01_tama",
                                  ActorStageObjMaterialWriteScrollUV, &scroll[8]);
  GfxModelForEachMaterial(&self->base.base, ActorStageObjLandmarkMaterialReset, NULL);
  state = GfxModelFindMaterialStateBySubstr(&self->base.base, "f6_landmark01_tama");
  if (state != NULL) {
    state->shadeFlags = (state->shadeFlags & 0x1f) | 0x80;
    state->renderFlags = (state->renderFlags & 0xfc) | 2;
  }
}
