// bdc 0x08809aac UiLanguageSelectGetField
#include "bdc.h"

/* `GetField` override (slot 6) of the language-selection screen: 0-2 base (`CoreTaskGetField`), 3
   state, 4 sub-step, 5 word `+0x5c`; anything else returns 0. */

u32 UiLanguageSelectGetField(CoreTask *task, u32 field)

{
  UiLanguageSelect *self = (UiLanguageSelect *)task;

  if (field < 3) {
    return CoreTaskGetField(task, field);
  }
  if ((s32)field < 4) {
    if (2 < (s32)field) {
      return (u32)self->state;
    }
  } else {
    if ((s32)field < 5) {
      return (u32)self->subStep;
    }
    if ((s32)field < 6) {
      return self->word5c;
    }
  }
  return 0;
}
