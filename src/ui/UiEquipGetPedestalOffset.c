// bdc 0x08957038 UiEquipGetPedestalOffset
#include "bdc.h"

/* Writes the (x, y) model offset of player slot `slot` of `UiEquip` to `out`: always
   (0, −40) in both the 2-player and 4-player layout tables. */

void UiEquipGetPedestalOffset(float *out, UiScreen *screen, u8 slot)
{
  float table[12] = {0.0f, -40.0f, 0.0f, -40.0f, 0.0f, -40.0f,
                     0.0f, -40.0f, 0.0f, -40.0f, 0.0f, -40.0f};
  const float *e;

  if (((UiEquip *)screen)->playerCount < 3) {
    e = &table[slot * 2];
  } else {
    e = &table[slot * 2 + 4];
  }
  out[0] = e[0];
  out[1] = e[1];
}
