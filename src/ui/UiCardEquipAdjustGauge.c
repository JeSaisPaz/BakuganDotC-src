// bdc 0x0896cc54 UiCardEquipAdjustGauge
#include "bdc.h"

/* Adjusts the G-power gauge of the selected Bakugan in `UiCardEquip` while
   Left/Right is held: −10 down to 50 or +10 up to 150, recording the direction in `+0x2a50`.
   Returns 1 when the value changed. */

int UiCardEquipAdjustGauge(UiCardEquip *self)

{
  PadState *pad = self->base.pad;

  if (((int)(char)pad->buttons & 0x80U) == 0) {
    if ((pad->buttons & 0x20) != 0) {
      if (self->gauge[self->selBakugan] < 0x96) {
        self->gauge[self->selBakugan] += 10;
        self->gaugeDir = 1;
        return 1;
      }
    }
  }
  else if (0x32 < self->gauge[self->selBakugan]) {
    self->gauge[self->selBakugan] -= 10;
    self->gaugeDir = 0;
    return 1;
  }
  return 0;
}
