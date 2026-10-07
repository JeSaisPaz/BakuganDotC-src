// bdc 0x088a90cc ActorStageObjMaterialWriteScrollV
#include "bdc.h"

/* Stage-object UV-scroll material callback (landmark `f6_landmark01_kara_` in
   `ActorStageObjLandmarkSetupMaterials`, mine materials in `ActorStageObjMineSetupMaterials`):
   appends GE `TOFFSETV` = `*scroll` to the display list `*dl`. */
void ActorStageObjMaterialWriteScrollV(u32 **dl, const float *scroll)
{
    union {
        float f;
        u32 bits;
    } v;

    v.f = *scroll;
    **dl = (v.bits >> 8) | 0x4b000000;
    *dl = *dl + 1;
}
