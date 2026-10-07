// bdc 0x0890e4a4 UiConfirmDialogSetField
#include "bdc.h"

/* Slot-5 override (`SetField`) of the ConfirmDialog screen (task id 510, vtable `0x08af4884`):
   indices 0-2 go to `CoreTaskSetField`; index 3 sets the screen state (`+0x28`) and resets the
   sub-state (`+0x2c`) when the value changes. */

void UiConfirmDialogSetField(UiConfirmDialog *task, u32 index, u32 value)

{
  if (index < 3) {
    CoreTaskSetField(&task->base,index,value);
    return;
  }
  if ((index == 3) && (task->state != value)) {
    task->state = value;
    task->subState = 0;
  }
  return;
}

