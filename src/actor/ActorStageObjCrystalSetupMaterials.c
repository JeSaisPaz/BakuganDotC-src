// bdc 0x088b3ffc ActorStageObjCrystalSetupMaterials
#include "bdc.h"

/* Material setup of the crystal stage object (vtable `0x08af2b94` slot 10, also called by
   `ActorStageObjCrystalCtor`): initialises the UV scroll at `+0x380` (`offset = 0`, `speed =
   1/180` (`0x3bb60b61`), `+0x388`/`+0x38c` = 0) and installs the material animation callback
   `0x088b3f70` on the `"fz_crystal02"` material with that scroll as argument
   (`GfxModelSetMaterialAnimCallback`). */

void ActorStageObjCrystalSetupMaterials(ActorStageObjCrystal *self)

{
  self->uvScroll[0] = 0.0f;
  self->uvScroll[1] = 0.0055555557f;
  self->uvScroll[2] = 0.0f;
  self->uvScroll[3] = 0.0f;
  GfxModelSetMaterialAnimCallback
            ((GfxModel *)self,"fz_crystal02",ActorStageObjCrystalMaterialWriteScrollV,self->uvScroll
            );
  return;
}

