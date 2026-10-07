// bdc 0x088beb74 GameFieldGetField
#include "bdc.h"

/* `GetField` override (vtable slot 6) of the field scene task (id
   500): field 3 = byte `+0x6a0`, 4 = byte `+0x6a2 != 0`, 5 = phase word `+0x618`; others 0 (no base
   call). Slot 5 keeps `CoreTaskSetField`. */

u32 GameFieldGetField(CoreTask *task, s32 field)
{
  GameFieldTask *t = (GameFieldTask *)task;
  u32 ret = 0;

  if (field < 4) {
    if (2 < field) {
      return t->field3;
    }
  } else {
    if (field < 5) {
      return t->field4 != 0;
    }
    if (field < 6) {
      ret = t->phase;
    }
  }
  return ret;
}
