// bdc 0x08854ef4 ActorCrystalMaterialWriteScrollV
#include "bdc.h"

/* Crystal material animation callback (installed by `ActorCrystalInitMaterials` on
   `mat_context00`, `mat_context01`, `mat_spel` and `mat_Ambient`): appends GE `TOFFSETV` =
   `*scroll` and a black specular colour (`0x57000000`) to the display list `*dl`. */

void ActorCrystalMaterialWriteScrollV(u32 **dl, const float *scroll)

{
  **dl = (uint)*scroll >> 8 | 0x4b000000;
  *dl = *dl + 1;
  **dl = 0x57000000;
  *dl = *dl + 1;
  return;
}

