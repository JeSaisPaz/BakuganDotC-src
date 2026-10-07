// bdc 0x088099fc UiLanguageSelectSetField
#include "bdc.h"

/* `SetField` override (slot 5) of the language-selection screen: 0-2
   base (`CoreTaskSetField`); 3 sets the state `+0x10` (resetting the sub-step when it changes), 4
   the sub-step `+0x14`, 5 the word `+0x5c` (checked when the player confirms). */

void UiLanguageSelectSetField(CoreTask *task, u32 field, void *value)

{
  UiLanguageSelect *self = (UiLanguageSelect *)task;

  if (field < 3) {
    CoreTaskSetField(task, field, (u32)(uintptr_t)value);
    return;
  }
  if ((s32)field < 4) {
    if (2 < (s32)field && (void *)(intptr_t)self->state != value) {
      self->state = (s32)(intptr_t)value;
      self->subStep = 0;
      return;
    }
  } else {
    if ((s32)field < 5) {
      self->subStep = (s32)(intptr_t)value;
      return;
    }
    if ((s32)field < 6) {
      self->word5c = value;
    }
  }
}
