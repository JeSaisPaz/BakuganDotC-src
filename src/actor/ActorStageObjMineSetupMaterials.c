// bdc 0x088a62e0 ActorStageObjMineSetupMaterials
#include "bdc.h"

/* Material setup virtual of the mine (vtable `0x08af2674` slot 10): for mine types 0..1 installs a
   UV scroll (speed 0.0333 at `+0x2c0`) on material `gimmick_01_kirai_04` (`GfxModelSetMaterialAnimCallback`). */

void ActorStageObjMineSetupMaterials(ActorStageObjMine *self)

{
  float *uv;

  if ((self->mineType < 2) && (-1 < self->mineType)) {
    uv = (float *)self->base.uvScrolls;
    uv[0] = 0.0f;
    uv[1] = 0.033333335f;
    uv[2] = 0.0f;
    uv[3] = 0.0f;
    GfxModelSetMaterialAnimCallback((GfxModel *)self, "gimmick_01_kirai_04",
                                    ActorStageObjMaterialWriteScrollV, uv);
  }
}
