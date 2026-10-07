// bdc 0x08929214 UiHologramViewGetField
#include "bdc.h"

/* Slot-6 override (`GetField`) of the hologram view screen (task id 392, vtable `0x08af4a44`):
   indices 0-2 come from `CoreTaskGetField`; index 3 returns the phase (`+0x28`); other indices
   return 0. */

u32 UiHologramViewGetField(UiHologramView *self, u32 index)
{
  u32 result = 0;

  if (index < 3) {
    return CoreTaskGetField((CoreTask *)self, index);
  }
  if (index == 3) {
    result = self->base.phase;
  }
  return result;
}
