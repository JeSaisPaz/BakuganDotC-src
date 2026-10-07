// bdc 0x088a90f8 ActorStageObjMaterialWriteScrollUV
#include "bdc.h"

/* Stage-object UV-scroll material callback used by `ActorStageObjLandmarkSetupMaterials`
   (materials `f6_landmark01` and `f6_landmark01_tama`): appends GE `TOFFSETU` and `TOFFSETV`, both
   = `*scroll`, to the display list `*dl` (diagonal scroll). */

void ActorStageObjMaterialWriteScrollUV(u32 **dl, const float *scroll)

{
  u32 *p;
  u32 bits;

  bits = *(const u32 *)scroll;
  p = *dl;
  p[0] = (bits >> 8) | 0x4a000000;
  p[1] = (bits >> 8) | 0x4b000000;
  *dl = p + 2;
  return;
}

