// bdc 0x08957988 UiEquipGetSlotViewport
#include "bdc.h"

/* Writes the screen rectangle (x, y, w, h) of player slot `slot` of `UiEquip` to
   `out`: in the 2-player layout halves of the screen ((0, 0, 240, 272) and (240, 0, 240, 272)); in
   the 4-player layout from the table `g_equipViewport4P`. */

void UiEquipGetSlotViewport(float *out, UiScreen *screen, u8 slot)
{
  float rects[24];

  rects[0] = 0.0f;
  rects[1] = 0.0f;
  rects[2] = 240.0f;
  rects[3] = 272.0f;
  rects[4] = 240.0f;
  rects[5] = 0.0f;
  rects[6] = 240.0f;
  rects[7] = 272.0f;
  memcpy(rects + 8, g_equipViewport4P, 0x40);
  if (((UiEquip *)screen)->playerCount < 3) {
    out[0] = rects[slot * 4];
    out[1] = rects[slot * 4 + 1];
    out[2] = rects[slot * 4 + 2];
    out[3] = rects[slot * 4 + 3];
  } else {
    out[0] = rects[slot * 4 + 8];
    out[1] = rects[slot * 4 + 9];
    out[2] = rects[slot * 4 + 10];
    out[3] = rects[slot * 4 + 11];
  }
}
