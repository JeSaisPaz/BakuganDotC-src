// bdc 0x08854eb0 GfxDlWriteTexOffsetUAndSpecular
#include "bdc.h"

/* Material display-list callback of the crystal model's `mat_Ambient` (installed by
   `ActorCrystalInitMaterials`, address loaded at `0x08855494`): appends two GE commands to the
   display list `*dl`: 0x4A (TOFFSETU) with the float `*value` (>> 8) and 0x57 (specular colour) =
   0, advancing `*dl`. */

void GfxDlWriteTexOffsetUAndSpecular(u32 **dl, const float *value)
{
  **dl = (*(const u32 *)value) >> 8 | 0x4a000000;
  *dl = *dl + 1;
  **dl = 0x57000000;
  *dl = *dl + 1;
}
