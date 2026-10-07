// bdc 0x08809aac UiLanguageSelectGetField
#include "bdc.h"

/* `GetField` override (slot 6) of the language-selection screen: 0-2 base (`CoreTaskGetField`), 3
   state, 4 sub-step, 5 word `+0x5c`; anything else returns NULL. */

void *UiLanguageSelectGetField(CoreTask *task, u32 field)

{
  UiLanguageSelect *self = (UiLanguageSelect *)task;

  if (field < 3) {
    return (void *)(uintptr_t)CoreTaskGetField(task, field);
  }
  if ((s32)field < 4) {
    if (2 < (s32)field) {
      return (void *)(intptr_t)self->state;
    }
  } else {
    if ((s32)field < 5) {
      return (void *)(intptr_t)self->subStep;
    }
    if ((s32)field < 6) {
      return self->word5c;
    }
  }
  return NULL;
}
