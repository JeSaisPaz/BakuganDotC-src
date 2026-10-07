// bdc 0x0890af5c UiLoadingGetField
#include "bdc.h"

/* `GetField` (vtable slot `+0x34`) of the now-loading screen (task 10100 / 0x2774, 0x240 bytes,
   vtable `0x08af47dc`, `UiLoadingCtor`; shared UI objects `0x08ac0e80`): indices 0-2 from
   `CoreTaskGetField`, 3 = state `+0x10`, 4 = step `+0x14`, else 0. */

u32 UiLoadingGetField(UiLoading *self, u32 index)

{
  u32 result = 0;

  if (index < 3) {
    return CoreTaskGetField(&self->base, index);
  }
  if ((s32)index < 4) {
    if (2 < (s32)index) {
      return self->state;
    }
  } else if ((s32)index < 5) {
    result = self->step;
  }
  return result;
}
