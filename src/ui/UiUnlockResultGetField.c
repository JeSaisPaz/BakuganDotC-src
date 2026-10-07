// bdc 0x089380f8 UiUnlockResultGetField
#include "bdc.h"

/* Slot-6 override (`GetField`) of the UnlockResult screen (task id 375, vtable `0x08af4b24`):
   indices 0-2 come from `CoreTaskGetField`; index 3 returns the phase (`+0x28`); other indices
   return 0. */

u32 UiUnlockResultGetField(UiUnlockResult *self, u32 index)

{
  u32 result;

  result = 0;
  if (index < 3) {
    return CoreTaskGetField((CoreTask *)self, index);
  }
  if (index == 3) {
    result = (self->base).phase;
  }
  return result;
}
