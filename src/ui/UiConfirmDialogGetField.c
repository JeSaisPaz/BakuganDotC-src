// bdc 0x0890e508 UiConfirmDialogGetField
#include "bdc.h"

/* Slot-6 override (`GetField`) of the ConfirmDialog screen (task id 510, vtable `0x08af4884`):
   indices 0-2 come from `CoreTaskGetField`; index 3 returns the screen state (`+0x28`); other
   indices return 0. */

u32 UiConfirmDialogGetField(CoreTask *task, u32 index)

{
  u32 value = 0;

  if (index < 3) {
    return CoreTaskGetField(task, index);
  }
  if (index == 3) {
    value = ((UiScreen *)task)->phase;
  }
  return value;
}
