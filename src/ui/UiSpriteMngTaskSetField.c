// bdc 0x089ee3b0 UiSpriteMngTaskSetField
#include "bdc.h"

/* `SetField` method (vtable `0x08af57ac` slot 5) of the 2D sprite manager task
   (`UiSpriteMngEnsureTask`): forwards fields 0..2 to `CoreTaskSetField` and ignores the others.
    */

void UiSpriteMngTaskSetField(CoreTask *task, u32 field, u32 value)

{
  if (field < 3) {
    CoreTaskSetField(task,field,value);
  }
  return;
}

