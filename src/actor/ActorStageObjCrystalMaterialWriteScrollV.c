// bdc 0x088b3f70 ActorStageObjCrystalMaterialWriteScrollV
#include "bdc.h"

/* UV-scroll material callback of the crystal stage object (`ActorStageObjCrystalSetupMaterials`,
   stepped by `ActorStageObjCrystalStepUvScroll`): appends GE `TOFFSETV` = `*scroll` to the
   display list `*dl`. */
void ActorStageObjCrystalMaterialWriteScrollV(u32 **dl, const float *scroll)
{
    union {
        float f;
        u32 bits;
    } v;

    v.f = *scroll;
    **dl = (v.bits >> 8) | 0x4b000000;
    *dl = *dl + 1;
}
