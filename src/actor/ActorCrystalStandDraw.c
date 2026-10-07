// bdc 0x088a3a5c ActorCrystalStandDraw
#include "bdc.h"

/* Draw method (vtable `0x08af24d4` slot 8) of the `ActorCrystalStandCtor` stage objects: writes
   the model display list (`GfxModelDlWriteState`) only while the draw alpha `+0x6c` (ambient w)
   is positive. */

void ActorCrystalStandDraw(ActorCrystalStand *obj, u32 **dl)
{
  if (!(obj->base.ambient[3] <= 0.0f)) {
    GfxModelDlWriteState(&obj->base, dl);
  }
}
