// bdc 0x088a3520 ActorCrystalStandMaterialWriteNoSpecular
#include "bdc.h"

/* Material animation callback of the crystal stand (`ActorCrystalStandCtor`, material
   `fz_crystal01_stand_01`): appends a black specular colour (GE `0x57000000`) to the display list
   `*dl`. */

void ActorCrystalStandMaterialWriteNoSpecular(u32 **dl, const void *arg)

{
  **dl = 0x57000000;
  *dl = *dl + 1;
  return;
}

