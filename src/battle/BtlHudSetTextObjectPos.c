// bdc 0x0883a730 BtlHudSetTextObjectPos
#include "bdc.h"

/* Sets the position (`transform[3][0]` = x, `transform[3][1]` = y, the translation row of the
   placement matrix) of HUD object `slot` in `fabs` (see `BtlHudCreateTextObject`), if it
   exists. */

void BtlHudSetTextObjectPos(float x, float y, BtlHud *self, int slot)

{
  if (self->fabs[slot] != (GfxFab *)0x0) {
    self->fabs[slot]->transform[3][0] = x;
    self->fabs[slot]->transform[3][1] = y;
  }
  return;
}
