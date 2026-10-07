// bdc 0x089ee3e8 UiSpriteMngTaskGetField
#include "bdc.h"

/* `GetField` method of the sprite manager task: fields 0..2 from `CoreTaskGetField`, others 0. */

u32 UiSpriteMngTaskGetField(CoreTask *task, u32 field)

{
  u32 result;
  
  result = 0;
  if (field < 3) {
    result = CoreTaskGetField(task,field);
  }
  return result;
}

